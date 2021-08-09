#pragma once

#include "memory.hpp"
#include "allocator_base.hpp"

namespace uwr::mem {

template<class T>
class std_allocator : public allocator_base<std_allocator<T>, T> {
public:
    using base_t = allocator_base<std_allocator<T>, T>;
    using size_type = typename base_t::size_type;
    using value_type = T;
    using pointer = typename base_t::pointer;

    constexpr std_allocator() noexcept;
    constexpr explicit std_allocator(size_type n);

    UWR_FORCEINLINE constexpr size_type fix_capacity(size_type n) const;

    UWR_FORCEINLINE constexpr pointer alloc(size_type n) const;
    UWR_FORCEINLINE constexpr void dealloc(pointer data, size_type n) const;
    constexpr void realloc(size_type req);

    constexpr bool expand_or_alloc_raw(size_type req, pointer& out_ptr);
    constexpr bool expand_or_dealloc_and_alloc_raw(size_type req);
};

template<class T>
constexpr
std_allocator<T>::std_allocator() noexcept
    : base_t() {}

template<class T>
constexpr
std_allocator<T>::std_allocator(size_type n)
    : base_t(n) {}

template<class T>
constexpr typename std_allocator<T>::pointer
std_allocator<T>::alloc(size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));
    return new T[n];
}

template<class T>
constexpr void
std_allocator<T>::dealloc(pointer data, UWR_UNUSED size_type n) const {
    UWR_ASSERT(n == this->fix_capacity(n));
    delete[] data;
}

template<class T>
constexpr void
std_allocator<T>::realloc(size_type req) {
    req = this->fix_capacity(req);

    pointer new_data = this->alloc(req);
    umove(new_data, this->m_data, this->m_size);
    destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data, this->m_capacity);

    this->m_data = new_data;
    this->m_capacity = req;
}

template<class T>
constexpr bool
std_allocator<T>::expand_or_alloc_raw(size_type req, pointer& out_ptr) {
    UWR_ASSERT(req > this->m_capacity);
    UWR_ASSERT(req == this->fix_capacity(req));

    out_ptr = this->alloc(req);

    return false;
}

template<class T>
constexpr bool
std_allocator<T>::expand_or_dealloc_and_alloc_raw(size_type req) {
    UWR_ASSERT(req > this->m_capacity);

    destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data, this->m_capacity);

    this->m_capacity = this->fix_capacity(req);
    this->m_data = this->alloc(this->m_capacity);

    return false;
}

template<class T>
constexpr typename std_allocator<T>::size_type
std_allocator<T>::fix_capacity(size_type n) const {
    return std::max((64 + sizeof(T) - 1) / sizeof(T), n);
}

} // namespace uwr::mem
