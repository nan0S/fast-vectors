#pragma once

// TODO: make sure sys instead of linux
#include <sys/mman.h>

#include "define.hpp"
#include "memory.hpp"

namespace uwr::mem {

/*
 * allocate n objects of type T
 */
template<class T>
T* allocate(len_t n);

/*
 * is allocating n objects of type T
 * big allocation
 */
template<class T>
int is_big(len_t n);

/*
 * allocate using big allocation
 */
template<class T>
T* big_allocate(len_t n);

/*
 * allocate using small allocation
 */
template<class T>
T* small_allocate(len_t n);

/*
 * deallocate n objects of type T
 * under address `data`
 */
template<class T>
void deallocate(T* data, len_t n);

/*
 * deallocate using big deallocation
 */
template<class T>
void big_deallocate(T* data, len_t n);

/*
 * deallocate using small deallocation
 */
template<class T>
void small_deallocate(T* data, UWR_UNUSED len_t n);

/*
 * reallocate (possibly in place) memory at
 * `data` (possibly nullptr) of `len` objects of
 * type T (in total has capacity of `cap` objects
 * of type T) to `req` objects of type T and
 * assign new address to `data`
 */
template<class T>
T_Move_C<T> reallocate(T*& data, len_t len, len_t cap, len_t req);
template<class T>
NT_Move_C<T> reallocate(T*& data, len_t len, len_t cap, len_t req);

/*
 * try to expand in place,
 * return true on success, false on failure,
 * if failure, destroy data at `data` address
 * and assign to it newly allocated chunk of memory
 *
 * NOTE: do not copy the data to the new address (raw),
 *       but still destroy old data,
 *
 * USAGE: this function should be used when you want to
 *        allocate more memory and you don't care about
 *        objects at `data` (they would be destroyed in
 *        a while)
 */
template<class T>
bool expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req);

/*
 * expand_in_place_or_alloc_raw helper functions
 */
template<class T>
static T_Move_C<T, bool> do_expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req);
template<class T>
static NT_Move_C<T, bool> do_expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req);

/*
 * the same as expand_in_place_or_destroy_and_alloc_raw(data, len, cap, req)
 * but do not destroy and deallocate old data, but still allocate new
 * as raw memory region, on failed expansion places new address into
 * `out_ptr` variable
 *
 * USAGE: this function should be used when you want to allocate
 *        more memory, but still somehow care about the data at
 *        `data`, e.g. it should be copied to new address but in
 *        special way
 */
template<class T>
bool expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr);

/*
 * expand_in_place_or_alloc_raw helper functions
 */
template<class T>
static T_Move_C<T, bool> do_expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr);
template<class T>
static NT_Move_C<T, bool> do_expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr);


/*
 * implementations
 */
template<class T>
T* allocate(len_t n) {
    if (is_big<T>(n))
        return big_allocate<T>(n);
    else
        return small_allocate<T>(n);
}

template<class T>
int is_big(len_t n) {
    return n * sizeof(T) >= page_size;
}

template<class T>
T* big_allocate(len_t n) {
    return (T*)mmap(NULL, n * sizeof(T),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0);
}

template<class T>
T* small_allocate(len_t n) {
    UWR_ASSERT(!is_big<T>(n));
    return (T*)malloc(n * sizeof(T));
}

template<class T>
void deallocate(T* data, len_t n) {
    if (is_big<T>(n))
        big_deallocate(data, n);
    else
        small_deallocate(data, n);
}

template<class T>
void big_deallocate(T* data, len_t n) {
    UWR_ASSERT(is_big<T>(n));
    munmap(data, n * sizeof(T));
}

template<class T>
void small_deallocate(T* data, UWR_UNUSED len_t n) {
    UWR_ASSERT(!is_big<T>(n));
    free(data);
}

template<class T>
T_Move_C<T> reallocate(T*& data, len_t len, len_t cap, len_t req) {
    UWR_ASSERT(req > cap);
    int cond = is_big<T>(cap) | is_big<T>(req) << 1;

    switch (cond) {
        case 0: { /* both are small sizes */
            data = (T*)realloc(data, req * sizeof(T));
            break;
        }
        case 2: { /* new size is big, old is small */
            T* new_data = big_allocate<T>(req);
            umove(new_data, data, len);
            destroy(data, len);
            small_deallocate(data, cap);
            data = new_data;
            break;
        }
        case 3: { /* both are big sizes */
            data = (T*)mremap(data, len * sizeof(T),
                              req * sizeof(T), MREMAP_MAYMOVE);
            break;
        }
        default: /* impossible */
            UWR_ASSERT(false);
    }
}

