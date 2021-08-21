#pragma once

#include "../common/memory.hpp"
#include "allocator_base.hpp"

namespace uwr::mem {

using std::true_type;
using std::false_type;

template<class T>
class malloc_allocator : public allocator_base<malloc_allocator<T>, T> {
public:
    using base_t = allocator_base<malloc_allocator<T>, T>;
    using size_type = typename base_t::size_type;
    using value_type = T;
    using pointer = typename base_t::pointer;

    constexpr malloc_allocator() noexcept;
    constexpr explicit malloc_allocator(size_type n);

    UWR_FORCEINLINE constexpr size_type fix_capacity(size_type n) const;

    UWR_FORCEINLINE constexpr pointer alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void dealloc(pointer data, size_type n) const;
    UWR_FORCEINLINE constexpr void realloc(size_type req);

    constexpr bool expand_or_alloc_raw(size_type req, pointer& out_ptr);
    constexpr bool expand_or_dealloc_and_alloc_raw(size_type req);

private:
    UWR_FORCEINLINE constexpr T* do_realloc(size_type req, true_type);
    constexpr T* do_realloc(size_type req, false_type);
};

template<class T>
constexpr
malloc_allocator<T>::malloc_allocator() noexcept
    : base_t() {}

template<class T>
constexpr
malloc_allocator<T>::malloc_allocator(size_type n)
    : base_t(n) {}

template<class T>
constexpr typename malloc_allocator<T>::size_type
malloc_allocator<T>::fix_capacity(size_type n) const {
#ifndef NDEBUG
    if (!n) return 0;
#endif
    return n;
    // return std::max((64 + sizeof(T) - 1) / sizeof(T), n);
}

template<class T>
constexpr typename malloc_allocator<T>::pointer
malloc_allocator<T>::alloc(size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));
    return static_cast<T*>(malloc(n * sizeof(T)));
}

template<class T>
constexpr void
malloc_allocator<T>::dealloc(pointer data, UWR_UNUSED size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));
    free(data);
}

template<class T>
constexpr void
malloc_allocator<T>::realloc(size_type req) {
    req = this->fix_capacity(req);
    this->m_data = this->do_realloc(req,
        std::is_trivially_move_constructible<T>());
    this->m_capacity = req;
}

template<class T>
constexpr bool
malloc_allocator<T>::expand_or_alloc_raw(size_type req, pointer& out_ptr) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    out_ptr = this->alloc(req);

    return false;
}

template<class T>
constexpr bool
malloc_allocator<T>::expand_or_dealloc_and_alloc_raw(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data, this->m_capacity);

    this->m_capacity = this->fix_capacity(req);
    this->m_data = this->alloc(this->m_capacity);

    return false;
}

template<class T>
constexpr T*
malloc_allocator<T>::do_realloc(size_type req, true_type) {
    return (T*)::realloc(this->m_data, req * sizeof(T));
}

template<class T>
constexpr T*
malloc_allocator<T>::do_realloc(size_type req, false_type) {
    pointer new_data = this->alloc(req);
    umove_and_destroy(new_data, this->m_data, this->m_size);
    this->dealloc(this->m_data, this->m_capacity);

    return new_data;
}

} // namespace uwr::mem
