#pragma once

#define LIKELY(x)       __builtin_expect((x), 1)
#define UNLIKELY(x)     __builtin_expect((x), 0)

#include "memory.hpp"
// TODO: make sure sys instead of linux
#include <sys/mman.h>

namespace uwr {
    template<class T>
    constexpr len_t map_threshold = 4096 / sizeof(T);

    template<class T>
    T* allocate(len_t n);
    template<class T>
    T* deallocate(T* p, len_t n);

    template<class T>
    len_t fix_capacity(len_t n);

    template<class T>
    T_Move<T, T*> realloc(T* data, len_t length, len_t capacity, len_t n);
    template<class T>
    NT_Move<T, T*> realloc(T* data, len_t length, len_t capacity, len_t n);

    template<class T>
    void change_capacity(T*& data, len_t length, len_t& capacity, len_t n);
    template<class T>
    void grow(T*& data, len_t length, len_t& capacity);

    template<class T>
    T* allocate(len_t n) {
        if (n > map_threshold<T>)
            return (T*)mmap(NULL, n * sizeof(T),
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1, 0);
        else
            return (T*)malloc(n * sizeof(T));
    }

    template<class T>
    void deallocate(T* p, len_t n) {
        if (n > map_threshold<T>)
            munmap(p, n * sizeof(T));
        else
            free(p);
    }

    template<class T>
    len_t fix_capacity(len_t n) {
        if (n < map_threshold<T>)
            return std::max(64 / sizeof(T), n);
        return map_threshold<T> * (n / map_threshold<T> + 1);
    }

    template<class T>
    T_Move<T, T*> realloc(T* data, len_t length, len_t capacity, len_t n) {
        int cond = (n > map_threshold<T>) |
            (capacity > map_threshold<T>) << 1;

        switch (cond) {
            case 3:
                return (T*)mremap(data, capacity * sizeof(T),
                            n * sizeof(T), MREMAP_MAYMOVE);
            case 0:
                return ::realloc(data, n * sizeof(T));

            default:
                T* new_data = allocate<T>(n);
                move(new_data, data, length);
                deallocate(data, capacity);
                return new_data;
        }
    }

    template<class T>
    NT_Move<T, T*> realloc(T* data, len_t length, len_t capacity, len_t n) {
        if (capacity > map_threshold<T>) {
            void* new_data = mremap(data, capacity * sizeof(T),
                            n * sizeof(T), 0);
            if (new_data != (void*)-1)
                return (T*)new_data;
        }

        // TODO: if n >= capacity is true then optimize
        T* new_data = allocate<T>(n);
        umove(new_data, data, length);
        destroy(data, length);
        // TODO: checking the same if twice
        deallocate(data, capacity);

        return new_data;
    }

    template<class T>
    void change_capacity(T*& data, len_t length, len_t& capacity, len_t n) {
        len_t new_capacity = fix_capacity<T>(n);
        if (UNLIKELY(new_capacity < map_threshold<T> &&
                    capacity > map_threshold<T>))
            return;

        if (data)
            data = realloc(data, length, capacity, new_capacity);
        else
            data = allocate<T>(new_capacity);

        capacity = new_capacity;
    }

    template<class T>
    void grow(T*& data, len_t length, len_t& capacity) {
        // TODO: likely here (?)
        if (LIKELY(length < capacity))
            return;
        change_capacity(data, length, capacity, capacity * 2 + 1);
    }
}
