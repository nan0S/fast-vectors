#pragma once

#include <iterator>

#include "../common/memory.hpp"
#include "../common/define.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif

namespace uwr {

using len_t = mem::len_t;

template<class T, len_t C, class size_t=len_t>
class static_vector_alt {
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr static_vector_alt() noexcept;
    constexpr explicit static_vector_alt(size_type n);
    constexpr static_vector_alt(size_type n, const T& val);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr static_vector_alt(InputIterator first, InputIterator last);
    constexpr static_vector_alt(const static_vector_alt& x);
    constexpr static_vector_alt(static_vector_alt&& x) noexcept;
    constexpr static_vector_alt(std::initializer_list<T> ilist);

    #if CPP_ABOVE_17
    constexpr
    #endif
    ~static_vector_alt();

    constexpr static_vector_alt& operator=(const static_vector_alt& other) noexcept;
    constexpr static_vector_alt& operator=(static_vector_alt&& other) noexcept;
    UWR_FORCEINLINE constexpr static_vector_alt& operator=(std::initializer_list<T> ilist) noexcept;

    UWR_FORCEINLINE constexpr iterator begin() noexcept;
    UWR_FORCEINLINE constexpr const_iterator begin() const noexcept;
    UWR_FORCEINLINE constexpr iterator end() noexcept;
    UWR_FORCEINLINE constexpr const_iterator end() const noexcept;

    UWR_FORCEINLINE constexpr reverse_iterator rbegin() noexcept;
    UWR_FORCEINLINE constexpr const_reverse_iterator rbegin() const noexcept;
    UWR_FORCEINLINE constexpr reverse_iterator rend() noexcept;
    UWR_FORCEINLINE constexpr const_reverse_iterator rend() const noexcept;

    UWR_FORCEINLINE constexpr const_iterator cbegin() const noexcept;
    UWR_FORCEINLINE constexpr const_iterator cend() const noexcept;
    UWR_FORCEINLINE constexpr const_reverse_iterator crbegin() const noexcept;
    UWR_FORCEINLINE constexpr const_reverse_iterator crend() const noexcept;

    UWR_FORCEINLINE constexpr size_type size() const noexcept;
    UWR_FORCEINLINE constexpr size_type max_size() const noexcept;
    constexpr void resize(size_type n);
    constexpr void resize(size_type n, const T& val);
    UWR_FORCEINLINE constexpr size_type capacity() const noexcept;
    [[nodiscard]] UWR_FORCEINLINE constexpr bool empty() const noexcept;
    UWR_FORCEINLINE constexpr void reserve(size_type n) noexcept;
    UWR_FORCEINLINE constexpr void shrink_to_fit() noexcept;

    UWR_FORCEINLINE constexpr reference operator[](size_type n);
    UWR_FORCEINLINE constexpr const_reference operator[](size_type n) const;
    UWR_FORCEINLINE constexpr reference at(size_type n);
    UWR_FORCEINLINE constexpr const_reference at(size_type n) const;
    UWR_FORCEINLINE constexpr reference front();
    UWR_FORCEINLINE constexpr const_reference front() const;
    UWR_FORCEINLINE constexpr reference back();
    UWR_FORCEINLINE constexpr const_reference back() const;
    UWR_FORCEINLINE constexpr T* data() noexcept;
    UWR_FORCEINLINE constexpr const T* data() const noexcept;

    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr void assign(InputIterator first, InputIterator last);
    constexpr void assign(size_type n, const T& val);
    UWR_FORCEINLINE constexpr void assign(std::initializer_list<T> ilist);

    UWR_FORCEINLINE constexpr void push_back(const T& value);
    UWR_FORCEINLINE constexpr void push_back(T&& value);
    UWR_FORCEINLINE constexpr void fast_push_back(T&& value) noexcept;

