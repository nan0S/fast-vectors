#pragma once

#include <algorithm>
#include <cstddef>
#include <cassert>
#include <sys/mman.h>

inline constexpr size_t pagesize = 4096;

template<class T>
T* allocate(size_t n) {
    T* data = (T*)mmap(NULL, n * sizeof(T),
                       PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS,
                       -1, 0);
    std::uninitialized_default_construct_n(data, n);
    return data;
}

template<class T>
void deallocate(T* data, size_t n) {
    std::destroy_n(data, n);
    munmap(data, n * sizeof(T));
}

template<class T>
T* reallocate(T* data, size_t n, size_t req) {
    assert(req >= n);
    T* new_data = (T*)mremap((void*)data,
                              n * sizeof(T),
                              req * sizeof(T),
                              MREMAP_MAYMOVE);
    std::uninitialized_default_construct(
        new_data + n, new_data + req);
    return new_data; 
}
