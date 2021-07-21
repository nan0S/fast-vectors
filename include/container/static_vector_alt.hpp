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
    "static_vector_alt template type has to be the same as len_t!");

/* template type is uint insted of len_t clarity reasons
   (not to lookup what len_t is) */
// TODO: restore to uint
template<class T, std::size_t C>
class static_vector_alt {
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

    constexpr static_vector_alt() noexcept;
    constexpr explicit static_vector_alt(size_type n);
    constexpr static_vector_alt(size_type n, const T& val);
    template<class InputIterator,
             class = typename std::iterator_traits<InputIterator>::value_type>
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
    constexpr static_vector_alt& operator=(std::initializer_list<T> ilist) noexcept;

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

    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr void resize(size_type n);
    constexpr void resize(size_type n, const T& val);
    constexpr size_type capacity() const noexcept;
    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr void reserve(size_type n) noexcept;
    constexpr void shrink_to_fit() noexcept;

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

    template<class InputIterator,
        class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr void assign(InputIterator first, InputIterator last);
    constexpr void assign(size_type n, const T& val);
    constexpr void assign(std::initializer_list<T> ilist);

    constexpr void push_back(const T& value);
    constexpr void push_back(T&& value);
    constexpr void fast_push_back(const T& value) noexcept;
    constexpr void fast_push_back(T&& value) noexcept;

    constexpr void pop_back();
    constexpr void safe_pop_back() noexcept;

    constexpr iterator insert(const_iterator pos, const T& value);
    constexpr iterator insert(const_iterator pos, T&& value);
    constexpr iterator insert(const_iterator pos, size_type count, const T& value);
    template<class InputIterator,
        class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator first, const_iterator last);

    constexpr void swap(static_vector_alt& other);
    constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    constexpr void emplace_back(Args&&... args);
    template<class... Args>
    constexpr void fast_emplace_back(Args&&... args) noexcept;

