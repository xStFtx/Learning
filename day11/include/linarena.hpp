#pragma once
#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <utility>
#include <cassert>
#include <stack>
#include <type_traits>
#include <functional>

class LinArena {
public:
    LinArena(size_t size_): cap_(size_), offset_(0), buff_(static_cast<uint8_t*>(::operator new(size_))) {}
    
    LinArena(const LinArena&) = delete;
    LinArena& operator=(const LinArena&) = delete;

    LinArena(LinArena&& other) noexcept : buff_(other.buff_), cap_(other.cap_), offset_(other.offset_), destructor_list(std::move(other.destructor_list))
    {
        other.buff_ = nullptr;
        other.cap_ = 0;
        other.offset_ = 0;
    }

    ~LinArena() {
        reset();                // Call destructors
        ::operator delete(buff_); 
    }

    void* allocate(size_t size_, size_t alignment = alignof(max_align_t)) {
        if (buff_ == nullptr)
            throw std::logic_error("Arena is in moved-from state");

        // Check alignment is power of 2
        assert((alignment != 0) && ((alignment & (alignment - 1)) == 0));

        uintptr_t curr = reinterpret_cast<uintptr_t>(buff_) + offset_;
        uintptr_t aligned = align_up(curr, alignment);
        size_t new_offset = aligned - reinterpret_cast<uintptr_t>(buff_) + size_;

        if (new_offset > cap_)
            throw std::bad_alloc();

        offset_ = new_offset;
        return reinterpret_cast<void*>(aligned);
    }

    void reset() noexcept {
        // Call all tracked destructors in reverse order
        while (!destructor_list.empty()) {
            destructor_list.top()();
            destructor_list.pop();
        }
        offset_ = 0;
    }

    template <typename T, typename... Args>
    T* make(Args&&... args) {
        void* mem = allocate(sizeof(T), alignof(T));

        T* obj = new (mem) T(std::forward<Args>(args)...);

        // Track destructor if not trivial
        if constexpr (!std::is_trivially_destructible<T>::value) {
            destructor_list.push([obj]() { obj->~T(); });
        }

        return obj;
    }

private:
    static uintptr_t align_up(uintptr_t ptr, size_t alignment) {
        return (ptr + alignment - 1) & ~(alignment - 1);
    }

    uint8_t* buff_;
    size_t cap_;
    size_t offset_;

    // Stack of type-erased destructors
    std::stack<std::function<void()>> destructor_list;
};