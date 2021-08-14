#pragma once

#include <sys/mman.h>

#include "../common/memory.hpp"
#include "allocator_base.hpp"

namespace uwr::mem {

using std::true_type;
using std::false_type;

template<class T>
class hybrid_allocator : public allocator_base<hybrid_allocator<T>, T> {
public:
    using base_t = allocator_base<hybrid_allocator<T>, T>;
    using size_type = typename base_t::size_type;
    using value_type = T;
    using pointer = typename base_t::pointer;

    constexpr hybrid_allocator() noexcept;
    constexpr explicit hybrid_allocator(size_type n);

    UWR_FORCEINLINE constexpr size_type fix_capacity(size_type n) const;

    UWR_FORCEINLINE constexpr pointer alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void realloc(size_type req);

    UWR_FORCEINLINE constexpr bool expand_or_alloc_raw(size_type req, pointer& out_ptr);
    UWR_FORCEINLINE constexpr bool expand_or_dealloc_and_alloc_raw(size_type req);

private:
    UWR_FORCEINLINE constexpr int is_big(size_type n) const;

    UWR_FORCEINLINE constexpr pointer big_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr pointer small_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void big_dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void small_dealloc(pointer data, size_type n) const;

    constexpr pointer do_realloc(size_type req, true_type);
    constexpr pointer do_realloc(size_type req, false_type);

    constexpr bool do_expand_or_alloc_raw(size_type req, pointer& out_ptr, true_type);
    constexpr bool do_expand_or_alloc_raw(size_type req, pointer& out_ptr, false_type);
    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, true_type);
    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, false_type);

public:
    static int mremaps;
    static int success;
    UWR_FORCEINLINE constexpr size_type npages(size_type x) { return x * sizeof(T) / page_size; }
};

template<class T>
int hybrid_allocator<T>::mremaps = 0;
template<class T>
int hybrid_allocator<T>::success = 0;

template<class T>
constexpr
hybrid_allocator<T>::hybrid_allocator() noexcept
    : base_t() {}

template<class T>
constexpr
hybrid_allocator<T>::hybrid_allocator(size_type n)
    : base_t(n) {}

template<class T>
constexpr typename hybrid_allocator<T>::size_type
hybrid_allocator<T>::fix_capacity(size_type n) const {
#ifndef NDEBUG
    if (!n) return 0;
#endif
    if (this->is_big(n))
        return ((n * sizeof(T) + page_size - 1) / page_size)
            * page_size / sizeof(T);
        // TODO: change
        // return ((n * sizeof(T) + page_size - 1) / page_size + 1)
            // * page_size / sizeof(T);
    else
        return std::max((64 + sizeof(T) - 1) / sizeof(T), n);
}

template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::alloc(size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));

    if (this->is_big(n))
        return this->big_alloc(n);
    else
        return this->small_alloc(n);
}

template<class T>
constexpr void
hybrid_allocator<T>::dealloc(pointer data, size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));

    if (this->is_big(n))
        this->big_dealloc(data, n);
    else
        this->small_dealloc(data, n);
}

template<class T>
constexpr void
hybrid_allocator<T>::realloc(size_type req) {
    req = this->fix_capacity(req);
    if (UWR_LIKELY(!!this->m_data))
        // this->m_data = this->do_realloc(req);
        this->m_data = this->do_realloc(req,
            std::is_trivially_move_constructible<T>());
    else
        this->m_data = this->alloc(req);
    this->m_capacity = req;
}

template<class T>
constexpr bool
hybrid_allocator<T>::expand_or_alloc_raw(size_type req, pointer& out_ptr) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    if (UWR_LIKELY(!!this->m_data)) {
        return this->do_expand_or_alloc_raw(req, out_ptr,
                std::is_trivially_move_constructible<T>());
    }
    else {
        out_ptr = this->alloc(req);
        return false;
    }
}

