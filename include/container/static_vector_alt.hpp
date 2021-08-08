#pragma once

#include <iterator>

#include "../common/memory.hpp"
#include "../common/define.hpp"
#include "../common/proxy.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif

namespace uwr {

using len_t = mem::len_t;

template<class T, len_t C>
class static_vector_alt {
public:
    using value_type = T;
    using size_type = len_t;
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
    UWR_FORCEINLINE constexpr void resize(size_type n);
    UWR_FORCEINLINE constexpr void resize(size_type n, const T& val);
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
    UWR_FORCEINLINE constexpr void assign(InputIterator first, InputIterator last);
    UWR_FORCEINLINE constexpr void assign(size_type n, const T& val);
    UWR_FORCEINLINE constexpr void assign(std::initializer_list<T> ilist);

    UWR_FORCEINLINE constexpr void push_back(const T& value);
    UWR_FORCEINLINE constexpr void push_back(T&& value);
    UWR_FORCEINLINE constexpr void fast_push_back(T&& value) noexcept;

    UWR_FORCEINLINE constexpr void pop_back();
    UWR_FORCEINLINE constexpr bool safe_pop_back() noexcept;

    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, const T& value);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, T&& value);
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, size_type count, const T& value);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    UWR_FORCEINLINE constexpr iterator insert(const_iterator pos, InputIterator first, InputIterator last);
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
    template<class InputIterator>
    UWR_FORCEINLINE constexpr void priv_copy_assign(InputIterator first, InputIterator last, size_type n);
    template<class InputIterator>
    UWR_FORCEINLINE constexpr void priv_move_assign(InputIterator first, InputIterator last, size_type n);
    template<class InputIterator>
    UWR_FORCEINLINE constexpr iterator priv_copy_insert(const_iterator pos, InputIterator first, InputIterator last, size_type n);

    template<class ResizeProxy>
    constexpr void priv_resize(ResizeProxy&& proxy);
    template<class AssignProxy>
    constexpr void priv_assign(AssignProxy&& proxy);
    template<class InsertProxy>
    constexpr iterator priv_insert(const_iterator pos, InsertProxy&& proxy);
    constexpr void priv_swap(static_vector_alt& shorter, static_vector_alt& longer,
            size_type l_size) const;

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data[C];
    T* m_end;
};

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt() noexcept
    : m_end(this->data()) {}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(size_type n)
    : m_end(this->data() + n) {
    mem::construct(this->data(), this->m_end);
}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(size_type n, const T& val)
    : m_end(mem::ufill(this->data(), n, val)) {}

template<class T, len_t C>
template<class InputIterator, class>
constexpr
static_vector_alt<T, C>::static_vector_alt(InputIterator first, InputIterator last)
    : m_end(mem::ucopy(this->data(), first, last)) {}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(const static_vector_alt& x)
    : m_end(mem::ucopy(this->data(), x.begin(), x.end())) {}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(static_vector_alt&& x) noexcept
    : m_end(mem::umove(this->data(), x.begin(), x.end())) {}

template<class T, len_t C>
constexpr
static_vector_alt<T, C>::static_vector_alt(std::initializer_list<T> ilist)
    : m_end(mem::ucopy(this->data(), ilist.begin(), ilist.size())) {
}

template<class T, len_t C>
#if CPP_ABOVE_17
constexpr
#endif
static_vector_alt<T, C>::~static_vector_alt() {
    mem::destroy(this->data(), this->m_end);
}

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(const static_vector_alt<T, C>& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_copy_assign(other.begin(), other.end(), other.size());
    return *this;
}

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(static_vector_alt<T, C>&& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_move_assign(other.begin(), other.end(), other.size());
    return *this;
}