    UWR_FORCEINLINE constexpr void pop_back();
    UWR_FORCEINLINE constexpr bool safe_pop_back() noexcept;

    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, const T& value);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, T&& value);
    constexpr iterator insert(const_iterator pos, size_type count, const T& value);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator first, const_iterator last);

    UWR_FORCEINLINE constexpr void swap(static_vector_alt& other);
    UWR_FORCEINLINE constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference emplace_back(Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference fast_emplace_back(Args&&... args) noexcept;

private:
    UWR_FORCEINLINE constexpr T* data_at(size_type n) noexcept;
    UWR_FORCEINLINE constexpr const T* data_at(size_type n) const noexcept;
    constexpr void priv_swap(static_vector_alt& shorter, static_vector_alt& longer,
            size_type s_length, size_type l_length);

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
    T* m_end;
};

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt() noexcept
    : m_end(data()) {
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(size_type n)
    : m_end(data() + n) {
    mem::construct(data(), n);
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(size_type n, const T& val)
    : m_end(data() + n) {
    mem::ufill(data(), n, val);
}

template<class T, len_t C, class size_t>
template<class InputIterator, class>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(InputIterator first, InputIterator last)
    : m_end(data() + std::distance(first, last)) {
    mem::ucopy(data(), first, last);
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(const static_vector_alt& x)
    : m_end(data() + x.size()) {
    mem::ucopy(data(), x.begin(), x.end());
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(static_vector_alt&& x) noexcept
    : m_end(data() + x.size()) {
    mem::umove(data(), x.begin(), x.end());
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(std::initializer_list<T> ilist)
    : m_end(data() + ilist.size()) {
    mem::ucopy(data(), ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
#if CPP_ABOVE_17
constexpr
#endif
static_vector_alt<T, C, size_t>::~static_vector_alt() {
    mem::destroy(data(), m_end);
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(const static_vector_alt<T, C, size_t>& other) noexcept {
    T* ptr = data();
    const T* optr = other.data();
    size_type len = size();
    size_type olen = other.size();

    if (olen < len) {
        mem::destroy(ptr + olen, m_end);
        mem::copy(ptr, optr, olen);
    }
    else {
        mem::copy(ptr, optr, len);
        mem::ucopy<T, const T*>(m_end, optr + len, other.m_end);
    }

    m_end = ptr + olen;

    return *this;
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(static_vector_alt<T, C, size_t>&& other) noexcept {
    T* ptr = data();
    T* optr = other.data();
    size_type len = size();
    size_type olen = other.size();

    if (olen < len) {
        mem::destroy(ptr + olen, m_end);
        mem::move(ptr, optr, olen);
    }
    else {
        mem::move(ptr, optr, len);
        mem::umove(m_end, optr + len, other.m_end);
    }

    m_end = ptr + olen;

    return *this;
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(std::initializer_list<T> ilist) noexcept {
    assign(ilist.begin(), ilist.end());

    return *this;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::begin() noexcept {
    return data();
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::begin() const noexcept {
    return data();
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::end() noexcept {
    return m_end;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::end() const noexcept {
    return m_end;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reverse_iterator
static_vector_alt<T, C, size_t>::rbegin() noexcept {
    return reverse_iterator(m_end);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reverse_iterator
static_vector_alt<T, C, size_t>::rbegin() const noexcept {
    return const_reverse_iterator(m_end);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reverse_iterator
static_vector_alt<T, C, size_t>::rend() noexcept {
    return reverse_iterator(data());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reverse_iterator
static_vector_alt<T, C, size_t>::rend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::cbegin() const noexcept {
    return data();
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_iterator
constexpr static_vector_alt<T, C, size_t>::cend() const noexcept {
    return m_end;
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reverse_iterator
constexpr static_vector_alt<T, C, size_t>::crbegin() const noexcept {
    return const_reverse_iterator(m_end);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reverse_iterator
constexpr static_vector_alt<T, C, size_t>::crend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::size() const noexcept {
    return static_cast<size_type>(m_end - data());
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::max_size() const noexcept {
    return C;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::resize(size_type n) {
    T* ptr = data();
    size_type len = size();

    if (n > len)
        mem::construct(m_end, ptr + n);
    else
        mem::destroy(ptr + n, m_end);

    m_end = ptr + n;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::resize(size_type n, const T& val) {
    T* ptr = data();
    size_type len = size();

    if (n > len)
        mem::ufill(m_end, ptr + n, val);
    else
        mem::destroy(ptr + n, m_end);

    m_end = ptr + n;
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::capacity() const noexcept {
    return C;
}

template<class T, len_t C, class size_t>
constexpr bool
static_vector_alt<T, C, size_t>::empty() const noexcept {
    return data() == m_end;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::reserve(size_type) noexcept {
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::shrink_to_fit() noexcept {
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::operator[](size_type n) {
    return *data_at(n);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reference
constexpr static_vector_alt<T, C, size_t>::operator[](size_type n) const {
    return *data_at(n);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::at(size_type n) {
    if (data() + n >= m_end)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reference
static_vector_alt<T, C, size_t>::at(size_type n) const {
    if (data() + n >= m_end)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::front() {
    return *data_at(0);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reference
static_vector_alt<T, C, size_t>::front() const {
    return *data_at(0);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::reference
constexpr static_vector_alt<T, C, size_t>::back() {
    return *(m_end - 1);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reference
constexpr static_vector_alt<T, C, size_t>::back() const {
    return *(m_end - 1);
}

template<class T, len_t C, class size_t>
constexpr T*
static_vector_alt<T, C, size_t>::data() noexcept {
    return data_at(0);
}

template<class T, len_t C, class size_t>
constexpr const T*
static_vector_alt<T, C, size_t>::data() const noexcept {
    return data_at(0);
}

template<class T, len_t C, class size_t>
template<class InputIterator, class>
constexpr void
static_vector_alt<T, C, size_t>::assign(InputIterator first, InputIterator last) {
    T* ptr = data();
    size_type len = size();
    size_type n = static_cast<size_type>(std::distance(first, last));

    if (n < len) {
        mem::destroy(ptr + n, m_end);
        mem::copy(ptr, first, n);
    }
    else {
        mem::copy(ptr, first, len);
        mem::ucopy(m_end, first + len, last);
    }

    m_end = ptr + n;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::assign(size_type n, const T& val) {
    T* ptr = data();
    size_type len = size();
    
    if (n < len) {
        mem::destroy(ptr + n, m_end);
        mem::fill(ptr, n, val);
    }
    else {
        mem::fill(ptr, len, val);
        mem::ufill(m_end, n - len, val);
    }

    m_end = ptr + n;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::push_back(const_reference value) {
    emplace_back(value);
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::push_back(T&& value) {
    emplace_back(std::move(value));
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::fast_push_back(T&& value) noexcept {
    fast_emplace_back(std::forward<T>(value));
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::pop_back() {
    mem::destroy_at(--m_end);
}

template<class T, len_t C, class size_t>
constexpr bool
static_vector_alt<T, C, size_t>::safe_pop_back() noexcept {
    if (UNLIKELY(m_end == data()))
        return false;
    pop_back();
    return true;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, const T& value) {
    return emplace(pos, value);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, T&& value) {
    return emplace(pos, std::move(value));
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, size_type count, const T& value) {
    T* position = const_cast<T*>(pos);

    if (UNLIKELY(!count))
        return position;

    size_type rest = static_cast<size_type>(std::distance(position, m_end));

    if (count < rest) {
        mem::shiftr(position + count, position, m_end);
        mem::fill(position, count, value);
    }
    else {
        mem::umove(position + count, position, m_end);
        mem::fill(position, rest, value);
        mem::ufill(m_end, count - rest, value);
    }

    m_end += count;

    return position;
}

template<class T, len_t C, class size_t>
template<class InputIterator, class>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    T* position = const_cast<T*>(pos);

    if (UNLIKELY(first == last))
        return position;

    size_type count = static_cast<size_type>(std::distance(first, last));
    size_type rest = static_cast<size_type>(std::distance(position, m_end));

    if (count < rest) {
        mem::shiftr(position + count, position, m_end);
        mem::copy(position, first, count);
    }
    else {
        mem::umove(position + count, position, m_end);
        mem::copy(position, first, rest);
        mem::ucopy(m_end, first + rest, last);
    }

    m_end += count;

    return position;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::erase(const_iterator pos) {
    T* position = const_cast<T*>(pos);
    mem::shiftl(position, position + 1, m_end);
    pop_back();

    return position;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::erase(const_iterator first, const_iterator last) {
    T* f = const_cast<T*>(first);

    if (LIKELY(first != last)) {
        T* l = const_cast<T*>(last);
        mem::shiftl(f, l, m_end);
        mem::destroy(f + (m_end - l), m_end);
        m_end -= l - f;
    }

    return f;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::swap(static_vector_alt<T, C, size_t>& other) {
    size_type m_length = size();
    size_type o_length = other.size();

    if (m_length < o_length)
        priv_swap(*this, other, m_length, o_length);
    else
        priv_swap(other, *this, o_length, m_length);
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::priv_swap(
        static_vector_alt<T, C, size_t>& shorter,
        static_vector_alt<T, C, size_t>& longer,
        size_type s_length,
        size_type l_length) {
    T* const s_begin = shorter.begin();
    T* const l_begin = longer.begin();

    std::swap_ranges(s_begin, shorter.m_end, l_begin);
    mem::umove(shorter.m_end, l_begin + s_length, longer.m_end);
    mem::destroy(l_begin + s_length, longer.m_end);
    shorter.m_end = s_begin + l_length;
    longer.m_end = l_begin + s_length;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::clear() noexcept {
    T* const bptr = data();
    mem::destroy(bptr, m_end);
    m_end = bptr;
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::emplace(const_iterator pos, Args&&... args) {
    T* position = const_cast<T*>(pos);

    if (position == m_end)
        new (position) T(std::forward<Args>(args)...);
    else {
        mem::shiftr(position + 1, position, m_end);
        // this strange construction is caused by the fact
        // args can be either "proper" constructor arguments
        // or it can be an object of type T, if it is, we don't want to do
        // *position = T(args), because that would create unnecessary object,
        // instead we would like to write *position = args
        // note: this problem only happens when we use opeartor= (so we copy
        // into initialized memory), when we copy into unitialized memory
        // we have to call constructor in both situation so
        // T(std::forward<Args>(args)...) will do
        *position = mem::create<T>(std::forward<Args>(args)...);
    }
    ++m_end;

    return position;
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::emplace_back(Args&&... args) {
    if (UNLIKELY(m_end == data() + C))
        throw std::out_of_range("Out of bounds");
    return fast_emplace_back(std::forward<Args>(args)...);
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::fast_emplace_back(Args&&... args) noexcept {
    new (m_end) T(std::forward<Args>(args)...);
    return *m_end++;
}

template<class T, len_t C, class size_t>
constexpr T*
static_vector_alt<T, C, size_t>::data_at(size_type n) noexcept {
    return reinterpret_cast<T*>(&m_data[n]);
}

template<class T, len_t C, class size_t>
constexpr const T*
static_vector_alt<T, C, size_t>::data_at(size_type n) const noexcept {
    return reinterpret_cast<const T*>(&m_data[n]);
}

/*
 * non-member operators 
 */
template<class T, len_t C, class size_t>
constexpr inline bool operator==(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr inline bool operator!=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr inline bool operator<(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr inline bool operator<=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr inline bool operator>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr inline bool operator>=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector_alt<T, C, size_t>& v);

#if CPP_ABOVE_17
template<class T, len_t C, class size_t>
constexpr inline auto operator<=>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
#endif

/*
 * non-member operators' implementations
 */
template<class T, len_t C, class size_t>
constexpr bool
operator==(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    if (lhs.size() != rhs.size())
        return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, len_t C, class size_t>
constexpr bool
operator!=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return !(lhs == rhs);
}

template<class T, len_t C, class size_t>
constexpr bool
operator<(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, len_t C, class size_t>
constexpr bool
operator<=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return !(rhs < lhs);
}

template<class T, len_t C, class size_t>
constexpr bool
operator>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return rhs < lhs;
}

template<class T, len_t C, class size_t>
constexpr bool
operator>=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return !(lhs < rhs);
}

template<class T, len_t C, class size_t>
constexpr std::ostream&
operator<<(std::ostream& out, const static_vector_alt<T, C, size_t>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i)
        out << v[i] << ' ';
    if (!v.empty())
        out << v.back();
    return out;
}

#if CPP_ABOVE_17

template<class T, len_t C, class size_t>
constexpr auto
operator<=>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                  rhs.begin(), rhs.end(),
                                                  mem::synth_three_way);
}

#endif

} // namespace uwr


/*
* non-member functions
*/
namespace std {

template<class T, uwr::len_t C>
constexpr void swap(uwr::static_vector_alt<T, C, size_t>& x, uwr::static_vector_alt<T, C, size_t>& y);

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector_alt<T, C, size_t>::size_type
erase(uwr::static_vector_alt<T, C, size_t>& c, const U& value);

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector_alt<T, C, size_t>::size_type
erase_if(uwr::static_vector_alt<T, C, size_t>& c, Pred pred);

#endif

/*
 * non-member functions' implementations
 */
template<class T, uwr::len_t C>
constexpr void
swap(uwr::static_vector_alt<T, C, size_t>& x, uwr::static_vector_alt<T, C, size_t>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector_alt<T, C, size_t>::size_type
erase(uwr::static_vector_alt<T, C, size_t>& c, const U& value) {
    using size_type = typename uwr::static_vector_alt<T, C, size_t>::size_type;

    T* const cend = c.end();
    T* const it = std::remove(c.begin(), cend, value);
    c.erase(it, cend);

    return static_cast<size_type>(std::distance(it, cend));
}

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector_alt<T, C, size_t>::size_type
erase_if(uwr::static_vector_alt<T, C, size_t>& c, Pred pred) {
    using size_type = typename uwr::static_vector_alt<T, C, size_t>::size_type;

    T* const cend = c.end();
    T* const it = std::remove_if(c.begin(), cend, pred);
    c.erase(it, cend);

    return static_cast<size_type>(std::distance(it, cend));
}

#endif // CPP_ABOVE_17

} // namespace std
