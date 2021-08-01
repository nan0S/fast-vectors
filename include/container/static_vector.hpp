#pragma once

#include <iterator>

#include "../common/memory.hpp"
#include "../common/define.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif

namespace uwr {

using len_t = mem::len_t;
// TODO: restore to uint
static_assert(std::is_same_v<len_t, std::size_t>,
    "static_vector template type has to be the same as len_t!");

/* template type is uint insted of len_t clarity reasons
   (not to lookup what len_t is) */
// TODO: restore to uint
template<class T, std::size_t C>
class static_vector {
public:
    using value_type = T;
    // TODO: restore to uint...
    // using size_type = uint_fast32_t;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr static_vector() noexcept;
    constexpr explicit static_vector(size_type n);
    constexpr static_vector(size_type n, const T& val);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr static_vector(InputIterator first, InputIterator last);
    constexpr static_vector(const static_vector& x);
    constexpr static_vector(static_vector&& x) noexcept;
    constexpr static_vector(std::initializer_list<T> ilist);

    #if CPP_ABOVE_17
    constexpr
    #endif
    ~static_vector();

    constexpr static_vector& operator=(const static_vector& other) noexcept;
    constexpr static_vector& operator=(static_vector&& other) noexcept;
    UWR_FORCEINLINE constexpr static_vector& operator=(std::initializer_list<T> ilist) noexcept;

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
    constexpr void resize1(size_type n);
    constexpr void resize2(size_type n);
    constexpr void resize1(size_type n, const T& val);
    constexpr void resize2(size_type n, const T& val);
    UWR_FORCEINLINE constexpr size_type capacity() const noexcept;
    [[nodiscard]] UWR_FORCEINLINE constexpr bool empty() const noexcept;
    UWR_FORCEINLINE constexpr void reserve(size_type n) noexcept;
    UWR_FORCEINLINE constexpr void shrink_to_fit() noexcept;

    UWR_FORCEINLINE constexpr reference operator[](size_type n);
    UWR_FORCEINLINE constexpr const_reference operator[](size_type n) const;
    UWR_FORCEINLINE constexpr reference at1(size_type n);
    UWR_FORCEINLINE constexpr reference at2(size_type n);
    UWR_FORCEINLINE constexpr const_reference at(size_type n) const;
    UWR_FORCEINLINE constexpr reference front();
    UWR_FORCEINLINE constexpr const_reference front() const;
    UWR_FORCEINLINE constexpr reference back();
    UWR_FORCEINLINE constexpr const_reference back() const;
    UWR_FORCEINLINE constexpr T* data() noexcept;
    UWR_FORCEINLINE constexpr const T* data() const noexcept;

    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr void assign(InputIterator first, InputIterator last);
    constexpr void assign1(size_type n, const T& val);
    constexpr void assign2(size_type n, const T& val);
    UWR_FORCEINLINE constexpr void assign(std::initializer_list<T> ilist);

    UWR_FORCEINLINE constexpr void push_back(const T& value);
    UWR_FORCEINLINE constexpr void push_back(T&& value);
    UWR_FORCEINLINE constexpr void fast_push_back(T&& value) noexcept;