template<class T>
NT_Move_C<T> reallocate(T*& data, len_t len, len_t cap, len_t req) {
    UWR_ASSERT(req > cap);
    int cond = is_big<T>(cap) | is_big<T>(req) << 1;

    switch (cond) {
        case 0: { /* both are small sizes */
            T* new_data = small_allocate<T>(req);
            umove(new_data, data, len);
            destroy(data, len);
            small_deallocate(data, cap);
            data = new_data;
            break;
        }
        case 2: { /* new size is big, old is small */
            T* new_data = big_allocate<T>(req);
            umove(new_data, data, len);
            destroy(data, len);
            small_deallocate(data, cap);
            data = new_data;
            break;
        }
        case 3: { /* both are big sizes */
            T* new_data = (T*)mremap(data, cap * sizeof(T),
                                     req * sizeof(T), 0);
            if (new_data != (T*)-1)
                data = new_data;
            if (new_data == (T*)-1) {
                new_data = big_allocate<T>(req);
                umove(new_data, data, len);
                destroy(data, len);
                big_deallocate(data, cap);
                data = new_data;
            }
            break;
        }
        default: /* impossible */
            UWR_ASSERT(false);
    }
}

template<class T>
bool expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req) {
    UWR_ASSERT(req > len);

    if (UWR_LIKELY(!!data))
        return do_expand_in_place_or_destroy_and_alloc_raw(data, len, cap, req);
    else {
        data = allocate<T>(req);
        return false;
    }
}

template<class T>
T_Move_C<T, bool> do_expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req) {
    UWR_ASSERT(req > cap);

    int cond = is_big<T>(cap) | is_big<T>(req) << 1;
    destroy(data, len);

    switch (cond) {
        case 0: { /* both are small sizes */
            small_deallocate(data, cap);
            data = small_allocate<T>(req);
            break;
        }
        case 2: { /* new size is big, old is small */
            small_deallocate(data, cap);
            data = big_allocate<T>(req);
            break;
        }
        case 3: { /* both are big sizes */
            // TODO: check alternative
            data = (T*)mremap(data, cap * sizeof(T),
                              req * sizeof(T), MREMAP_MAYMOVE);
            break;
        }
        default: /* impossible */
            UWR_ASSERT(false);
    }

    // always return false as these objects are trivial
    // NOTE: assumption is that you don't care about
    //       objects under `data` address, so return
    //       always false so that caller thinks they
    //       are gone
    return false;
}

template<class T>
NT_Move_C<T, bool> do_expand_in_place_or_destroy_and_alloc_raw(T*& data, len_t len, len_t cap, len_t req) {
    UWR_ASSERT(req > len);
    int cond = is_big<T>(cap) | is_big<T>(req) << 1;

    switch (cond) {
        case 0: { /* both are small */
            destroy(data, len);
            small_deallocate(data, cap);
            data = small_allocate<T>(req);
            return false;
        }
        case 2: { /* new size is big, old is small */
            destroy(data, len);
            small_deallocate(data, cap);
            data = big_allocate<T>(req);
            return false;
        }
        case 3: { /* both are big sizes */
            void* new_data = mremap(data, cap * sizeof(T),
                                    req * sizeof(T), 0);
            if (new_data == (void*)-1) {
                destroy(data, len);
                big_deallocate(data, cap);
                data = big_allocate<T>(req);
                return false;
            }
            else {
                UWR_ASSERT((T*)new_data == data);
                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}

template<class T>
bool expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr) {
    UWR_ASSERT(req > len);

    if (UWR_LIKELY(!!data))
        return do_expand_in_place_or_alloc_raw(data, cap, req, out_ptr);
    else {
        out_ptr = allocate<T>(req);
        return false;
    }
}

// TODO: simplify
template<class T>
T_Move_C<T, bool> do_expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr) {
    UWR_ASSERT(req > cap);
    int cond = is_big<T>(cap) | is_big<T>(req) << 1;

    switch (cond) {
        case 0: { /* both are small sizes */
            out_ptr = small_allocate<T>(req);
            return false;
        }
        case 2: { /* new size is big, old is small */
            out_ptr = big_allocate<T>(req);
            return false;
        }
        case 3: { /* both are big sizes */
            out_ptr = (T*)mremap(data, cap * sizeof(T),
                                 req * sizeof(T), 0);
            if (out_ptr == (T*)-1) {
                out_ptr = big_allocate<T>(req);
                return false;
            }
            else {
                UWR_ASSERT(out_ptr == data);
                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}

template<class T>
NT_Move_C<T, bool> do_expand_in_place_or_alloc_raw(T*& data, len_t cap, len_t req, T*& out_ptr) {
    UWR_ASSERT(req > cap);
    int cond = is_big<T>(cap) | is_big<T>(req) << 1;

    switch (cond) {
        case 0: { /* both are small sizes */
            out_ptr = small_allocate<T>(req);
            return false;
        }
        case 2: { /* new size is big, old is small */
            out_ptr = big_allocate<T>(req);
            return false;
        }
        case 3: { /* both are big sizes */
            out_ptr = (T*)mremap(data, cap * sizeof(T),
                                 req * sizeof(T), 0);
            if (out_ptr == (T*)-1) {
                out_ptr = big_allocate<T>(req);
                return false;
            }
            else {
                UWR_ASSERT(out_ptr == data);
                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}    


} // namespace mem::uwr
