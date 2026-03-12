#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <future>

class thread_pool {
    public:
        explicit thread_pool(size_t n) {
            for (size_t i = 0; i < n; ++i) {
                workers.emplace_back([this]{
                worker_loop();
                });
            }
        }
        ~thread_pool() {

            {
                std::lock_guard lock(mtx);
                stop = true;
            
            }
            cv.notify_all();
            
            for (auto& t : workers)
                t.join();
            
        };
        template <typename F>
        auto submit(F&& f) {

            using return_type = std::invoke_result_t<F>;
            
            auto task_ptr = std::make_shared<std::packaged_task<return_type()>>{
                std::forward<F>(f)
            };

            std::future<return_type> fut = task_ptr->get_future();

            {
                std::lock_guard lock(mtx);

                if (stop)
                    throw std::runtime_error("submit on stopped pool");

                tasks.emplace([task_ptr]{
                    (*task_ptr);
                });
            }

            cv.notify_one();
            return fut;
        }; 

        size_t pending_tasks() {
           std::lock_guard lock(mtx);
           return tasks.size();
        };

        template<typename F>
        void parallel_for(size_t begin, size_t end, F&& func){
            if (begin > end)
                throw std::range_error("begin must be smaller than end");
            
            std::vector<std::future<void>> futures;
            for(size_t i = begin; i < end; ++i){
                futures.emplace_back(submit[i, &func]{
                    func(i);
                })
            };

            for (auto& f : futures)
                f.get();
        };

    private:
        void worker_loop() {
            while(true) {
                std::function<void()> task;
                //scope block for liftetime control
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [this] {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty())
                        return;
                    
                    task = std::move(tasks.front());
                    tasks.pop();
                } 

                task();
            }
        };

        std::vector<std::thread> workers;

        std::queue<std::function<void()>> tasks;
        //protection for the shared queue
        std::mutex mtx;
        //for sleep; not waste resources
        std::condition_variable cv;

        //safe stopping
        std::atomic<bool> stop{false};

};