#pragma once

#include <sys/mman.h>

#include "../common/memory.hpp"
#include "allocator_base.hpp"

// TODO: remove
#define UWR_TRACK
// #define UWR_VERBOSE_PRINTING

// TODO: remove
#ifdef UWR_TRACK
#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
using namespace boost::accumulators;
#endif

namespace uwr::mem {

using std::true_type;
using std::false_type;

// TODO: remove
#ifdef UWR_TRACK

struct counters {
    static accumulator_set<int,
        features<tag::count,
                 tag::sum,
                 tag::mean>> mremaps;
    static accumulator_set<double,
        features<tag::mean>> grows;

    static void print() {
        std::cout << "\n==== hybrid_allocator::print() ====\n"
                  << "total mremaps   = " << count(mremaps) << "\n"
                  << "success mremaps = " << sum(mremaps) << "\n"
                  << "mean success    = " << mean(mremaps) << "\n"
                  << "mean grow       = " << mean(grows) << "\n"
                  << std::endl;
    }
};

accumulator_set<int,
    features<tag::count,
             tag::sum,
             tag::mean>> counters::mremaps;
accumulator_set<double,
    features<tag::mean>> counters::grows;

#endif // UWR_TRACK

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
    UWR_FORCEINLINE constexpr bool expand_or_dealloc_and_alloc_raw(size_type req);

private:
    UWR_FORCEINLINE constexpr int is_big(size_type n) const;

    UWR_FORCEINLINE constexpr pointer big_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr pointer small_alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void big_dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void small_dealloc(pointer data, size_type n) const;

    constexpr pointer do_realloc(size_type req, true_type);
    constexpr pointer do_realloc(size_type req, false_type);

    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, true_type);
    constexpr bool do_expand_or_dealloc_and_alloc_raw(size_type req, false_type);

public:
    UWR_FORCEINLINE constexpr size_type npages(size_type x) { return x * sizeof(T) / page_size; }
};

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
    if (this->is_big(n)) {
        return ((n * sizeof(T) + page_size - 1) / page_size)
            * page_size / sizeof(T);
    }
    else {
        return n;
        // return std::max((64 + sizeof(T) - 1) / sizeof(T), n);
    }
}

template<class T>
constexpr int
hybrid_allocator<T>::is_big(size_type n) const {
    return n * sizeof(T) > page_size;
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
hybrid_allocator<T>::dealloc(pointer data, size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));

    if (this->is_big(n))
        this->big_dealloc(data, n);
    else
        this->small_dealloc(data, n);
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

template<class T>
constexpr void
hybrid_allocator<T>::realloc(size_type req) {
    req = this->fix_capacity(req);

    // TODO: remove
    #ifdef UWR_TRACK
    if (this->m_capacity)
        counters::grows(double(req) / this->m_capacity);
    #endif

    if (UWR_LIKELY(!!this->m_data)) {
        this->m_data = this->do_realloc(req,
            std::is_trivially_move_constructible<T>());
    }
    else {
        this->m_data = this->alloc(req);
    }

    this->m_capacity = req;
}

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
            umove_and_destroy(new_data, this->m_data, this->m_size);
            this->small_dealloc(this->m_data, this->m_capacity);

            return new_data;
        }
        case 0b11: { /* both are big sizes */
// TODO: remove
#ifdef UWR_TRACK
            pointer ret = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T),
                    MREMAP_MAYMOVE);
            counters::mremaps(this->m_data == ret);
            return ret;
#else
            return (pointer)mremap(
                    this->m_data,
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
constexpr typename hybrid_allocator<T>::pointer
hybrid_allocator<T>::do_realloc(size_type req, false_type) {
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
            counters::mremaps(this->m_data == new_data);
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
constexpr bool
hybrid_allocator<T>::expand_or_dealloc_and_alloc_raw(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    req = this->fix_capacity(req);

// TODO: remove
#ifdef UWR_TRACK
    if (this->m_capacity)
        counters::grows(double(req) / this->m_capacity);
#endif

    if (UWR_LIKELY(!!this->m_data)) {
        return this->do_expand_or_dealloc_and_alloc_raw(req,
                std::is_trivially_move_constructible<T>());
    }
    else {
        this->m_data = this->alloc(req);
        this->m_capacity = req;

        return false;
    }
}

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
            counters::mremaps(save == this->m_data);
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
            pointer new_data = (pointer)mremap(
                    this->m_data,
                    this->m_capacity * sizeof(T),
                    req * sizeof(T), 0);

// TODO: remove
#ifdef UWR_TRACK
            counters::mremaps(this->m_data == (pointer)new_data);
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
