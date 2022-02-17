#pragma once

#include <sys/mman.h>

#include "allocator_base.hpp"
#include "../common/memory.hpp"
// TODO: remove
#ifdef UWR_TRACK
#include "../common/alloc_counter.hpp"
#endif

// #include <map>
// std::map<void*, std::vector<double>> succ;

// TODO: remove
// #define UWR_VERBOSE_PRINTING

namespace uwr::mem {

using std::true_type;
using std::false_type;

template<class T>
class hybrid_allocator_exp : public allocator_base<hybrid_allocator_exp<T>, T> {
public:
    using base_t = allocator_base<hybrid_allocator_exp<T>, T>;
    using size_type = typename base_t::size_type;
    using value_type = T;
    using pointer = typename base_t::pointer;

    constexpr hybrid_allocator_exp() noexcept;
    constexpr explicit hybrid_allocator_exp(size_type n);

    UWR_FORCEINLINE constexpr size_type fix_capacity(size_type n) const;
    UWR_FORCEINLINE constexpr pointer alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void realloc(size_type req);
    UWR_FORCEINLINE constexpr void grow(size_type req);
    UWR_FORCEINLINE constexpr bool expand_or_dealloc_and_alloc_raw(size_type req);

    UWR_FORCEINLINE constexpr size_type npages(size_type x) { return (x * sizeof(T) + page_size - 1) / page_size; }

private:
    UWR_FORCEINLINE constexpr int is_big(size_type n) const;
    UWR_FORCEINLINE constexpr int is_big(size_type n, true_type) const;
    UWR_FORCEINLINE constexpr int is_big(size_type n, false_type) const;

    UWR_FORCEINLINE constexpr size_type fix_big_capacity(size_type n) const;
    UWR_FORCEINLINE constexpr size_type fix_small_capacity(size_type n) const;

    UWR_FORCEINLINE constexpr pointer big_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr pointer small_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void big_dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void small_dealloc(pointer data, size_type n) const;

    UWR_FORCEINLINE constexpr void do_grow(size_type req, true_type);
    constexpr void do_grow(size_type req, false_type);

    constexpr pointer do_realloc(size_type req, true_type);
    constexpr pointer do_realloc(size_type req, false_type);

    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, true_type);
    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, false_type);

// TODO: remove
#ifdef UWR_TRACK
public:
    static alloc_counter counter;
#endif
};

// TODO: remove
#ifdef UWR_TRACK
template<class T>
alloc_counter hybrid_allocator_exp<T>::counter("hybrid_allocator_exp");
#endif

template<class T>
constexpr
hybrid_allocator_exp<T>::hybrid_allocator_exp() noexcept
    : base_t() {}

template<class T>
constexpr
hybrid_allocator_exp<T>::hybrid_allocator_exp(size_type n)
    : base_t(n) {}

