#pragma once

#define CPP_ABOVE_17 __cplusplus > 201703L

#include <iterator>
#include "../common/memory.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif

namespace uwr {

using len_t = mem::len_t;
// TODO: restore to uint
static_assert(std::is_same_v<len_t, std::size_t>, "static_vector template type has to be the same as len_t!");

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
    template<class InputIterator,
             class = typename std::iterator_traits<InputIterator>::value_type>
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
    constexpr static_vector& operator=(std::initializer_list<T> ilist) noexcept;

    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;

    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;

    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    constexpr size_type size() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr void resize(size_type n);
    constexpr void resize(size_type n, const T& val);
    constexpr size_type capacity() const noexcept;
    [[nodiscard]] constexpr bool empty() const noexcept;
    constexpr void reserve(size_type n) noexcept;
    constexpr void shrink_to_fit() noexcept;

    constexpr reference operator[](size_type n);
    constexpr const_reference operator[](size_type n) const;
    constexpr reference at(size_type n);
    constexpr const_reference at(size_type n) const;
    constexpr reference front();
    constexpr const_reference front() const;
    constexpr reference back();
    constexpr const_reference back() const;
    constexpr T* data() noexcept;
    constexpr const T* data() const noexcept;

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

    constexpr void swap(static_vector& other);
    constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    constexpr void emplace_back(Args&&... args);
    template<class... Args>
    constexpr void fast_emplace_back(Args&&... args) noexcept;

private:
    // TODO: maybe inline
    constexpr
        T* data_at(size_type n) noexcept;
    constexpr const T* data_at(size_type n) const noexcept;

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
    auto ptr = data();
    auto optr = other.data();

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
    auto ptr = data();
    auto optr = other.data();

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
    auto ptr = data();
    auto optr = ilist.begin();
    auto ilist_len = ilist.size();

    if (ilist_len < m_length) {
        mem::destroy(ptr + ilist_len, ptr + m_length);
        mem::copy(ptr, optr, ilist_len);
    }
    else {
        mem::copy(ptr, optr, m_length);
        mem::ucopy(ptr + m_length, optr + m_length, optr + ilist_len);
    }

