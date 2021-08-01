#pragma once

#include <iterator>
#include <limits>

#include "../common/allocator.hpp"
#include "../common/define.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif

namespace uwr {

using len_t = mem::len_t;

template<class T, class size_t=len_t>
class vector {
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

    constexpr vector() noexcept;
    constexpr explicit vector(size_type n);
    constexpr vector(size_type n, const T& val);
    template<class InputIterator,
             class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr vector(InputIterator first, InputIterator last);
    constexpr vector(const vector& x);
    constexpr vector(vector&& x) noexcept;
    constexpr vector(std::initializer_list<T> ilist);

    #if CPP_ABOVE_17
    constexpr
    #endif
    ~vector();

    constexpr vector& operator=(const vector& other) noexcept;
    constexpr vector& operator=(vector&& other) noexcept;
    constexpr vector& operator=(std::initializer_list<T> ilist) noexcept;

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
    constexpr void fast_push_back(const T& value) noexcept;
    constexpr void push_back(T&& value);
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

    constexpr void swap(vector& other);
    constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    constexpr void emplace_back(Args&&... args);
    template<class... Args>
    constexpr void fast_emplace_back(Args&&... args) noexcept;

private:
    size_type m_length;
    size_type m_capacity;
    T* m_data;
};

template<class T, class size_t>
constexpr
vector<T, size_t>::vector() noexcept
    : m_length(0),
      m_capacity(0),
      m_data(nullptr) {
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(size_type n)
    : m_length(n),
      m_capacity(mem::fix_capacity<T>(n)),
      m_data(mem::allocate<T>(m_capacity)) {
    mem::construct(m_data, n);
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(size_type n, const T& val)
    : m_length(n),
      m_capacity(mem::fix_capacity<T>(n)),
      m_data(mem::allocate<T>(m_capacity)) {
    mem::ufill(m_data, n, val);
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr
vector<T, size_t>::vector(InputIterator first, InputIterator last)
    : m_length(std::distance(first, last)),
      m_capacity(mem::fix_capacity<T>(m_length)),
      m_data(mem::allocate<T>(m_capacity)) {
    mem::ucopy(m_data, first, last);
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(const vector& x)
    : m_length(x.m_length),
      m_capacity(x.m_capacity),
      m_data(mem::allocate<T>(m_capacity)) {
    mem::ucopy(m_data, x.begin(), x.end());
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(vector&& x) noexcept
    : m_length(x.m_length),
      m_capacity(x.m_capacity),
      m_data(x.m_data) {
    x.m_length = 0;
    x.m_capacity = 0;
    x.m_data = nullptr;
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(std::initializer_list<T> ilist)
    : m_length(ilist.size()),
      m_capacity(mem::fix_capacity<T>(m_length)),
      m_data(mem::allocate<T>(m_capacity)) {
    mem::ucopy(m_data, ilist.begin(), ilist.end());
}

template<class T, class size_t>
#if CPP_ABOVE_17
constexpr
#endif
vector<T, size_t>::~vector() {
    mem::destroy(m_data, m_length);
    mem::deallocate(m_data, m_capacity);
}

// TODO: fix
template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(const vector<T, size_t>& other) noexcept {
    if (UNLIKELY(this == std::addressof(other)))
        return *this;
    if (other.m_length > m_length)
        mem::change_capacity(m_data, m_length, m_capacity, other.m_capacity);

    mem::destroy(m_data, m_length);
    mem::ucopy(m_data, other.begin(), other.end());
    m_length = other.m_length;

    return *this;
}

// TODO: fix
template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(vector<T, size_t>&& other) noexcept {
    std::swap(m_length, other.m_length);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_data, other.m_data);

    return *this;
}

// TODO: fix
template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(std::initializer_list<T> ilist) noexcept {
    if (ilist.size() > m_capacity)
        mem::change_capacity(m_data, m_length, m_capacity, ilist.size());
    
    mem::destroy(m_data, m_data + m_length);
    mem::ucopy(m_data, ilist.begin(), ilist.end());
    m_length = ilist.size();

    return *this;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::begin() noexcept {
    return m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::begin() const noexcept {
    return m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::end() noexcept {
    return m_data + m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::end() const noexcept {
    return m_data + m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reverse_iterator
vector<T, size_t>::rbegin() noexcept {
    return reverse_iterator(m_data + m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::rbegin() const noexcept {
    return const_reverse_iterator(m_data + m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reverse_iterator
vector<T, size_t>::rend() noexcept {
    return reverse_iterator(m_data);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::rend() const noexcept {
    return const_reverse_iterator(m_data);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::cbegin() const noexcept {
    return m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::cend() const noexcept {
    return m_data + m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::crbegin() const noexcept {
    return const_reverse_iterator(m_data + m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::crend() const noexcept {
    return const_reverse_iterator(m_data);
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::size() const noexcept {
    return m_length;
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(T);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::resize(size_type n) {
    if (n > m_capacity)
        mem::change_capacity(m_data, m_length, m_capacity, n);

    if (n < m_length) {
        mem::destroy(m_data + n, m_data + m_length);
        m_length = n;
    }
    else if (n > m_length)
        mem::construct(m_data + m_length, n - m_length);

    m_length = n;
}

template<class T, class size_t>
constexpr void vector<T, size_t>::resize(size_type n, const T& val) {
    if (n > m_capacity)
        mem::change_capacity(m_data, m_length, m_capacity, n);

    if (n < m_length) {
        mem::destroy(m_data + n, m_data + m_length);
        m_length = n;
    }
    else if (n > m_length)
        mem::ufill(m_data + m_length, n - m_length, val);

    m_length = n;
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::capacity() const noexcept {
    return m_capacity;
}

template<class T, class size_t>
constexpr bool
vector<T, size_t>::empty() const noexcept {
    return m_length == 0;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::reserve(size_type n) noexcept {
    if (n <= m_capacity)
        return;
    n = n < 2 * m_capacity ? 2 * m_capacity : n;
    mem::change_capacity(m_data, m_length, m_capacity, n);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::shrink_to_fit() noexcept {
    if (m_capacity < mem::map_threshold<T>)
        mem::change_capacity(m_data, m_length, m_capacity, m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::operator[](size_type n) {
    return m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::operator[](size_type n) const {
    return m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::at(size_type n) {
    // TODO: unlikely
    if (n >= m_length)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::at(size_type n) const {
    // TODO: unlikely
    if (n >= m_length)
        throw std::out_of_range("Index out of range: " + std::to_string(n));
    return m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::front() {
    return m_data[0];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::front() const {
    return m_data[0];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::back() {
    return m_data[m_length - 1];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::back() const {
    return m_data[m_length - 1];
}

template<class T, class size_t>
constexpr T*
vector<T, size_t>::data() noexcept {
    return m_data;
}

template<class T, class size_t>
constexpr const T*
vector<T, size_t>::data() const noexcept {
    return m_data;
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr void
vector<T, size_t>::assign(InputIterator first, InputIterator last) {
    size_type count = std::distance(first, last);
    if (count > m_capacity)
        mem::change_capacity(m_data, m_length, m_capacity, count);

    mem::destroy(m_data, m_data + m_length);
    mem::ucopy(m_data, first, last);
    
    m_length = count;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::assign(size_type n, const T& val) {
    if (n > m_capacity)
        mem::change_capacity(m_data, m_length, m_capacity, n);

    mem::destroy(m_data, m_data + m_length);
    mem::ufill(m_data, n, val);

    m_length = n;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::push_back(const_reference value) {
    mem::grow(m_data, m_length, m_capacity);
    new (data() + m_length) T(value);
    ++m_length;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::fast_push_back(const_reference value) noexcept {
    new (m_data + m_length) T(value);
    ++m_length;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::push_back(T&& value) {
    mem::grow(m_data, m_length, m_capacity);
    new (m_data + m_length) T(std::forward<T>(value));
    ++m_length;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::fast_push_back(T&& value) noexcept {
    new (m_data + m_length) T(std::forward<T>(value));
    ++m_length;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::pop_back() {
    if constexpr (!std::is_trivially_destructible_v<T>)
        back().~T();
    --m_length;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::safe_pop_back() noexcept {
    if (m_length == 0)
        return;
    if constexpr (!std::is_trivially_destructible_v<T>)
        back().~T();
    --m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, const T& value) {
    auto position = const_cast<T*>(pos);
    auto m = std::distance(begin(), position);
    mem::grow(m_data, m_length, m_capacity);
    position = begin() + m;
    if (position != end())
        mem::shiftr_data(position, (end() - position));
    new (position) T(value);
    ++m_length;

    return position;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, T&& value) {
    auto position = const_cast<T*>(pos);
    auto m = std::distance(begin(), position);
    mem::grow(m_data, m_length, m_capacity);
    position = begin() + m;
    if (position != end())
        mem::shiftr_data(position, (end() - position));
    new (position) T(std::forward<T>(value));
    ++m_length;

    return position;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, size_type count, const T& value) {
    auto position = const_cast<T*>(pos);
    if (!count)
        return position;

    if (m_length + count > m_capacity) {
        auto m = std::distance(begin(), position);
        size_type new_cap = std::max(m_length + count, m_capacity * 2);
        mem::change_capacity(m_data, m_length, m_capacity, new_cap);
        position = begin() + m;
    }
    auto end_ = end();
    size_type rest = std::distance(position, end_);
    if (rest > count) {
        std::uninitialized_move(end_ - count, end_, end_);
        std::move_backward(position, end_ - count, end_);
        std::fill(position, position + count, value);
    }
    else {
        std::uninitialized_move(position, end_, position + count);
        std::fill(position, end_, value);
        std::uninitialized_fill(end_, position + count, value);
    }
    m_length += count;

    return position;
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    auto position = const_cast<T*>(pos);
    if (first == last)
        return position;

    size_type count = std::distance(first, last);
    if (m_length + count > m_capacity) {
        auto m = std::distance(begin(), position);
        size_type new_cap = std::max(m_length + count, m_capacity * 2);
        mem::change_capacity(m_data, m_length, m_capacity, new_cap);
        position = begin() + m;
    }
    auto end_ = end();
    size_type rest = std::distance(position, end_);
    if (rest > count) {
        std::uninitialized_move(end_ - count, end_, end_);
        std::move_backward(position, end_ - count, end_);
        std::copy(first, last, position);
    }
    else {
        std::uninitialized_move(position, end_, position + count);
        std::copy(first, first + rest, position);
        std::uninitialized_copy(first + rest, last, end_);
    }
    m_length += count;

    return position;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::erase(const_iterator pos) {
    auto position = const_cast<T*>(pos);
    if (position + 1 != end())
        std::copy(position + 1, end(), position);
    pop_back();

    return position;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::erase(const_iterator first, const_iterator last) {
    auto f = const_cast<T*>(first);
    if (first == last)
        return f;

    auto l = const_cast<T*>(last);
    auto n = std::distance(first, last);
    if (last != end())
        std::move(l, end(), f);
    mem::destroy(end() - n, end());
    m_length -= n;

    return f;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::swap(vector<T, size_t>& other) {
    using std::swap;
    swap(m_data, other.m_data);
    swap(m_length, other.m_length);
    swap(m_capacity, other.m_capacity);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::clear() noexcept {
    mem::destroy(m_data, m_data + m_length);
    m_length = 0;
}

template<class T, class size_t>
template<class... Args>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::emplace(const_iterator pos, Args&&... args) {
    // TODO: improve when pos == end()
    return insert(pos, T(std::forward<Args>(args)...));
}

template<class T, class size_t>
template<class... Args>
constexpr void
vector<T, size_t>::emplace_back(Args&&... args) {
    mem::grow(m_data, m_length, m_capacity);
    new (m_data + m_length) T(std::forward<Args>(args)...);
    ++m_length;
}

template<class T, class size_t>
template<class... Args>
constexpr void
vector<T, size_t>::fast_emplace_back(Args&&... args) noexcept {
    new (m_data + m_length) T(std::forward<Args>(args)...);
    ++m_length;
}


/* non-member operators  */
template<class T, class size_t>
constexpr inline bool operator==(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr inline bool operator!=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr inline bool operator<(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr inline bool operator<=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr inline bool operator>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr inline bool operator>=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr std::ostream& operator<<(std::ostream& out, const vector<T, size_t>& v);

#if CPP_ABOVE_17
template<class T, class size_t>
constexpr inline auto operator<=>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
#endif

/* non-member operators' implementations */
template<class T, class size_t>
constexpr bool
operator==(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    if (lhs.size() != rhs.size())
        return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, class size_t>
constexpr bool
operator!=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return !(lhs == rhs);
}

template<class T, class size_t>
constexpr bool
operator<(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class size_t>
constexpr bool
operator<=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return !(rhs < lhs);
}

template<class T, class size_t>
constexpr bool
operator>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return rhs < lhs;
}

template<class T, class size_t>
constexpr bool
operator>=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return !(lhs < rhs);
}

template<class T, class size_t>
constexpr std::ostream&
operator<<(std::ostream& out, const vector<T, size_t>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i)
        out << v[i] << ' ';
    if (!v.empty())
        out << v.back();
    return out;
}

#if CPP_ABOVE_17

template<class T, class size_t>
constexpr auto
operator<=>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(),
                                                  rhs.begin(), rhs.end(),
                                                  mem::synth_three_way);
}

#endif

} // namespace uwr


namespace std {

#if CPP_ABOVE_17

/* non-member functions */
template<class T, class U>
constexpr typename uwr::vector<T, size_t>::size_type
erase(uwr::vector<T, size_t>& c, const U& value);

template<class T, class Pred>
constexpr typename uwr::vector<T, size_t>::size_type
erase_if(uwr::vector<T, size_t>& c, Pred pred);

#endif

/* non-member functions' implementations */
template<class T, class size_t>
constexpr void
swap(uwr::vector<T, size_t>& x, uwr::vector<T, size_t>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, class U>
constexpr typename uwr::vector<T, size_t>::size_type
erase(uwr::vector<T, size_t>& c, const U& value) {
    // TODO: possible optimizations?
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());

    return r;
}

template<class T, class Pred>
constexpr typename uwr::vector<T, size_t>::size_type
erase_if(uwr::vector<T, size_t>& c, Pred pred) {
    // TODO: possible optimizations?
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = std::distance(it, c.end());
    c.erase(it, c.end());

    return r;
}

#endif // CPP_ABOVE_17

} // namespace uwr