    UWR_FORCEINLINE constexpr void pop_back();
    UWR_FORCEINLINE constexpr bool safe_pop_back1() noexcept;
    UWR_FORCEINLINE constexpr bool safe_pop_back2() noexcept;

    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, const T& value);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, T&& value);
    constexpr iterator insert1(const_iterator pos, size_type count, const T& value);
    constexpr iterator insert2(const_iterator pos, size_type count, const T& value);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator first, const_iterator last);
    constexpr iterator erase1(const_iterator first, const_iterator last);
    constexpr iterator erase2(const_iterator first, const_iterator last);

    UWR_FORCEINLINE constexpr void swap(static_vector& other);
    UWR_FORCEINLINE constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference emplace_back1(Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference emplace_back2(Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference fast_emplace_back(Args&&... args) noexcept;

private:
    UWR_FORCEINLINE constexpr T* data_at(size_type n) noexcept;
    UWR_FORCEINLINE constexpr const T* data_at(size_type n) const noexcept;
    constexpr void priv_swap(static_vector& shorter, static_vector& longer);

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
    size_type m_length;
};

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector() noexcept
    : m_length(0) {
}

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector(size_type n)
    : m_length(n) {
    mem::construct(data(), n);
}

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector(size_type n, const T& val)
    : m_length(n) {
    mem::ufill(data(), n, val);
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr
static_vector<T, C>::static_vector(InputIterator first, InputIterator last)
    : m_length(std::distance(first, last)) {
    mem::ucopy(data(), first, last);
}

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector(const static_vector& x)
    : m_length(x.m_length) {
    mem::ucopy(data(), x.begin(), x.end());
}

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector(static_vector&& x) noexcept
    : m_length(x.m_length) {
    mem::umove(data(), x.begin(), x.end());
}

template<class T, len_t C>
constexpr
static_vector<T, C>::static_vector(std::initializer_list<T> ilist)
    : m_length(ilist.size()) {
    mem::ucopy(data(), ilist.begin(), ilist.end());
}

template<class T, len_t C>
#if CPP_ABOVE_17
constexpr
#endif
static_vector<T, C>::~static_vector() {
    mem::destroy(data(), m_length);
}

template<class T, len_t C>
constexpr static_vector<T, C>&
static_vector<T, C>::operator=(const static_vector<T, C>& other) noexcept {
    T* ptr = data();
    const T* optr = other.data();

    if (other.m_length < m_length) {
        mem::destroy(ptr + other.m_length, ptr + m_length);
        mem::copy(ptr, optr, other.m_length);
    }
    else {
        mem::copy(ptr, optr, m_length);
        mem::ucopy(ptr + m_length, optr + m_length, optr + other.m_length);
    }

    m_length = other.m_length;

    return *this;
}

template<class T, len_t C>
constexpr static_vector<T, C>&
static_vector<T, C>::operator=(static_vector<T, C>&& other) noexcept {
    T* ptr = data();
    T* optr = other.data();

    if (other.m_length < m_length) {
        mem::destroy(ptr + other.m_length, ptr + m_length);
        mem::move(ptr, optr, other.m_length);
    }
    else {
        mem::move(ptr, optr, m_length);
        mem::umove(ptr + m_length, optr + m_length, optr + other.m_length);
    }

    m_length = other.m_length;

    return *this;
}

template<class T, len_t C>
constexpr static_vector<T, C>&
static_vector<T, C>::operator=(std::initializer_list<T> ilist) noexcept {
    assign(ilist.begin(), ilist.end());

    return *this;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::begin() noexcept {
    return data();
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_iterator
static_vector<T, C>::begin() const noexcept {
    return data();
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::end() noexcept {
    return data() + m_length;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_iterator
static_vector<T, C>::end() const noexcept {
    return data() + m_length;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::reverse_iterator
static_vector<T, C>::rbegin() noexcept {
    return reverse_iterator(data() + m_length);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_reverse_iterator
static_vector<T, C>::rbegin() const noexcept {
    return const_reverse_iterator(data() + m_length);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::reverse_iterator
static_vector<T, C>::rend() noexcept {
    return reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_reverse_iterator
static_vector<T, C>::rend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_iterator
static_vector<T, C>::cbegin() const noexcept {
    return data();
}

template<class T, len_t C>
typename static_vector<T, C>::const_iterator
constexpr static_vector<T, C>::cend() const noexcept {
    return data() + m_length;
}

template<class T, len_t C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::crbegin() const noexcept {
    return const_reverse_iterator(data() + m_length);
}

template<class T, len_t C>
typename static_vector<T, C>::const_reverse_iterator
constexpr static_vector<T, C>::crend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::size_type
static_vector<T, C>::size() const noexcept {
    return m_length;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::size_type
static_vector<T, C>::max_size() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::resize1(size_type n) {
    if (m_length < n) {
        T* ptr = data();
        mem::construct(ptr + m_length, ptr + n);
        m_length = n;
    }
    // TODO: remove second check (?)
    else if (n < m_length) {
        T* ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        m_length = n;
    }
}

// MARK: better
template<class T, len_t C>
constexpr void
static_vector<T, C>::resize2(size_type n) {
    if (m_length < n) {
        T* ptr = data();
        mem::construct(ptr + m_length, ptr + n);
        m_length = n;
    }
    else {
        T* ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        m_length = n;
    }
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::resize1(size_type n, const T& val) {
    if (m_length < n) {
        T* ptr = data();
        mem::ufill(ptr + m_length, ptr + n, val);
        m_length = n;
    }
    // TODO: remove second check (?)
    else if (n < m_length) {
        T* ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        m_length = n;
    }
}

// MARK: better
template<class T, len_t C>
constexpr void
static_vector<T, C>::resize2(size_type n, const T& val) {
    if (m_length < n) {
        T* ptr = data();
        mem::ufill(ptr + m_length, ptr + n, val);
        m_length = n;
    }
    // TODO: remove second check (?)
    else {
        T* ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        m_length = n;
    }
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::size_type
static_vector<T, C>::capacity() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr bool
static_vector<T, C>::empty() const noexcept {
    return m_length == 0;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::reserve(size_type) noexcept {
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::shrink_to_fit() noexcept {
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::operator[](size_type n) {
    return *data_at(n);
}

template<class T, len_t C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::operator[](size_type n) const {
    return *data_at(n);
}

// MARK: better
template<class T, len_t C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::at1(size_type n) {
    // TODO: unlikely (?)
    if (n >= m_length)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::at2(size_type n) {
    // TODO: unlikely (?)
    if (UNLIKELY(n >= m_length))
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_reference
static_vector<T, C>::at(size_type n) const {
    // TODO: unlikely (?) - should not
    if (n >= m_length)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::front() {
    return *data_at(0);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_reference
static_vector<T, C>::front() const {
    return *data_at(0);
}

template<class T, len_t C>
typename static_vector<T, C>::reference
constexpr static_vector<T, C>::back() {
    return *data_at(m_length - 1);
}

template<class T, len_t C>
typename static_vector<T, C>::const_reference
constexpr static_vector<T, C>::back() const {
    return *data_at(m_length - 1);
}

template<class T, len_t C>
constexpr T*
static_vector<T, C>::data() noexcept {
    return data_at(0);
}

template<class T, len_t C>
constexpr const T*
static_vector<T, C>::data() const noexcept {
    return data_at(0);
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr void
static_vector<T, C>::assign(InputIterator first, InputIterator last) {
    T* ptr = data();
    size_type n = static_cast<size_type>(std::distance(first, last));

    if (n < m_length) {
        mem::destroy(ptr + n, ptr + m_length);
        mem::copy(ptr, first, n);
    }
    // TODO: maybe add if (?) - maybe not :)
    else {
        mem::copy(ptr, first, m_length);
        mem::ucopy(ptr + m_length, first + m_length, last);
    }

    m_length = n;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::assign1(size_type n, const T& val) {
    
    if (n < m_length) {
        T* ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        mem::fill(ptr, n, val);
        m_length = n;
    }
    // TODO: maybe add if (?)
    else if (m_length < n) {
        T* ptr = data();
        mem::fill(ptr, m_length, val);
        mem::ufill(ptr + m_length, n - m_length, val);
        m_length = n;
    }

}

// MARK: better
template<class T, len_t C>
constexpr void
static_vector<T, C>::assign2(size_type n, const T& val) {
    T* ptr = data();
    
    if (n < m_length) {
        mem::destroy(ptr + n, ptr + m_length);
        mem::fill(ptr, n, val);
    }
    // TODO: maybe add if (?)
    else {
        mem::fill(ptr, m_length, val);
        mem::ufill(ptr + m_length, n - m_length, val);
    }

    m_length = n;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::push_back(const_reference value) {
    emplace_back(value);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::push_back(T&& value) {
    emplace_back(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::fast_push_back(T&& value) noexcept {
    fast_emplace_back(std::forward<T>(value));
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::pop_back() {
    mem::destroy_at(data() + --m_length);
}

template<class T, len_t C>
constexpr bool
static_vector<T, C>::safe_pop_back1() noexcept {
    // TODO: unlikely (?)
    if (m_length == 0)
        return false;
    pop_back();
    return true;
}

// MARK: better
template<class T, len_t C>
constexpr bool
static_vector<T, C>::safe_pop_back2() noexcept {
    // TODO: unlikely (?)
    if (UNLIKELY(m_length == 0))
        return false;
    pop_back();
    return true;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, const T& value) {
    return emplace(pos, value);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, T&& value) {
    return emplace(pos, std::move(value));
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert1(const_iterator pos, size_type count, const T& value) {
    T* position = const_cast<T*>(pos);

    // TODO: unlikely or can do better (?)
    if (!count)
        return position;

    T* eptr = end();
    size_type rest = static_cast<size_type>(std::distance(position, eptr));

    if (count < rest) {
        mem::shiftr(position + count, position, eptr);
        mem::fill(position, count, value);
    }
    else {
        mem::umove(position + count, position, eptr);
        mem::fill(position, rest, value);
        mem::ufill(eptr, count - rest, value);
    }

    m_length += count;

    return position;
}

// MARK: better
template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert2(const_iterator pos, size_type count, const T& value) {
    T* position = const_cast<T*>(pos);

    // TODO: unlikely or can do better (?)
    if (UNLIKELY(!count))
        return position;

    T* eptr = end();
    size_type rest = static_cast<size_type>(std::distance(position, eptr));

    if (count < rest) {
        mem::shiftr(position + count, position, eptr);
        mem::fill(position, count, value);
    }
    else {
        mem::umove(position + count, position, eptr);
        mem::fill(position, rest, value);
        mem::ufill(eptr, count - rest, value);
    }

    m_length += count;

    return position;
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    T* position = const_cast<T*>(pos);

    // TODO: unlikely or can do better (?) - unlikely
    if (first == last)
        return position;

    T* eptr = end();
    size_type count = static_cast<size_type>(std::distance(first, last));
    size_type rest = static_cast<size_type>(std::distance(position, eptr));

    if (count < rest) {
        mem::shiftr(position + count, position, eptr);
        mem::copy(position, first, count);
    }
    else {
        mem::umove(position + count, position, eptr);
        mem::copy(position, first, rest);
        mem::ucopy(eptr, first + rest, last);
    }

    m_length += count;

    return position;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::erase(const_iterator pos) {
    T* position = const_cast<T*>(pos);
    mem::shiftl(position, position + 1, end());
    pop_back();

    return position;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::erase(const_iterator first, const_iterator last) {
    T* f = const_cast<T*>(first);

    // TODO: unlikely, maybe can do better (?)
    if (first != last) {
        T* l = const_cast<T*>(last);
        T* e = end();
        mem::shiftl(f, l, e);
        mem::destroy(f + (e - l), e);
        m_length -= l - f;
    }

    return f;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::erase1(const_iterator first, const_iterator last) {
    T* f = const_cast<T*>(first);

    // TODO: unlikely, maybe can do better (?)
    if (first != last) {
        T* l = const_cast<T*>(last);
        T* e = end();
        mem::shiftl(f, l, e);
        mem::destroy(f + (e - l), e);
        m_length -= l - f;
    }

    return f;
}

// MARK: similar/better
template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::erase2(const_iterator first, const_iterator last) {
    T* f = const_cast<T*>(first);

    // TODO: unlikely, maybe can do better (?)
    if (UNLIKELY(first != last)) {
        T* l = const_cast<T*>(last);
        T* e = end();
        mem::shiftl(f, l, e);
        mem::destroy(f + (e - l), e);
        m_length -= l - f;
    }

    return f;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::swap(static_vector<T, C>& other) {
    if (m_length < other.m_length)
        priv_swap(*this, other);
    else
        priv_swap(other, *this);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::priv_swap(static_vector<T, C>& shorter, static_vector<T, C>& longer) {
    T* const s_begin = shorter.begin();
    T* const s_end = shorter.end();
    T* const l_begin = longer.begin();
    T* const l_end = longer.end();

    std::swap_ranges(s_begin, s_end, l_begin);
    mem::umove(s_end, l_begin + shorter.m_length, l_end);
    mem::destroy(l_begin + shorter.m_length, l_end);
    std::swap(shorter.m_length, longer.m_length);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::clear() noexcept {
    mem::destroy(data(), m_length);
    m_length = 0;
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::emplace(const_iterator pos, Args&&... args) {
    T* position = const_cast<T*>(pos);
    T* eptr = end();

    if (position == eptr)
        new (position) T(std::forward<Args>(args)...);
    else {
        mem::shiftr(position + 1, position, eptr);
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
    ++m_length;

    return position;
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::emplace_back1(Args&&... args) {
    // TODO: unlikely (?)
    if (m_length == C)
        throw std::out_of_range("Out of bounds");
    return fast_emplace_back(std::forward<Args>(args)...);
}

// MARK: better
template<class T, len_t C>
template<class... Args>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::emplace_back2(Args&&... args) {
    // TODO: unlikely (?)
    if (UNLIKELY(m_length == C))
        throw std::out_of_range("Out of bounds");
    return fast_emplace_back(std::forward<Args>(args)...);
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::fast_emplace_back(Args&&... args) noexcept {
    T* eptr = data() + m_length++;
    new (eptr) T(std::forward<Args>(args)...);
    return *eptr;
}

template<class T, len_t C>
constexpr T*
static_vector<T, C>::data_at(size_type n) noexcept {
    return reinterpret_cast<T*>(&m_data[n]);
}

template<class T, len_t C>
constexpr const T*
static_vector<T, C>::data_at(size_type n) const noexcept {
    return reinterpret_cast<const T*>(&m_data[n]);
}

/*
 * non-member operators 
 */
template<class T, len_t C>
constexpr inline bool operator==(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator!=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator<(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator<=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator>=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
template<class T, len_t C>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector<T, C>& v);

#if CPP_ABOVE_17
template<class T, len_t C>
constexpr inline auto operator<=>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs);
#endif

/*
 * non-member operators' implementations
 */
template<class T, len_t C>
constexpr bool
operator==(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    if (lhs.size() != rhs.size())
        return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, len_t C>
constexpr bool
operator!=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    return !(lhs == rhs);
}

template<class T, len_t C>
constexpr bool
operator<(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, len_t C>
constexpr bool
operator<=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    return !(rhs < lhs);
}

template<class T, len_t C>
constexpr bool
operator>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    return rhs < lhs;
}

template<class T, len_t C>
constexpr bool
operator>=(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
    return !(lhs < rhs);
}

template<class T, len_t C>
constexpr std::ostream&
operator<<(std::ostream& out, const static_vector<T, C>& v) {
    for (typename static_vector<T, C>::size_type i = 0; i < v.size() - 1; ++i)
        out << v[i] << ' ';
    if (!v.empty())
        out << v.back();
    return out;
}

#if CPP_ABOVE_17

template<class T, len_t C>
constexpr auto
operator<=>(const static_vector<T, C>& lhs, const static_vector<T, C>& rhs) {
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
constexpr void swap(uwr::static_vector<T, C>& x, uwr::static_vector<T, C>& y);

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector<T, C>::size_type
erase(uwr::static_vector<T, C>& c, const U& value);

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector<T, C>::size_type
erase_if(uwr::static_vector<T, C>& c, Pred pred);

#endif

/*
 * non-member functions' implementations
 */
template<class T, uwr::len_t C>
constexpr void
swap(uwr::static_vector<T, C>& x, uwr::static_vector<T, C>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector<T, C>::size_type
erase(uwr::static_vector<T, C>& c, const U& value) {
    using size_type = typename uwr::static_vector<T, C>::size_type;

    T* const cend = c.end();
    T* const it = std::remove(c.begin(), cend, value);
    c.erase(it, cend);

    return static_cast<size_type>(std::distance(it, cend));
}

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector<T, C>::size_type
erase_if(uwr::static_vector<T, C>& c, Pred pred) {
    using size_type = typename uwr::static_vector<T, C>::size_type;

    T* const cend = c.end();
    T* const it = std::remove_if(c.begin(), cend, pred);
    c.erase(it, cend);

    return static_cast<size_type>(std::distance(it, cend));
}

#endif // CPP_ABOVE_17

} // namespace std
