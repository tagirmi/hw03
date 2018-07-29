#pragma once

#include <cstdlib>
#include <new>
#include <utility>

namespace my {

template<typename T>
struct allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<typename U>
    struct rebind {
        using other = allocator<U>;
    };

    allocator() = default;
    ~allocator() = default;

    template<typename U>
    allocator(const allocator<U>&)
    {
    }

    T* allocate(std::size_t n)
    {
        auto p = std::malloc(n * sizeof(T));
        if (!p)
            throw std::bad_alloc();
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T* p, std::size_t n)
    {
        std::free(p);
    }

    template<typename U, typename ...Args>
    void construct(U* p, Args&&... args)
    {
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(T* p)
    {
        p->~T();
    }
};

} // namespace my