template<class T>
constexpr bool
hybrid_allocator<T>::expand_or_dealloc_and_alloc_raw(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    req = this->fix_capacity(req);

    if (UWR_LIKELY(!!this->m_data))
        return this->do_expand_or_dealloc_and_alloc_raw(req,
                std::is_trivially_move_constructible<T>());
    else {
        this->m_data = this->alloc(req);
        this->m_capacity = req;

        return false;
    }
}

template<class T>
constexpr int
hybrid_allocator<T>::is_big(size_type n) const {
    return n * sizeof(T) > page_size;
}

template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::big_alloc(size_type n) const {
    UWR_ASSERT(this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    return (pointer)mmap(NULL, n * sizeof(T),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0);
}

template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::small_alloc(size_type n) const {
    UWR_ASSERT(!this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    return (pointer)malloc(n * sizeof(T));
}

template<class T>
constexpr void
hybrid_allocator<T>::big_dealloc(pointer data, size_type n) const {
    UWR_ASSERT(this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));
    
    munmap(data, n * sizeof(T));
}

template<class T>
constexpr void
hybrid_allocator<T>::small_dealloc(pointer data, UWR_UNUSED size_type n) const {
    UWR_ASSERT(!this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    free(data);
}

#if 1
template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::do_realloc(size_type req, true_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            return (pointer)::realloc(this->m_data,
                    req * sizeof(T));
        }
        case 0b10: { /* new size is big, old is small */
            pointer new_data = this->big_alloc(req);

            umove(new_data, this->m_data, this->m_size);
            destroy(this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b11: { /* both are big sizes */
            return (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);
        }
        default: { /* impossible */
            UWR_ASSERT(false);
            return nullptr; // keep compiler happy
        }
    }
}
#else
template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::do_realloc(size_type req, true_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    if (this->is_big(req) != this->is_big(this->m_capacity)) {
        pointer new_data = this->alloc(req);
        std::memcpy(new_data, this->m_data, this->m_size * sizeof(T));
        this->dealloc(this->m_data, this->m_capacity);

        return new_data;
    }
    else {
        if (this->is_big(this->m_capacity))
            return (pointer)mremap(
                    this->m_data, this->m_capacity * sizeof(T),
                    req * sizeof(T), MREMAP_MAYMOVE);
        else
            return (pointer)::realloc(this->m_data,
                                      req * sizeof(T));
    }
}
#endif

#if 1
template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::do_realloc(size_type req, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            pointer new_data = this->small_alloc(req);

            umove(new_data, this->m_data, this->m_size);
            destroy(this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b10: { /* new size is big, old is small */
            pointer new_data = this->big_alloc(req);

            umove(new_data, this->m_data, this->m_size);
            destroy(this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b11: { /* both are big sizes */
            pointer new_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

            // if (new_data == this->m_data) {
                // std::cout << npages(this->m_capacity) << " -> "
                // << npages(req) << " " 
                // << (new_data == this->m_data)
                // << std::endl;

            // }

            ++mremaps;
            if (new_data == (pointer)-1) {
                new_data = this->big_alloc(req);
                umove(new_data, this->m_data, this->m_size);
                destroy(this->m_data, this->m_size);
                this->big_dealloc(this->m_data, this->m_capacity);
            }
            else
                ++success;

            return new_data;
        }
        default: { /* impossible */
            UWR_ASSERT(false);
            return nullptr; // keep compiler happy
        }
    }
}
#else
template<class T>
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::do_realloc(size_type req, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));
    
    if (this->is_big(this->m_capacity)) {
        pointer new_data = (pointer)mremap(
                this->m_data,
                this->m_capacity * sizeof(T),
                req * sizeof(T), 0);
        if (new_data != (pointer)-1)
            return new_data;
    }

    pointer new_data = this->alloc(req);
    umove(new_data, this->m_data, this->m_size);
    destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data, this->m_capacity);

    return new_data;
}
#endif

// TODO: simplify
template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_alloc_raw(size_type req, pointer& out_ptr, true_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            out_ptr = this->small_alloc(req);
            return false;
        }
        case 0b10: { /* new size is big, old is small */
            out_ptr = this->big_alloc(req);
            return false;
        }
        case 0b11: { /* both are big sizes */
            out_ptr = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

            if (out_ptr == (pointer)-1) {
                out_ptr = this->big_alloc(req);
                return false;
            }
            else {
                UWR_ASSERT(out_ptr == this->m_data);
                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}

#if 1
template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_alloc_raw(size_type req, pointer& out_ptr, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            out_ptr = this->small_alloc(req);
            return false;
        }
        case 0b10: { /* new size is big, old is small */
            out_ptr = this->big_alloc(req);
            return false;
        }
        case 0b11: { /* both are big sizes */
            out_ptr = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

            if (out_ptr == (pointer)-1) {
                out_ptr = this->big_alloc(req);
                return false;
            }
            else {
                UWR_ASSERT(out_ptr == this->m_data);
                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}    
#else
template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_alloc_raw(size_type req, pointer& out_ptr, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));
    
    if (this->is_big(this->m_capacity)) {
        out_ptr = (pointer)mremap(
                this->m_data,
                this->m_capacity * sizeof(T),
                req * sizeof(T), 0);
        if (out_ptr != (pointer)-1)
            return true;
        out_ptr = this->big_alloc(req);
        return false;
    }
    else {
        out_ptr = this->alloc(req);
        return false;
    }

}    
#endif

template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_dealloc_and_alloc_raw(size_type req, true_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;
    destroy(this->m_data, this->m_size);

    switch (cond) {
        case 0b00: { /* both are small sizes */
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = this->small_alloc(req);

            break;
        }
        case 0b10: { /* new size is big, old is small */
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = this->big_alloc(req);

            break;
        }
        case 0b11: { /* both are big sizes */
            this->m_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);

            break;
        }
        default: /* impossible */
            UWR_ASSERT(false);
    }

    this->m_capacity = req;

    // always return false as these objects are trivial
    // NOTE: assumption is that you don't care about
    //       objects under `data` address, so return
    //       always false so that caller thinks they
    //       are gone
    return false;
}

