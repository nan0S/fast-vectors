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

    UWR_FORCEINLINE constexpr static_vector_alt& operator=(const static_vector_alt& other) noexcept;
    UWR_FORCEINLINE constexpr static_vector_alt& operator=(static_vector_alt&& other) noexcept;
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

    template<class InputIterator>
    UWR_FORCEINLINE constexpr void assign(InputIterator first, InputIterator last);
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
    template<class InputIterator>
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
            size_type s_length, size_type l_length) const;
    template<class InputIterator>
    constexpr void priv_copy_assign(InputIterator first, InputIterator last, size_type n);
    template<class InputIterator>
    constexpr void priv_move_assign(InputIterator first, InputIterator last, size_type n);

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
    T* m_end;
};

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt() noexcept
    : m_end(this->data()) {
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(size_type n)
    : m_end(this->data() + n) {
    mem::construct(this->data(), this->m_end);
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(size_type n, const T& val)
    : m_end(this->data() + n) {
    mem::ufill(this->data(), this->m_end, val);
}

template<class T, len_t C, class size_t>
template<class InputIterator, class>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(InputIterator first, InputIterator last)
    : m_end(this->data() + std::distance(first, last)) {
    mem::ucopy(this->data(), first, last);
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(const static_vector_alt& x)
    : m_end(this->data() + x.size()) {
    mem::ucopy(this->data(), x.begin(), x.end());
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(static_vector_alt&& x) noexcept
    : m_end(this->data() + x.size()) {
    mem::umove(this->data(), x.begin(), x.end());
}

template<class T, len_t C, class size_t>
constexpr
static_vector_alt<T, C, size_t>::static_vector_alt(std::initializer_list<T> ilist)
    : m_end(this->data() + ilist.size()) {
    mem::ucopy(this->data(), ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
#if CPP_ABOVE_17
constexpr
#endif
static_vector_alt<T, C, size_t>::~static_vector_alt() {
    mem::destroy(this->data(), this->m_end);
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(const static_vector_alt<T, C, size_t>& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_copy_assign(other.begin(), other.end(), other.size());
    return *this;
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(static_vector_alt<T, C, size_t>&& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_move_assign(other.begin(), other.end(), other.size());
    return *this;
}

template<class T, len_t C, class size_t>
constexpr static_vector_alt<T, C, size_t>&
static_vector_alt<T, C, size_t>::operator=(std::initializer_list<T> ilist) noexcept {
    this->assign(ilist.begin(), ilist.end());
    return *this;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::begin() noexcept {
    return this->data();
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::begin() const noexcept {
    return this->data();
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::end() noexcept {
    return this->m_end;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::end() const noexcept {
    return this->m_end;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reverse_iterator
static_vector_alt<T, C, size_t>::rbegin() noexcept {
    return reverse_iterator(this->m_end);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reverse_iterator
static_vector_alt<T, C, size_t>::rbegin() const noexcept {
    return const_reverse_iterator(this->m_end);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reverse_iterator
static_vector_alt<T, C, size_t>::rend() noexcept {
    return reverse_iterator(this->data());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reverse_iterator
static_vector_alt<T, C, size_t>::rend() const noexcept {
    return const_reverse_iterator(this->data());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_iterator
static_vector_alt<T, C, size_t>::cbegin() const noexcept {
    return this->data();
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_iterator
constexpr static_vector_alt<T, C, size_t>::cend() const noexcept {
    return this->m_end;
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reverse_iterator
constexpr static_vector_alt<T, C, size_t>::crbegin() const noexcept {
    return const_reverse_iterator(this->m_end);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reverse_iterator
constexpr static_vector_alt<T, C, size_t>::crend() const noexcept {
    return const_reverse_iterator(this->data());
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::size() const noexcept {
    return static_cast<size_type>(this->m_end - this->data());
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::max_size() const noexcept {
    return C;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::resize(size_type n) {
    T* const new_end = this->data() + n;

    if (new_end > this->m_end)
        mem::construct(this->m_end, new_end);
    else
        mem::destroy(new_end, this->m_end);

    this->m_end = new_end;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::resize(size_type n, const T& val) {
    T* const new_end = this->data() + n;

    if (new_end > this->m_end)
        mem::ufill(this->m_end, new_end, val);
    else
        mem::destroy(new_end, this->m_end);

    this->m_end = new_end;
}

template<class T, len_t C, class size_t>
constexpr size_t
static_vector_alt<T, C, size_t>::capacity() const noexcept {
    return C;
}

template<class T, len_t C, class size_t>
constexpr bool
static_vector_alt<T, C, size_t>::empty() const noexcept {
    return this->data() == this->m_end;
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
    return *this->data_at(n);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reference
constexpr static_vector_alt<T, C, size_t>::operator[](size_type n) const {
    return *this->data_at(n);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::at(size_type n) {
    if (this->data() + n < this->m_end)
        return *this->data_at(n);
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reference
static_vector_alt<T, C, size_t>::at(size_type n) const {
    if (this->data() + n < this->m_end)
        return *this->data_at(n);
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::front() {
    return *this->data_at(0);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::const_reference
static_vector_alt<T, C, size_t>::front() const {
    return *this->data_at(0);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::reference
constexpr static_vector_alt<T, C, size_t>::back() {
    return *(this->m_end - 1);
}

template<class T, len_t C, class size_t>
typename static_vector_alt<T, C, size_t>::const_reference
constexpr static_vector_alt<T, C, size_t>::back() const {
    return *(this->m_end - 1);
}

template<class T, len_t C, class size_t>
constexpr T*
static_vector_alt<T, C, size_t>::data() noexcept {
    return this->data_at(0);
}

template<class T, len_t C, class size_t>
constexpr const T*
static_vector_alt<T, C, size_t>::data() const noexcept {
    return this->data_at(0);
}

template<class T, len_t C, class size_t>
template<class InputIterator>
constexpr void
static_vector_alt<T, C, size_t>::assign(InputIterator first, InputIterator last) {
    this->priv_copy_assign(first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::assign(size_type n, const T& val) {
    T* const m_data = this->data();
    T* const split = m_data + n;
    
    if (split > this->m_end) {
        mem::fill(m_data, this->m_end, val);
        mem::ufill(this->m_end, split, val);
    }
    else {
        mem::destroy(split, this->m_end);
        mem::fill(m_data, split, val);
    }

    this->m_end = split;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::assign(std::initializer_list<T> ilist) {
    this->assign(ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::push_back(const_reference value) {
    this->emplace_back(value);
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::push_back(T&& value) {
    this->emplace_back(std::move(value));
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::fast_push_back(T&& value) noexcept {
    this->fast_emplace_back(std::forward<T>(value));
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::pop_back() {
    mem::destroy_at(--this->m_end);
}

template<class T, len_t C, class size_t>
constexpr bool
static_vector_alt<T, C, size_t>::safe_pop_back() noexcept {
    if (UWR_LIKELY(this->m_end != this->data())) {
        this->pop_back();
        return true;
    }
    else
        return false;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, const T& value) {
    return this->emplace(pos, value);
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, T&& value) {
    return this->emplace(pos, std::move(value));
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, size_type count, const T& value) {
    T* const position = const_cast<T* const>(pos);

    if (UWR_UNLIKELY(!count))
        return position;

    T* const spill = position + count;

    if (spill < this->m_end) {
        mem::shiftr(spill, position, this->m_end);
        mem::fill(position, spill, value);
    }
    else {
        mem::umove(spill, position, this->m_end);
        mem::fill(position, this->m_end, value);
        mem::ufill(this->m_end, spill, value);
    }

    this->m_end += count;

    return position;
}

template<class T, len_t C, class size_t>
template<class InputIterator>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    T* const position = const_cast<T* const>(pos);
    size_type count = static_cast<size_type>(std::distance(first, last));

    if (UWR_UNLIKELY(count == 0))
        return position;
    
    T* const spill = position + count;

    if (spill < this->m_end) {
        mem::shiftr(spill, position, this->m_end);
        mem::copy(position, first, last);
    }
    else {
        size_type rest = static_cast<size_type>(std::distance(position, this->m_end));
        InputIterator split = std::next(first, rest);

        mem::umove(spill, position, this->m_end);
        mem::copy(position, first, split);
        mem::ucopy(this->m_end, split, last);
    }

    this->m_end += count;

    return position;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return this->insert(pos, ilist.begin(), ilist.end());
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::erase(const_iterator pos) {
    T* const position = const_cast<T* const>(pos);
    mem::shiftl(position, position + 1, this->m_end);
    this->pop_back();

    return position;
}

template<class T, len_t C, class size_t>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::erase(const_iterator first, const_iterator last) {
    T* const f = const_cast<T* const>(first);
    size_type count = static_cast<size_type>(std::distance(first, last));

    if (UWR_LIKELY(count != 0)) {
        T* const l = const_cast<T* const>(last);

        mem::shiftl(f, l, this->m_end);
        mem::destroy(this->m_end - count, this->m_end);
        this->m_end -= count;
    }

    return f;
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::swap(static_vector_alt<T, C, size_t>& other) {
    size_type m_len = this->size();
    size_type o_len = other.size();

    if (m_len < o_len)
        this->priv_swap(*this, other, m_len, o_len);
    else
        this->priv_swap(other, *this, o_len, m_len);
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::clear() noexcept {
    mem::destroy(this->data(), this->m_end);
    this->m_end = this->data();
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::iterator
static_vector_alt<T, C, size_t>::emplace(const_iterator pos, Args&&... args) {
    T* const position = const_cast<T* const>(pos);

    if (position != this->m_end) {
        mem::shiftr(position + 1, position, this->m_end);
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
    else
        new (position) T(std::forward<Args>(args)...);

    ++this->m_end;

    return position;
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::emplace_back(Args&&... args) {
    if (UWR_LIKELY(this->m_end != this->data_at(C)))
        return this->fast_emplace_back(std::forward<Args>(args)...);
    else
        throw std::out_of_range("Out of bounds");
}

template<class T, len_t C, class size_t>
template<class... Args>
constexpr typename static_vector_alt<T, C, size_t>::reference
static_vector_alt<T, C, size_t>::fast_emplace_back(Args&&... args) noexcept {
    new (this->m_end) T(std::forward<Args>(args)...);
    return *this->m_end++;
}

template<class T, len_t C, class size_t>
constexpr T*
static_vector_alt<T, C, size_t>::data_at(size_type n) noexcept {
    return reinterpret_cast<T*>(&this->m_data[n]);
}

template<class T, len_t C, class size_t>
constexpr const T*
static_vector_alt<T, C, size_t>::data_at(size_type n) const noexcept {
    return reinterpret_cast<const T*>(&this->m_data[n]);
}

template<class T, len_t C, class size_t>
constexpr void
static_vector_alt<T, C, size_t>::priv_swap(
        static_vector_alt<T, C, size_t>& shorter,
        static_vector_alt<T, C, size_t>& longer,
        size_type s_len, size_type l_len) const {
    T* const s_begin = shorter.begin();
    T* const l_begin = longer.begin();

    std::swap_ranges(s_begin, shorter.m_end, l_begin);
    mem::umove(shorter.m_end, l_begin + s_len, longer.m_end);
    mem::destroy(l_begin + s_len, longer.m_end);

    shorter.m_end = s_begin + l_len;
    longer.m_end = l_begin + s_len;
}

template<class T, len_t C, class size_t>
template<class InputIterator>
constexpr void
static_vector_alt<T, C, size_t>::priv_copy_assign(InputIterator first, InputIterator last, size_type n) {
    UWR_ASSERT(std::distance(first, last) == n);

    T* const m_data = this->data();
    T* const new_end = m_data + n;

    if (new_end < this->m_end) {
        mem::destroy(new_end, this->m_end);
        mem::copy(m_data, first, last);
    }
    else {
        size_type m_len = this->size();
        InputIterator split = std::next(first, m_len);

        mem::copy(m_data, first, split);
        mem::ucopy(this->m_end, split, last);
    }

    this->m_end = new_end;
}

template<class T, len_t C, class size_t>
template<class InputIterator>
constexpr void
static_vector_alt<T, C, size_t>::priv_move_assign(InputIterator first, InputIterator last, size_type n) {
    UWR_ASSERT(std::distance(first, last) == n);

    T* const m_data = this->data();
    T* const new_end = m_data + n;

    if (new_end < this->m_end) {
        mem::destroy(new_end, this->m_end);
        mem::move(m_data, first, last);
    }
    else {
        size_type m_len = this->size();
        InputIterator split = std::next(first, m_len);

        mem::move(m_data, first, split);
        mem::umove(this->m_end, split, last);
    }

    this->m_end = new_end;
}

/*
 * non-member operators 
 */
template<class T, len_t C, class size_t>
UWR_FORCEINLINE constexpr bool operator==(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
UWR_FORCEINLINE constexpr bool operator!=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr bool operator<(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
UWR_FORCEINLINE constexpr bool operator<=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
UWR_FORCEINLINE constexpr bool operator>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
UWR_FORCEINLINE constexpr bool operator>=(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);
template<class T, len_t C, class size_t>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector_alt<T, C, size_t>& v);

#if CPP_ABOVE_17

template<class T, len_t C, class size_t>
constexpr auto operator<=>(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs);

#endif

/*
 * non-member operators' implementations
 */
template<class T, len_t C, class size_t>
constexpr bool
operator==(const static_vector_alt<T, C, size_t>& lhs, const static_vector_alt<T, C, size_t>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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

template<class T, uwr::len_t C, class size_t>
constexpr void swap(uwr::static_vector_alt<T, C, size_t>& x, uwr::static_vector_alt<T, C, size_t>& y);

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class size_t, class U>
constexpr size_t erase(uwr::static_vector_alt<T, C, size_t>& c, const U& value);
template<class T, uwr::len_t C, class size_t, class Pred>
constexpr size_t erase_if(uwr::static_vector_alt<T, C, size_t>& c, Pred pred);

#endif

/*
 * non-member functions' implementations
 */
template<class T, uwr::len_t C, class size_t>
constexpr void
swap(uwr::static_vector_alt<T, C, size_t>& x, uwr::static_vector_alt<T, C, size_t>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class size_t, class U>
constexpr size_t
erase(uwr::static_vector_alt<T, C, size_t>& c, const U& value) {
    T* const c_end = c.end();
    T* const mid = std::remove(c.begin(), c_end, value);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

template<class T, uwr::len_t C, class size_t, class Pred>
constexpr size_t
erase_if(uwr::static_vector_alt<T, C, size_t>& c, Pred pred) {
    T* const c_end = c.end();
    T* const mid = std::remove_if(c.begin(), c_end, pred);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

#endif // CPP_ABOVE_17

} // namespace std