private:
    UWR_FORCEINLINE constexpr T* data_at(size_type n) noexcept;
    UWR_FORCEINLINE constexpr const T* data_at(size_type n) const noexcept;

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
    T* m_end;
};

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt() noexcept
    : m_end(data()) {
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(size_type n)
    : m_end(data() + n) {
    mem::construct(data(), n);
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(size_type n, const T& val)
    : m_end(data() + n) {
    mem::ufill(data(), n, val);
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr
static_vector_alt<T, C>::static_vector_alt(InputIterator first, InputIterator last)
    : m_end(data() + std::distance(first, last)) {
    mem::ucopy(data(), first, last);
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(const static_vector_alt& x)
    : m_end(data() + x.size()) {
    mem::ucopy(data(), x.begin(), x.end());
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(static_vector_alt&& x) noexcept
    : m_end(data() + x.size()) {
    mem::umove(data(), x.begin(), x.end());
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(std::initializer_list<T> ilist)
    : m_end(data() + ilist.size()) {
    mem::ucopy(data(), ilist.begin(), ilist.end());
}

template<class T, len_t C>
#if CPP_ABOVE_17
constexpr
#endif
static_vector_alt<T, C>::~static_vector_alt() {
    mem::destroy(data(), m_end);
}

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(const static_vector_alt<T, C>& other) noexcept {
    auto ptr = data();
    auto optr = other.data();
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

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(static_vector_alt<T, C>&& other) noexcept {
    auto ptr = data();
    auto optr = other.data();
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

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(std::initializer_list<T> ilist) noexcept {
    auto ptr = data();
    auto optr = ilist.begin();
    size_type len = size();
    size_type ilist_len = static_cast<size_type>(ilist.size());

    if (ilist_len < len) {
        mem::destroy(ptr + ilist_len, m_end);
        mem::copy(ptr, optr, ilist_len);
    }
    else {
        mem::copy(ptr, optr, len);
        mem::ucopy(m_end, optr + len, optr + ilist_len);
    }

    m_end = ptr + ilist_len;

    return *this;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::begin() noexcept {
    return data();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::begin() const noexcept {
    return data();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::end() noexcept {
    return m_end;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::end() const noexcept {
    return m_end;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reverse_iterator
static_vector_alt<T, C>::rbegin() noexcept {
    return reverse_iterator(m_end);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reverse_iterator
static_vector_alt<T, C>::rbegin() const noexcept {
    return const_reverse_iterator(m_end);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reverse_iterator
static_vector_alt<T, C>::rend() noexcept {
    return reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reverse_iterator
static_vector_alt<T, C>::rend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::cbegin() const noexcept {
    return data();
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_iterator
constexpr static_vector_alt<T, C>::cend() const noexcept {
    return m_end;
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reverse_iterator
constexpr static_vector_alt<T, C>::crbegin() const noexcept {
    return const_reverse_iterator(m_end);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reverse_iterator
constexpr static_vector_alt<T, C>::crend() const noexcept {
    return const_reverse_iterator(data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::size() const noexcept {
    return static_cast<size_type>(m_end - data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::max_size() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::resize(size_type n) {
    size_type len = size();

    if (n > len) {
        auto ptr = data();
        mem::construct(m_end, ptr + n);
        m_end = ptr + n;
    }
    else if (n < len) {
        auto ptr = data();
        mem::destroy(ptr + n, m_end);
        m_end = ptr + n;
    }
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::resize(size_type n, const T& val) {
    size_type len = size();

    if (n > len) {
        auto ptr = data();
        mem::ufill(m_end, ptr + n, val);
        m_end = ptr + n;
    }
    else if (n < len) {
        auto ptr = data();
        mem::destroy(ptr + n, m_end);
        m_end = ptr + n;
    }
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::capacity() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr bool
static_vector_alt<T, C>::empty() const noexcept {
    return data() == m_end;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::reserve(size_type) noexcept {
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::shrink_to_fit() noexcept {
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::operator[](size_type n) {
    return *data_at(n);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reference
constexpr static_vector_alt<T, C>::operator[](size_type n) const {
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::at(size_type n) {
    // TODO: unlikely
    if (data() + n >= m_end)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reference
static_vector_alt<T, C>::at(size_type n) const {
    // TODO: unlikely
    if (data() + n >= m_end)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::front() {
    return *data_at(0);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reference
static_vector_alt<T, C>::front() const {
    return *data_at(0);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::reference
constexpr static_vector_alt<T, C>::back() {
    return *(m_end - 1);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reference
constexpr static_vector_alt<T, C>::back() const {
    return *(m_end - 1);
}

template<class T, len_t C>
constexpr T*
static_vector_alt<T, C>::data() noexcept {
    return data_at(0);
}

template<class T, len_t C>
constexpr const T*
static_vector_alt<T, C>::data() const noexcept {
    return data_at(0);
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr void
static_vector_alt<T, C>::assign(InputIterator first, InputIterator last) {
    auto n = static_cast<size_type>(std::distance(first, last));
    auto ptr = data();
    size_type len = size();

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

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::assign(size_type n, const T& val) {
    auto ptr = data();
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

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::push_back(const_reference value) {
    // TODO: uncomment
    if (m_end == data() + C)
        throw std::out_of_range("Out of bounds");
    new (m_end++) T(value);
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::push_back(T&& value) {
    // TODO: uncomment
    if (m_end == data() + C)
        throw std::out_of_range("Out of bounds");
    new (m_end++) T(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::fast_push_back(const_reference value) noexcept {
    new (m_end++) T(value);
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::fast_push_back(T&& value) noexcept {
    new (m_end++) T(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::pop_back() {
    mem::destroy_at(--m_end);
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::safe_pop_back() noexcept {
    if (m_end == data())
        return;
    mem::destroy_at(--m_end);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, const T& value) {
    auto position = const_cast<T*>(pos);

    // TODO: unlikely or do better
    if (position == m_end)
        new (position) T(value);
    else {
        mem::shiftr(position + 1, position, m_end);
        *position = value;
    }
    ++m_end;

    return position;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, T&& value) {
    auto position = const_cast<T*>(pos);

    if (position == m_end)
        new (position) T(std::move(value));
    else {
        mem::shiftr(position + 1, position, m_end);
        *position = std::move(value);
    }
    ++m_end;

    return position;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, size_type count, const T& value) {
    auto position = const_cast<T*>(pos);
    // TODO: unlikely or can do better
    if (!count)
        return position;

    auto rest = static_cast<size_type>(std::distance(position, m_end));

    // TODO: likely?
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

template<class T, len_t C>
template<class InputIterator, class>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    auto position = const_cast<T*>(pos);

    // TODO: unlikely or can do better
    if (first == last)
        return position;

    auto count = std::distance(first, last);
    auto rest = std::distance(position, m_end);

    // TODO: likely?
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

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::erase(const_iterator pos) {
    auto position = const_cast<T*>(pos);
    mem::shiftl(position, position + 1, m_end);
    pop_back();

    return position;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::erase(const_iterator first, const_iterator last) {
    auto f = const_cast<T*>(first);
    // TODO: unlikely, maybe can do better
    if (first != last) {
        auto l = const_cast<T*>(last);
        mem::shiftl(f, l, m_end);
        mem::destroy(f + (m_end - l), m_end);
        m_end -= l - f;
    }

    return f;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::swap(static_vector_alt<T, C>& other) {
    T* m_begin = begin();
    T* o_begin = other.begin();

    while (true) {
        if (m_begin == m_end) {
            mem::umove(m_begin, o_begin, other.m_end);
            mem::destroy(o_begin, other.m_end);
            m_end += other.m_end - o_begin;
            other.m_end = o_begin;
            break;
        }
        if (o_begin == other.m_end) {
            mem::umove(o_begin, m_begin, m_end);
            mem::destroy(m_begin, m_end);
            other.m_end += m_end - m_begin;
            m_end = m_begin;
            break;
        }
        std::swap(*m_begin++, *o_begin++);
    }
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::clear() noexcept {
    auto m_begin = data();
    mem::destroy(m_begin, m_end);
    m_end = m_begin;
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::emplace(const_iterator pos, Args&&... args) {
    auto position = const_cast<T*>(pos);

    // TODO: unlikely or do better
    if (position == m_end)
        new (position) T(std::forward<Args>(args)...);
    else {
        mem::shiftr(position + 1, position, m_end);
        *position = mem::create<T>(std::forward<Args>(args)...);
    }
    ++m_end;

    return position;
}

template<class T, len_t C>
template<class... Args>
constexpr void
static_vector_alt<T, C>::emplace_back(Args&&... args) {
    // TODO: uncomment
    if (m_end == data() + C)
        throw std::out_of_range("Out of bounds");
    new (m_end++) T(std::forward<Args>(args)...);
}

template<class T, len_t C>
template<class... Args>
constexpr void
static_vector_alt<T, C>::fast_emplace_back(Args&&... args) noexcept {
    new (m_end) T(std::forward<Args>(args)...);
}

template<class T, len_t C>
constexpr T*
static_vector_alt<T, C>::data_at(size_type n) noexcept {
    return reinterpret_cast<T*>(&m_data[n]);
}

template<class T, len_t C>
constexpr const T*
static_vector_alt<T, C>::data_at(size_type n) const noexcept {
    return reinterpret_cast<const T*>(&m_data[n]);
}

/* non-member operators  */
template<class T, len_t C>
constexpr inline bool operator==(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator!=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator<(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator<=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr inline bool operator>=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector_alt<T, C>& v);

#if CPP_ABOVE_17
template<class T, len_t C>
constexpr inline auto operator<=>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
#endif

/* non-member operators' implementations */
template<class T, len_t C>
constexpr bool
operator==(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    if (lhs.size() != rhs.size())
        return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, len_t C>
constexpr bool
operator!=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return !(lhs == rhs);
}

template<class T, len_t C>
constexpr bool
operator<(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, len_t C>
constexpr bool
operator<=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return !(rhs < lhs);
}

template<class T, len_t C>
constexpr bool
operator>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return rhs < lhs;
}

template<class T, len_t C>
constexpr bool
operator>=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return !(lhs < rhs);
}

template<class T, len_t C>
constexpr std::ostream&
operator<<(std::ostream& out, const static_vector_alt<T, C>& v) {
    for (typename static_vector_alt<T, C>::size_type i = 0; i < v.size() - 1; ++i)
        out << v[i] << ' ';
    if (!v.empty())
        out << v.back();
    return out;
}

#if CPP_ABOVE_17

template<class T, len_t C>
constexpr auto
operator<=>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                  rhs.begin(), rhs.end(),
                                                  mem::synth_three_way);
}

#endif

} // namespace uwr


namespace std {

/* non-member functions */
template<class T, uwr::len_t C>
constexpr void swap(uwr::static_vector_alt<T, C>& x, uwr::static_vector_alt<T, C>& y);

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase(uwr::static_vector_alt<T, C>& c, const U& value);

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase_if(uwr::static_vector_alt<T, C>& c, Pred pred);

#endif

/* non-member functions' implementations */
template<class T, uwr::len_t C>
constexpr void
swap(uwr::static_vector_alt<T, C>& x, uwr::static_vector_alt<T, C>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase(uwr::static_vector_alt<T, C>& c, const U& value) {
    // TODO: possible optimizations?
    auto cend = c.end();
    auto it = std::remove(c.begin(), cend, value);
    auto r = std::distance(it, cend);
    c.erase(it, cend);

    return r;
}

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase_if(uwr::static_vector_alt<T, C>& c, Pred pred) {
    // TODO: possible optimizations?
    auto cend = c.end();
    auto it = std::remove_if(c.begin(), cend, pred);
    auto r = std::distance(it, cend);
    c.erase(it, cend);

    return r;
}

#endif // CPP_ABOVE_17

} // namespace std