#if 1
template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_dealloc_and_alloc_raw(size_type req, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small */
            destroy(this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = this->small_alloc(req);
            this->m_capacity = req;

            return false;
        }
        case 0b10: { /* new size is big, old is small */
            destroy(this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = this->big_alloc(req);
            this->m_capacity = req;

            return false;
        }
        case 0b11: { /* both are big sizes */
            void* new_data = mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    0);

            if (new_data == (void*)-1) {
                destroy(this->m_data, this->m_size);
                this->big_dealloc(this->m_data, this->m_capacity);
                this->m_data = this->big_alloc(req);
                this->m_capacity = req;

                return false;
            }
            else {
                UWR_ASSERT((pointer)new_data == this->m_data);
                this->m_capacity = req;

                return true;
            }
        }
        default: /* impossible */
            UWR_ASSERT(false);
            return false; // keep compiler happy
    }
}
#else
template<class T>
constexpr bool
hybrid_allocator<T>::do_expand_or_dealloc_and_alloc_raw(size_type req, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    if (this->is_big(this->m_capacity)) {
        void* new_data = mremap(
                this->m_data,
                this->m_capacity * sizeof(T),
                req * sizeof(T),
                0);

        if (new_data != (void*)-1) {
            this->m_capacity = req;
            return true;
        }

        destroy(this->m_data, this->m_size);
        this->big_dealloc(this->m_data, this->m_capacity);
        this->m_data = this->big_alloc(req);
        this->m_capacity = req;

        return false;
    }

    destroy(this->m_data, this->m_size);
    this->small_dealloc(this->m_data, this->m_capacity);
    this->m_data = this->alloc(req);
    this->m_capacity = req;

    return false;
}
#endif

} // namespace uwr::mem