template<class T>
constexpr typename hybrid_allocator_exp<T>::size_type
hybrid_allocator_exp<T>::fix_capacity(size_type n) const {
#ifndef NDEBUG
    if (!n) return 0;
#endif
    if (this->is_big(n))
        return this->fix_big_capacity(n);
    else
        return this->fix_small_capacity(n);
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::size_type
hybrid_allocator_exp<T>::fix_big_capacity(size_type n) const {
    UWR_ASSERT(this->is_big(n));
    return ((n * sizeof(T) + page_size - 1) / page_size)
        * page_size / sizeof(T);
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::size_type
hybrid_allocator_exp<T>::fix_small_capacity(size_type n) const {
    UWR_ASSERT(!this->is_big(n));
    return std::max((64 + sizeof(T) - 1) / sizeof(T), n);
}

template<class T>
constexpr int
hybrid_allocator_exp<T>::is_big(size_type n) const {
    return this->is_big(n, is_trivially_relocatable<T>());
}

template<class T>
constexpr int
hybrid_allocator_exp<T>::is_big(size_type n, true_type) const {
    return n * sizeof(T) >= page_size;
}

template<class T>
constexpr int
hybrid_allocator_exp<T>::is_big(size_type n, false_type) const {
    return n * sizeof(T) >= 32 * 1024 * 1024;
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::pointer
hybrid_allocator_exp<T>::alloc(size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));

    if (this->is_big(n))
        return this->big_alloc(n);
    else
        return this->small_alloc(n);
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::pointer
hybrid_allocator_exp<T>::big_alloc(size_type n) const {
    UWR_ASSERT(this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    return (pointer)mmap(NULL, n * sizeof(T),
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS,
                -1, 0);
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::pointer
hybrid_allocator_exp<T>::small_alloc(size_type n) const {
    UWR_ASSERT(!this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    return (pointer)malloc(n * sizeof(T));
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::dealloc(pointer data, size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));

    if (this->is_big(n))
        this->big_dealloc(data, n);
    else
        this->small_dealloc(data, n);
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::big_dealloc(pointer data, size_type n) const {
    UWR_ASSERT(this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));
    
    munmap(data, n * sizeof(T));
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::small_dealloc(pointer data, UWR_UNUSED size_type n) const {
    UWR_ASSERT(!this->is_big(n));
    UWR_ASSERT(n == this->fix_capacity(n));

    free(data);
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::realloc(size_type req) {
    req = this->fix_capacity(req);

    // TODO: remove
    #ifdef UWR_TRACK
    if (this->m_capacity)
        counter.grows(double(req) / this->m_capacity);
    #endif

    if (UWR_LIKELY(!!this->m_capacity)) {
        this->m_data = this->do_realloc(req,
            is_trivially_relocatable<T>());
    }
    else {
        this->m_data = this->alloc(req);
    }

    this->m_capacity = req;
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::pointer
hybrid_allocator_exp<T>::do_realloc(size_type req, true_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            return (pointer)::realloc(
                    (void*)this->m_data, req * sizeof(T));
        }
        case 0b10: { /* new size is big, old is small */
            pointer new_data = this->big_alloc(req);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b11: { /* both are big sizes */
            // TODO: remove
            #ifdef UWR_TRACK
            pointer ret = (pointer)mremap(
                    (void*)this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);
            counter.mremaps(this->m_data == ret);
            if (this->m_data == ret) {
                counter.objects(this->m_capacity);
            }
            return ret;
            #else
            return (pointer)mremap(
                    (void*)this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);
            #endif
        }
        default: { /* impossible */
            UWR_ASSERT(false);
            return nullptr; // keep compiler happy
        }
    }
}

template<class T>
constexpr typename hybrid_allocator_exp<T>::pointer
hybrid_allocator_exp<T>::do_realloc(size_type req, false_type) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(req) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            pointer new_data = this->small_alloc(req);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b10: { /* new size is big, old is small */
            pointer new_data = this->big_alloc(req);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b11: { /* both are big sizes */
            pointer new_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

            // TODO: remove
            #ifdef UWR_TRACK
            counter.mremaps(this->m_data == new_data);
            if (this->m_data == new_data) {
                counter.objects(this->m_capacity);
            }
            #endif

            // TODO: remove
            #ifdef UWR_VERBOSE_PRINTING
            if (new_data == this->m_data) {
                std::cout << "ha: "
                    << npages(this->m_capacity)
                    << " -> " << npages(req) << " " 
                    << (new_data == this->m_data)
                    << std::endl;
            }
            #endif

            if (new_data == (pointer)-1) {
                new_data = this->big_alloc(req);
                umove_and_destroy(new_data, this->m_data, this->m_size);
                this->big_dealloc(this->m_data, this->m_capacity);
            }

            return new_data;
        }
        default: { /* impossible */
            UWR_ASSERT(false);
            return nullptr; // keep compiler happy
        }
    }
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::grow(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    if (UWR_LIKELY(!!this->m_capacity)) {
        this->do_grow(req, is_trivially_relocatable<T>());
    }
    else {
        req = std::max(2 * this->m_capacity, req);
        req = this->fix_capacity(req);
        this->m_data = this->alloc(req);
        this->m_capacity = req;
    }
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::do_grow(size_type req, true_type) {
    req = std::max(2 * this->m_capacity, req);
    req = this->fix_capacity(req);
    this->m_data = this->do_realloc(req, true_type());
    this->m_capacity = req;
}

template<class T>
constexpr void
hybrid_allocator_exp<T>::do_grow(size_type req, false_type) {
    size_type next = std::max(2 * this->m_capacity, req);
    uint8_t cond = this->is_big(this->m_capacity) |
                   this->is_big(next) << 1;

    switch (cond) {
        case 0b00: { /* both are small sizes */
            next = this->fix_small_capacity(next);
            pointer new_data = this->small_alloc(next);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = new_data;
            this->m_capacity = next;

            break;
        }
        case 0b10: { /* new size is big, old is small */
            next = this->fix_big_capacity(next);
            pointer new_data = this->big_alloc(next);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);
            this->m_data = new_data;
            this->m_capacity = next;

            break;
        }
        case 0b11: { /* both are big sizes */
            size_type min_pages = this->npages(req);
            size_type delta = this->npages(next) - min_pages;

            while (delta) {
                pointer new_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    (min_pages + delta) * page_size,
                    0);
                if (new_data != (pointer)-1) {
                    #ifdef UWR_TRACK
                    counter.mremaps(1);
                    counter.objects(this->m_capacity);
                    counter.grows(double((min_pages + delta) * page_size / sizeof(T)) / this->m_capacity);
                    #endif
                    this->m_capacity = (min_pages + delta) * page_size / sizeof(T);
                    return;
                }
                delta = delta / 2;
            }

            next = this->fix_big_capacity(next);
            #ifdef UWR_TRACK
            counter.mremaps(0);
            counter.grows(double(next) / this->m_capacity);
            #endif
            pointer new_data = this->big_alloc(next);
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->big_dealloc(this->m_data, this->m_capacity);
            this->m_data = new_data;
            this->m_capacity = next;

            break;
        }
        default: { /* impossible */
            UWR_ASSERT(false);
            break;
        }
    }
}

template<class T>
constexpr bool
hybrid_allocator_exp<T>::expand_or_dealloc_and_alloc_raw(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    req = this->fix_capacity(req);

   // TODO: remove
   #ifdef UWR_TRACK
   if (this->m_capacity)
        counter.grows(double(req) / this->m_capacity);
   #endif

    if (UWR_LIKELY(!!this->m_capacity)) {
        return this->do_expand_or_dealloc_and_alloc_raw(req,
                is_trivially_relocatable<T>());
    }
    else {
        this->m_data = this->alloc(req);
        this->m_capacity = req;

        return false;
    }
}

template<class T>
constexpr bool
hybrid_allocator_exp<T>::do_expand_or_dealloc_and_alloc_raw(size_type req, true_type) {
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
            // TODO: remove
            #ifdef UWR_TRACK
            pointer save = this->m_data;
            #endif

            this->m_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);

            // TODO: remove
            #ifdef UWR_TRACK
            counter.mremaps(save == this->m_data);
            if (save == this->m_data) {
                counter.objects(this->m_capacity);
            }
            #endif

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

template<class T>
constexpr bool
hybrid_allocator_exp<T>::do_expand_or_dealloc_and_alloc_raw(size_type req, false_type) {
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
            pointer new_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

            // TODO: remove
            #ifdef UWR_TRACK
            counter.mremaps(this->m_data == (pointer)new_data);
            if (this->m_data == (pointer)new_data) {
                counter.objects(this->m_capacity);
            }
            #endif

            if (new_data == (pointer)-1) {
                // first alloc, then dealloc - why?
                // mremap seems to behave strange
                // if you do the other way around
                // and dramatically reduces in-place
                // reallocations in the future
                new_data = this->big_alloc(req);
                destroy(this->m_data, this->m_size);
                this->big_dealloc(this->m_data, this->m_capacity);
                this->m_data = new_data;
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

} // namespace uwr::mem