    m_length = ilist_len;

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
static_vector<T, C>::resize(size_type n) {
    if (n > m_length) {
        auto ptr = data();
        mem::construct(ptr + m_length, ptr + n);
        m_length = n;
    }
    else if (n < m_length) {
        auto ptr = data();
        mem::destroy(ptr + n, ptr + m_length);
        m_length = n;
    }
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::resize(size_type n, const T& val) {
    if (n > m_length) {
        auto ptr = data();
        mem::ufill(ptr + m_length, ptr + n, val);
        m_length = n;
    }
    else if (n < m_length) {
        auto ptr = data();
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

template<class T, len_t C>
constexpr typename static_vector<T, C>::reference
static_vector<T, C>::at(size_type n) {
    // TODO: unlikely
    if (n >= m_length)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return *data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::const_reference
static_vector<T, C>::at(size_type n) const {
    // TODO: unlikely
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
    auto n = static_cast<len_t>(std::distance(first, last));
    auto ptr = data();

    if (n < m_length) {
        mem::destroy(ptr + n, ptr + m_length);
        mem::copy(ptr, first, n);
    }
    else {
        mem::copy(ptr, first, m_length);
        mem::ucopy(ptr + m_length, first + m_length, last);
    }

    m_length = n;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::assign(size_type n, const T& val) {
    auto ptr = data();
    
    if (n < m_length) {
        mem::destroy(ptr + n, ptr + m_length);
        mem::fill(ptr, n, val);
    }
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
    // TODO: uncomment
    if (m_length == C)
        throw std::out_of_range("Out of bounds");
    new (data() + m_length++) T(value);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::push_back(T&& value) {
    // TODO: uncomment
    if (m_length == C)
        throw std::out_of_range("Out of bounds");
    new (data() + m_length++) T(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::fast_push_back(const_reference value) noexcept {
    new (data() + m_length++) T(value);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::fast_push_back(T&& value) noexcept {
    new (data() + m_length++) T(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::pop_back() {
    mem::destroy_at(data() + --m_length);
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::safe_pop_back() noexcept {
    if (m_length == 0)
        return;
    mem::destroy_at(data() + --m_length);
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, const T& value) {
    auto position = const_cast<T*>(pos);
    // TODO: unlikely or do better
    if (position == end())
        new (position) T(value);
    else {
        mem::shiftr(position + 1, position, end());
        *position = value;
    }
    ++m_length;

    return position;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, T&& value) {
    auto position = const_cast<T*>(pos);
    if (position == end())
        new (position) T(std::move(value));
    else {
        mem::shiftr(position + 1, position, end());
        *position = std::move(value);
    }
    ++m_length;

    return position;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::insert(const_iterator pos, size_type count, const T& value) {
    auto position = const_cast<T*>(pos);
    // TODO: unlikely or can do better
    if (!count)
        return position;

    auto eptr = end();
    auto rest = static_cast<size_type>(std::distance(position, eptr));

    // TODO: likely?
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
    auto position = const_cast<T*>(pos);
    // TODO: unlikely or can do better
    if (first == last)
        return position;

    auto eptr = end();
    auto count = std::distance(first, last);
    auto rest = std::distance(position, eptr);

    // TODO: likely?
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
    auto position = const_cast<T*>(pos);
    mem::shiftl(position, position + 1, end());
    pop_back();

    return position;
}

template<class T, len_t C>
constexpr typename static_vector<T, C>::iterator
static_vector<T, C>::erase(const_iterator first, const_iterator last) {
    auto f = const_cast<T*>(first);
    // TODO: unlikely, maybe can do better
    if (first != last) {
        auto l = const_cast<T*>(last);
        auto e = end();
        mem::shiftl(f, l, e);
        mem::destroy(f + (e - l), e);
        m_length -= l - f;
    }

    return f;
}

template<class T, len_t C>
constexpr void
static_vector<T, C>::swap(static_vector<T, C>& other) {
    static_vector<T, C> *o1 = this, *o2 = &other;
    size_type min_len = std::min(o1->m_length, o2->m_length);
    for (size_type i = 0; i < min_len; ++i)
        std::swap((*this)[i], other[i]);

    if (o1->m_length != min_len)
        std::swap(o1, o2);

    mem::umove(o1->begin() + min_len, o2->begin() + min_len, o2->end());
    mem::destroy(o2->begin() + min_len, o2->end());
    std::swap(o1->m_length, o2->m_length);
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
    auto position = const_cast<T*>(pos);
    // TODO: unlikely or do better
    if (position == end())
        new (position) T(std::forward<Args>(args)...);
    else {
        mem::shiftr(position + 1, position, end());
        *position = mem::create<T>(std::forward<Args>(args)...);
    }
    ++m_length;

    return position;
}

template<class T, len_t C>
template<class... Args>
constexpr void
static_vector<T, C>::emplace_back(Args&&... args) {
    // TODO: uncomment
    if (m_length == C)
        throw std::out_of_range("Out of bounds");
    new (data() + m_length++) T(std::forward<Args>(args)...);
}

template<class T, len_t C>
template<class... Args>
constexpr void
static_vector<T, C>::fast_emplace_back(Args&&... args) noexcept {
    new (data() + m_length++) T(std::forward<Args>(args)...);
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

/* non-member operators  */
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

/* non-member operators' implementations */
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


namespace std {

/* non-member functions */
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

/* non-member functions' implementations */
template<class T, uwr::len_t C>
constexpr void
swap(uwr::static_vector<T, C>& x, uwr::static_vector<T, C>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector<T, C>::size_type
erase(uwr::static_vector<T, C>& c, const U& value) {
    // TODO: possible optimizations?
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());

    return r;
}

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector<T, C>::size_type
erase_if(uwr::static_vector<T, C>& c, Pred pred) {
    // TODO: possible optimizations?
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());

    return r;
}

#endif // CPP_ABOVE_17

} // namespace std
