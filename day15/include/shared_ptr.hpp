#pragma once
#include <cstddef>

template<typename T>
class SharedPtr {
    public:
        SharedPtr() : ctrl(nullptr) {}

        //From raw pointer
        explicit SharedPtr(T* ptr) {
            ctrl = new ControlBlock(ptr);
        }

        SharedPtr(const SharedPtr& other) {
            ctrl = other.ctrl;
            if (ctrl)
                ctrl->ref_count++;
        }

        SharedPtr(SharedPtr&& other) noexcept {
            ctrl = other.ctrl;
            other.ctrl = nullptr;
        }
        
        SharedPtr& operator=(const SharedPtr& other) {
            if (this == &other) return *this;

            release();

            ctrl = other.ctrl;
            if (ctrl) 
                ctrl->ref_count++;

            return *this;
        }

        SharedPtr& operator=(SharedPtr&& other) noexcept {
            if (this == &other) return *this;

            release();

            ctrl = other.ctrl;
            other.ctrl = nullptr;

            return *this;
        }
        
        
        ~SharedPtr() {release();}

        T* get() const {
            return ctrl ? ctrl->ptr :nullptr;
        }

        T& operator*() const {
            return *(ctrl->ptr);
        }

        T* operator->() const {
            return ctrl->ptr;
        }

        size_t use_count() const {
            return ctrl ? ctrl->ref_count : 0;
        }

        explicit operator bool() const {
            return get() != nullptr;
        }

    private:

        struct ControlBlock {
            T* ptr;
            size_t ref_count;

            ControlBlock(T* p)
                : ptr(p), ref_count(1) {}
        };

        ControlBlock* ctrl;

        void release() {
            if (!ctrl) return;

            ctrl->ref_count--;

            if (ctrl->ref_count == 0) {
                delete ctrl->ptr;
                delete ctrl;
            }
        }
};