template<class T, len_t C>
constexpr static_vector_alt<T, C>&
static_vector_alt<T, C>::operator=(std::initializer_list<T> ilist) noexcept {
    this->assign(ilist.begin(), ilist.end());
    return *this;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::begin() noexcept {
    return iterator(this->data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::begin() const noexcept {
    return this->cbegin();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::end() noexcept {
    return iterator(this->m_end);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::end() const noexcept {
    return this->cend();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reverse_iterator
static_vector_alt<T, C>::rbegin() noexcept {
    return reverse_iterator(this->end());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reverse_iterator
static_vector_alt<T, C>::rbegin() const noexcept {
    return this->crbegin();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reverse_iterator
static_vector_alt<T, C>::rend() noexcept {
    return reverse_iterator(this->begin());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reverse_iterator
static_vector_alt<T, C>::rend() const noexcept {
    return this->crend();
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_iterator
static_vector_alt<T, C>::cbegin() const noexcept {
    return const_iterator(this->data());
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_iterator
constexpr static_vector_alt<T, C>::cend() const noexcept {
    return const_iterator(this->m_end);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reverse_iterator
constexpr static_vector_alt<T, C>::crbegin() const noexcept {
    return const_reverse_iterator(this->end());
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reverse_iterator
constexpr static_vector_alt<T, C>::crend() const noexcept {
    return const_reverse_iterator(this->begin());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::size() const noexcept {
    return static_cast<size_type>(this->m_end - this->data());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::max_size() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::resize(size_type n) {
    this->priv_resize(default_construct_proxy<T>(n));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::resize(size_type n, const T& val) {
    this->priv_resize(uninitialized_fill_proxy(n, val));
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::size_type
static_vector_alt<T, C>::capacity() const noexcept {
    return C;
}

template<class T, len_t C>
constexpr bool
static_vector_alt<T, C>::empty() const noexcept {
    return this->data() == this->m_end;
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
    return *this->data_at(n);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reference
constexpr static_vector_alt<T, C>::operator[](size_type n) const {
    return *this->data_at(n);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::at(size_type n) {
    if (this->data() + n < this->m_end)
        return *this->data_at(n);
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reference
static_vector_alt<T, C>::at(size_type n) const {
    if (this->data() + n < this->m_end)
        return *this->data_at(n);
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::front() {
    return *this->data_at(0);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::const_reference
static_vector_alt<T, C>::front() const {
    return *this->data_at(0);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::reference
constexpr static_vector_alt<T, C>::back() {
    return *(this->m_end - 1);
}

template<class T, len_t C>
typename static_vector_alt<T, C>::const_reference
constexpr static_vector_alt<T, C>::back() const {
    return *(this->m_end - 1);
}

template<class T, len_t C>
constexpr T*
static_vector_alt<T, C>::data() noexcept {
    return this->data_at(0);
}

template<class T, len_t C>
constexpr const T*
static_vector_alt<T, C>::data() const noexcept {
    return this->data_at(0);
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr void
static_vector_alt<T, C>::assign(InputIterator first, InputIterator last) {
    this->priv_copy_assign(first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::assign(size_type n, const T& val) {
    this->priv_assign(assign_fill_proxy(n, val));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::assign(std::initializer_list<T> ilist) {
    this->priv_copy_assign(ilist.begin(), ilist.end(), ilist.size());
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::push_back(const_reference value) {
    this->emplace_back(value);
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::push_back(T&& value) {
    this->emplace_back(std::move(value));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::fast_push_back(T&& value) noexcept {
    this->fast_emplace_back(std::forward<T>(value));
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::pop_back() {
    mem::destroy_at(--this->m_end);
}

template<class T, len_t C>
constexpr bool
static_vector_alt<T, C>::safe_pop_back() noexcept {
    if (UWR_LIKELY(this->m_end != this->data())) {
        this->pop_back();
        return true;
    }
    else
        return false;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, const T& value) {
    return this->emplace(pos, value);
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, T&& value) {
    return this->emplace(pos, std::move(value));
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, size_type count, const T& value) {
    return this->priv_insert(pos, insert_fill_proxy(count, value));
}

template<class T, len_t C>
template<class InputIterator, class>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    return this->priv_copy_insert(pos, first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return this->priv_copy_insert(pos, ilist.begin(), ilist.end(), ilist.size());
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::erase(const_iterator pos) {
    T* const m_pos = const_cast<T* const>(pos);
    mem::shiftl(m_pos, m_pos + 1, this->m_end);
    this->pop_back();

    return m_pos;
}

template<class T, len_t C>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::erase(const_iterator first, const_iterator last) {
    T* const m_first = const_cast<T* const>(first);

    if (UWR_LIKELY(first != last)) {
        T* const new_end = mem::shiftl(m_first, last, this->m_end);
        mem::destroy(new_end, this->m_end);
        this->m_end = new_end;
    }

    return m_first;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::swap(static_vector_alt<T, C>& other) {
    size_type m_size = this->size();
    size_type o_size = other.size();

    if (m_size < o_size)
        this->priv_swap(*this, other, o_size);
    else
        this->priv_swap(other, *this, m_size);
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::clear() noexcept {
    mem::destroy(this->data(), this->m_end);
    this->m_end = this->data();
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::emplace(const_iterator pos, Args&&... args) {
    T* const m_pos = const_cast<T* const>(pos);

    if (m_pos != this->m_end) {
        mem::shiftr(m_pos + 1, m_pos, this->m_end);
        // this strange construction is caused by the fact
        // args can be either "proper" constructor arguments
        // or it can be an object of type T, if it is, we don't want to do
        // *position = T(args), because that would create unnecessary object,
        // instead we would like to write *position = args
        // note: this problem only happens when we use opeartor= (so we copy
        // into initialized memory), when we copy into unitialized memory
        // we have to call constructor in both situation so
        // T(std::forward<Args>(args)...) will do
        *m_pos = mem::create<T>(std::forward<Args>(args)...);
    }
    else
        new (m_pos) T(std::forward<Args>(args)...);

    ++this->m_end;

    return m_pos;
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::emplace_back(Args&&... args) {
    if (UWR_LIKELY(this->m_end != this->data_at(C)))
        return this->fast_emplace_back(std::forward<Args>(args)...);
    else
        throw std::out_of_range("Out of bounds");
}

template<class T, len_t C>
template<class... Args>
constexpr typename static_vector_alt<T, C>::reference
static_vector_alt<T, C>::fast_emplace_back(Args&&... args) noexcept {
    new (this->m_end) T(std::forward<Args>(args)...);
    return *this->m_end++;
}

template<class T, len_t C>
constexpr T*
static_vector_alt<T, C>::data_at(size_type n) noexcept {
    return reinterpret_cast<T*>(&this->m_data[n]);
}

template<class T, len_t C>
constexpr const T*
static_vector_alt<T, C>::data_at(size_type n) const noexcept {
    return reinterpret_cast<const T*>(&this->m_data[n]);
}

template<class T, len_t C>
template<class ResizeProxy>
constexpr void
static_vector_alt<T, C>::priv_resize(ResizeProxy&& proxy) {
    T* const new_end = this->data() + proxy.n;

    if (new_end > this->m_end)
        proxy.construct(this->m_end, new_end);
    else
        mem::destroy(new_end, this->m_end);

    this->m_end = new_end;
}

template<class T, len_t C>
template<class InputIterator>
constexpr void
static_vector_alt<T, C>::priv_copy_assign(InputIterator first, InputIterator last, size_type n) {
    this->priv_assign(copy_assign_range_proxy<T, InputIterator>(first, last, n));
}

template<class T, len_t C>
template<class InputIterator>
constexpr void
static_vector_alt<T, C>::priv_move_assign(InputIterator first, InputIterator last, size_type n) {
    this->priv_assign(move_assign_range_proxy<T, InputIterator>(first, last, n));
}

template<class T, len_t C>
template<class AssignProxy>
constexpr void
static_vector_alt<T, C>::priv_assign(AssignProxy&& proxy) {
    T* const m_data = this->data();
    T* const new_end = m_data + proxy.n;
    size_type m_size = this->size();

    if (new_end > this->m_end)
        proxy.assign_to_short(m_data, m_size);
    else
        proxy.assign_to_long(m_data, m_size);

    this->m_end = new_end;
}

template<class T, len_t C>
template<class InputIterator>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::priv_copy_insert(const_iterator pos, InputIterator first, InputIterator last, size_type n) {
    return this->priv_insert(pos, insert_copy_range_proxy<T, InputIterator>(first, last, n));
}

template<class T, len_t C>
template<class InsertProxy>
constexpr typename static_vector_alt<T, C>::iterator
static_vector_alt<T, C>::priv_insert(const_iterator pos, InsertProxy&& proxy) {
    T* const m_pos = const_cast<T* const>(pos);

    if (UWR_UNLIKELY(!proxy.count))
        return m_pos;

    T* const spill = m_pos + proxy.count;

    if (spill < this->m_end) {
        mem::shiftr(spill, m_pos, this->m_end);
        proxy.insert_without_spill(m_pos, spill);
        this->m_end += proxy.count;
    }
    else {
        T* const new_end = mem::umove(spill, m_pos, this->m_end);
        proxy.insert_with_spill(m_pos, this->m_end, spill);
        this->m_end = new_end;
    }

    return m_pos;
}

template<class T, len_t C>
constexpr void
static_vector_alt<T, C>::priv_swap(
        static_vector_alt<T, C>& shorter,
        static_vector_alt<T, C>& longer,
        size_type l_len) const {
    T* const s_begin = shorter.begin();
    T* const l_begin = longer.begin();

    T* const l_split = std::swap_ranges(s_begin, shorter.m_end, l_begin);
    mem::umove(shorter.m_end, l_split, longer.m_end);
    mem::destroy(l_split, longer.m_end);

    shorter.m_end = s_begin + l_len;
    longer.m_end = l_split;
}


/*
 * non-member operators 
 */
template<class T, len_t C>
UWR_FORCEINLINE constexpr bool operator==(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
UWR_FORCEINLINE constexpr bool operator!=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr bool operator<(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
UWR_FORCEINLINE constexpr bool operator<=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
UWR_FORCEINLINE constexpr bool operator>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
UWR_FORCEINLINE constexpr bool operator>=(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);
template<class T, len_t C>
constexpr std::ostream& operator<<(std::ostream& out, const static_vector_alt<T, C>& v);

#if CPP_ABOVE_17

template<class T, len_t C>
constexpr auto operator<=>(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs);

#endif


/*
 * non-member operators' implementations
 */
template<class T, len_t C>
constexpr bool
operator==(const static_vector_alt<T, C>& lhs, const static_vector_alt<T, C>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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


/*
 * non-member functions
 */
namespace std {

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


/*
 * non-member functions' implementations
 */
template<class T, uwr::len_t C>
constexpr void
swap(uwr::static_vector_alt<T, C>& x, uwr::static_vector_alt<T, C>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, uwr::len_t C, class U>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase(uwr::static_vector_alt<T, C>& c, const U& value) {
    using size_type = typename uwr::static_vector_alt<T, C>::size_type;

    T* const c_end = c.end();
    T* const mid = std::remove(c.begin(), c_end, value);
    c.erase(mid, c_end);

    return static_cast<size_type>(std::distance(mid, c_end));
}

template<class T, uwr::len_t C, class Pred>
constexpr typename uwr::static_vector_alt<T, C>::size_type
erase_if(uwr::static_vector_alt<T, C>& c, Pred pred) {
    using size_type = typename uwr::static_vector_alt<T, C>::size_type;

    T* const c_end = c.end();
    T* const mid = std::remove_if(c.begin(), c_end, pred);
    c.erase(mid, c_end);

    return static_cast<size_type>(std::distance(mid, c_end));
}

#endif // CPP_ABOVE_17

} // namespace std
