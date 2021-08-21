#pragma once

#include <iterator>
#include <limits>

#include "../common/memory.hpp"
#include "../common/define.hpp"
#include "../common/proxy.hpp"
#if CPP_ABOVE_17
#include "../common/synth_three_way.hpp"
#endif
#include "../allocator/hybrid_allocator.hpp"

namespace uwr {

template<class T>
class simple_vector {
public:
    using value_type = T;
    using size_type = mem::len_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr simple_vector() noexcept;
    constexpr explicit simple_vector(size_type n);
    constexpr simple_vector(size_type n, const T& val);
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr simple_vector(InputIterator first, InputIterator last);
    constexpr simple_vector(const simple_vector& x);
    constexpr simple_vector(simple_vector&& x) noexcept;
    constexpr simple_vector(std::initializer_list<T> ilist);
    #if CPP_ABOVE_17
    constexpr
    #endif
    ~simple_vector();

    UWR_FORCEINLINE constexpr simple_vector& operator=(const simple_vector& other) noexcept;
    UWR_FORCEINLINE constexpr simple_vector& operator=(simple_vector&& other) noexcept;
    UWR_FORCEINLINE constexpr simple_vector& operator=(std::initializer_list<T> ilist) noexcept;

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
    constexpr void reserve(size_type n) noexcept;
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

    UWR_FORCEINLINE constexpr void swap(simple_vector& other);
    UWR_FORCEINLINE constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    constexpr reference emplace_back(Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference fast_emplace_back(Args&&... args) noexcept;

private:
    UWR_FORCEINLINE constexpr size_type next_capacity(size_type new_size) const noexcept;
    template<class InputIterator>
    UWR_FORCEINLINE constexpr void priv_copy_assign(InputIterator first, InputIterator last, size_type n);
    template<class InputIterator>
    UWR_FORCEINLINE constexpr iterator priv_copy_insert(const_iterator pos, InputIterator first, InputIterator last, size_type n);

    template<class ResizeProxy>
    constexpr void priv_resize(ResizeProxy&& proxy);
    template<class AssignProxy>
    constexpr void priv_assign(AssignProxy&& proxy);
    template<class InsertProxy>
    constexpr iterator priv_insert(const_iterator pos, InsertProxy&& proxy);

    UWR_FORCEINLINE constexpr T* alloc(size_type n);
    UWR_FORCEINLINE constexpr void dealloc(T* data);
    constexpr void realloc(size_type n);

private:
    size_type m_size;
    size_type m_capacity;
    T* m_data;

public:
    static int reallocs;
};

template<class T>
int simple_vector<T>::reallocs = 0;

template<class T>
constexpr
simple_vector<T>::simple_vector() noexcept
    : m_size(0),
      m_capacity(0),
      m_data(nullptr) {}

template<class T>
constexpr
simple_vector<T>::simple_vector(size_type n)
    : m_size(n),
      m_capacity(this->m_size),
      m_data(this->alloc(this->m_capacity)) {
    mem::construct(this->m_data, this->m_size);
}

template<class T>
constexpr
simple_vector<T>::simple_vector(size_type n, const T& val)
    : m_size(n),
      m_capacity(this->m_size),
      m_data(this->alloc(this->m_capacity)) {
    mem::ufill(this->m_data, n, val);
}

template<class T>
template<class InputIterator, class>
constexpr
simple_vector<T>::simple_vector(InputIterator first, InputIterator last)
    : m_size(std::distance(first, last)),
      m_capacity(this->m_size),
      m_data(this->alloc(this->m_capacity)) {
    mem::ucopy(this->m_data, first, last);
}

template<class T>
constexpr
simple_vector<T>::simple_vector(const simple_vector& x)
    : m_size(x.size()),
      m_capacity(this->m_size),
      m_data(this->alloc(this->m_capacity)) {
    mem::ucopy(this->m_data, x.begin(), x.size());
}

template<class T>
constexpr
simple_vector<T>::simple_vector(simple_vector&& x) noexcept
    : m_size(x.m_size),
      m_capacity(x.m_capacity),
      m_data(x.m_data) {
    x.m_size = 0;
    x.m_capacity = 0;
    x.m_data = nullptr;
}

template<class T>
constexpr
simple_vector<T>::simple_vector(std::initializer_list<T> ilist)
    : m_size(ilist.size()),
      m_capacity(this->m_size),
      m_data(this->alloc(this->m_capacity)) {
    mem::ucopy(this->m_data, ilist.begin(), ilist.size());
}

template<class T>
#if CPP_ABOVE_17
constexpr
#endif
simple_vector<T>::~simple_vector() {
    mem::destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data);
}

template<class T>
constexpr
simple_vector<T>& simple_vector<T>::operator=(const simple_vector<T>& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_copy_assign(other.begin(), other.end(), other.size());
    return *this;
}

template<class T>
constexpr
simple_vector<T>& simple_vector<T>::operator=(simple_vector<T>&& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->swap(other);
    return *this;
}

template<class T>
constexpr
simple_vector<T>& simple_vector<T>::operator=(std::initializer_list<T> ilist) noexcept {
    this->priv_copy_assign(ilist.begin(), ilist.end(), ilist.size());
    return *this;
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::begin() noexcept {
    return iterator(this->m_data);
}

template<class T>
constexpr typename simple_vector<T>::const_iterator
simple_vector<T>::begin() const noexcept {
    return this->cbegin();
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::end() noexcept {
    return iterator(this->m_data + this->m_size);
}

template<class T>
constexpr typename simple_vector<T>::const_iterator
simple_vector<T>::end() const noexcept {
    return this->cend();
}

template<class T>
constexpr typename simple_vector<T>::reverse_iterator
simple_vector<T>::rbegin() noexcept {
    return reverse_iterator(this->end());
}

template<class T>
constexpr typename simple_vector<T>::const_reverse_iterator
simple_vector<T>::rbegin() const noexcept {
    return this->crbegin();
}

template<class T>
constexpr typename simple_vector<T>::reverse_iterator
simple_vector<T>::rend() noexcept {
    return reverse_iterator(this->begin());
}

template<class T>
constexpr typename simple_vector<T>::const_reverse_iterator
simple_vector<T>::rend() const noexcept {
    return this->crend();
}

template<class T>
constexpr typename simple_vector<T>::const_iterator
simple_vector<T>::cbegin() const noexcept {
    return const_iterator(this->m_data);
}

template<class T>
constexpr typename simple_vector<T>::const_iterator
simple_vector<T>::cend() const noexcept {
    return const_iterator(this->m_data + this->m_size);
}

template<class T>
constexpr typename simple_vector<T>::const_reverse_iterator
simple_vector<T>::crbegin() const noexcept {
    return const_reverse_iterator(this->end());
}

template<class T>
constexpr typename simple_vector<T>::const_reverse_iterator
simple_vector<T>::crend() const noexcept {
    return const_reverse_iterator(this->begin());
}

template<class T>
constexpr typename simple_vector<T>::size_type
simple_vector<T>::size() const noexcept {
    return this->m_size;
}

template<class T>
constexpr typename simple_vector<T>::size_type
simple_vector<T>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(T);
}

template<class T>
constexpr void
simple_vector<T>::resize(size_type n) {
    this->priv_resize(default_construct_proxy<T>(n));
}

template<class T>
constexpr void
simple_vector<T>::resize(size_type n, const T& val) {
    this->priv_resize(uninitialized_fill_proxy(n, val));
}

template<class T>
constexpr typename simple_vector<T>::size_type
simple_vector<T>::capacity() const noexcept {
    return this->m_capacity;
}

template<class T>
constexpr bool
simple_vector<T>::empty() const noexcept {
    return this->m_size == 0;
}

template<class T>
constexpr void
simple_vector<T>::reserve(size_type n) noexcept {
    // TODO: likely here (?)
    if (UWR_LIKELY(n > this->m_capacity))
        this->realloc(n);
}

template<class T>
constexpr void
simple_vector<T>::shrink_to_fit() noexcept {
    // TODO: now we don't shrink, maybe in the future
}

template<class T>
constexpr typename simple_vector<T>::reference
simple_vector<T>::operator[](size_type n) {
    return this->m_data[n];
}

template<class T>
constexpr typename simple_vector<T>::const_reference
simple_vector<T>::operator[](size_type n) const {
    return this->m_data[n];
}

template<class T>
constexpr typename simple_vector<T>::reference
simple_vector<T>::at(size_type n) {
    if (UWR_LIKELY(n < this->m_size))
        return this->m_data[n];
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T>
constexpr typename simple_vector<T>::const_reference
simple_vector<T>::at(size_type n) const {
    if (UWR_LIKELY(n < this->m_size))
        return this->m_data[n];
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T>
constexpr typename simple_vector<T>::reference
simple_vector<T>::front() {
    return *this->m_data;
}

template<class T>
constexpr typename simple_vector<T>::const_reference
simple_vector<T>::front() const {
    return *this->m_data;
}

template<class T>
constexpr typename simple_vector<T>::reference
simple_vector<T>::back() {
    return this->m_data[this->m_size - 1];
}

template<class T>
constexpr typename simple_vector<T>::const_reference
simple_vector<T>::back() const {
    return this->m_data[this->m_size - 1];
}

template<class T>
constexpr T*
simple_vector<T>::data() noexcept {
    return this->m_data;
}

template<class T>
constexpr const T*
simple_vector<T>::data() const noexcept {
    return this->m_data;
}

template<class T>
template<class InputIterator, class>
constexpr void
simple_vector<T>::assign(InputIterator first, InputIterator last) {
    this->priv_copy_assign(first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T>
constexpr void
simple_vector<T>::assign(size_type n, const T& val) {
    this->priv_assign(assign_fill_proxy(n, val));
}

template<class T>
constexpr void
simple_vector<T>::assign(std::initializer_list<T> ilist) {
    this->priv_copy_assign(ilist.begin(), ilist.end(), ilist.size());
}

template<class T>
constexpr void
simple_vector<T>::push_back(const_reference value) {
    this->emplace_back(value);
}

template<class T>
constexpr void
simple_vector<T>::push_back(T&& value) {
    this->emplace_back(std::move(value));
}

template<class T>
constexpr void
simple_vector<T>::fast_push_back(T&& value) noexcept {
    this->fast_emplace_back(std::forward<T>(value));
}

template<class T>
constexpr void
simple_vector<T>::pop_back() {
    mem::destroy_at(this->m_data + --this->m_size);
}

template<class T>
constexpr bool
simple_vector<T>::safe_pop_back() noexcept {
    if (UWR_LIKELY(this->m_size != 0)) {
        this->pop_back();
        return true;
    }
    else
        return false;
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::insert(const_iterator pos, const T& value) {
    return this->emplace(pos, value);
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::insert(const_iterator pos, T&& value) {
    return this->emplace(pos, std::move(value));
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::insert(const_iterator pos, size_type count, const T& value) {
    return this->priv_insert(pos, insert_fill_proxy(count, value));
}

template<class T>
template<class InputIterator, class>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    return this->priv_copy_insert(pos, first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return this->priv_copy_insert(pos, ilist.begin(), ilist.end(), ilist.size());
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::erase(const_iterator pos) {
    T* const m_pos = const_cast<T* const>(pos);
    mem::shiftl(m_pos, m_pos + 1, this->end());
    this->pop_back();

    return m_pos;
}

template<class T>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::erase(const_iterator first, const_iterator last) {
    T* const m_first = const_cast<T* const>(first);
    T* const m_last = const_cast<T* const>(last);

    if (UWR_LIKELY(m_first != m_last)) {
        T* const m_end = this->end();

        mem::destroy(
                mem::shiftl(m_first, m_last, m_end),
                m_end);

        this->m_size -= static_cast<size_type>(
                std::distance(m_first, m_last));
    }

    return m_first;
}

template<class T>
constexpr void
simple_vector<T>::swap(simple_vector<T>& other) {
    std::swap(this->m_size, other.m_size);
    std::swap(this->m_capacity, other.m_capacity);
    std::swap(this->m_data, other.m_data);
}

template<class T>
constexpr void
simple_vector<T>::clear() noexcept {
    mem::destroy(this->m_data, this->m_size);
    this->m_size = 0;
}

template<class T>
template<class... Args>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::emplace(const_iterator pos, Args&&... args) {
    T* const m_end = this->end();

    if (this->m_size == this->m_capacity) {
        size_type new_capacity = this->next_capacity(this->m_size + 1);

        T* const new_data = this->alloc(new_capacity);
        T* const e_begin = mem::umove(new_data, this->cbegin(), pos);
        mem::umove<T, const T*>(e_begin + 1, pos, m_end);
        mem::destroy(this->m_data, this->m_size);
        this->dealloc(this->m_data);

        new (e_begin) T(std::forward<Args>(args)...);

        this->m_data = new_data;
        this->m_capacity = new_capacity;
        ++this->m_size;

        return e_begin;
    }
    else {
        T* const m_pos = const_cast<T* const>(pos);
        
        if (m_pos != m_end) {
            mem::shiftr(m_pos + 1, m_pos, m_end);
            *m_pos = mem::create<T>(std::forward<Args>(args)...);
        }
        else
            new (m_pos) T(std::forward<Args>(args)...);

        ++this->m_size;

        return m_pos;
    }
}

template<class T>
template<class... Args>
constexpr typename simple_vector<T>::reference
simple_vector<T>::emplace_back(Args&&... args) {
    // TODO: add unlikely (?)
    if (UWR_UNLIKELY(this->m_size == this->m_capacity)) {
        size_type new_capacity = this->next_capacity(this->m_size + 1);
        this->realloc(new_capacity);
    }
    return this->fast_emplace_back(std::forward<Args>(args)...);
}

template<class T>
template<class... Args>
constexpr typename simple_vector<T>::reference
simple_vector<T>::fast_emplace_back(Args&&... args) noexcept {
    T* const last_end = this->m_data + this->m_size++;
    new (last_end) T(std::forward<Args>(args)...);
    return *last_end;
}

template<class T>
constexpr typename simple_vector<T>::size_type
simple_vector<T>::next_capacity(size_type new_size) const noexcept {
    return std::max(2 * this->m_capacity, new_size);
}

template<class T>
template<class InputIterator>
constexpr void
simple_vector<T>::priv_copy_assign(InputIterator first, InputIterator last, size_type n) {
    this->priv_assign(copy_assign_range_proxy<T, InputIterator>(first, last, n));
}

template<class T>
template<class InputIterator>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::priv_copy_insert(const_iterator pos, InputIterator first, InputIterator last, size_type n) {
    return this->priv_insert(pos, insert_copy_range_proxy<T, InputIterator>(first, last, n));
}

template<class T>
template<class AssignProxy>
constexpr void
simple_vector<T>::priv_assign(AssignProxy&& proxy) {
    if (proxy.n > this->m_capacity) {
        mem::destroy(this->m_data, this->m_size);
        this->dealloc(this->m_data);

        this->m_data = this->alloc(proxy.n);
        this->m_capacity = proxy.n;

        proxy.assign_to_short(this->m_data, 0);
    }
    else if (proxy.n > this->m_size)
        proxy.assign_to_short(this->m_data, this->m_size);
    else
        proxy.assign_to_long(this->m_data, this->m_size);

    this->m_size = proxy.n;
}

template<class T>
template<class ResizeProxy>
constexpr void
simple_vector<T>::priv_resize(ResizeProxy&& proxy) {
    if (proxy.n > this->m_capacity) {
        T* const new_data = this->alloc(proxy.n);
        T* const m_end = mem::umove(new_data, this->m_data, this->m_size);

        mem::destroy(this->m_data, this->m_size);
        this->dealloc(this->m_data);

        this->m_data = new_data;
        this->m_capacity = proxy.n;

        proxy.construct(m_end, new_data + proxy.n);
    }
    else {
        T* const m_end = this->end();
        T* const new_end = this->m_data + proxy.n;

        if (proxy.n > this->m_size)
            proxy.construct(m_end, new_end);
        else
            mem::destroy(new_end, m_end);
    }

    this->m_size = proxy.n;
}

template<class T>
template<class InsertProxy>
constexpr typename simple_vector<T>::iterator
simple_vector<T>::priv_insert(const_iterator pos, InsertProxy&& proxy) {
    T* const m_pos = const_cast<T* const>(pos);

    if (UWR_UNLIKELY(!proxy.count))
        return m_pos;

    T* const m_end = this->end();
    size_type new_size = this->m_size + proxy.count;

    if (new_size > this->m_capacity) {
        size_type new_capacity = this->next_capacity(new_size);
        T* new_data = this->alloc(new_capacity);

        T* const i_begin = mem::umove(new_data, this->m_data, m_pos);
        T* const i_end = i_begin + proxy.count;
        mem::umove(i_end, m_pos, m_end);

        mem::destroy(this->m_data, this->m_size);
        this->dealloc(this->m_data);

        this->m_data = new_data;
        this->m_capacity = new_capacity;
        this->m_size = new_size;

        proxy.insert_with_spill(i_begin, i_begin, i_end);

        return i_begin;
    }
    else {
        T* const spill = m_pos + proxy.count;

        if (spill < m_end) {
            mem::shiftr(spill, m_pos, m_end);
            proxy.insert_without_spill(m_pos, spill);
        }
        else {
            mem::umove(spill, m_pos, m_end);
            proxy.insert_with_spill(m_pos, m_end, spill);
        }

        this->m_size = new_size;

        return m_pos;
    }
}

template<class T>
constexpr T*
simple_vector<T>::alloc(size_type n) {
    return static_cast<T*>(operator new (n * sizeof(T)));
}

template<class T>
constexpr void
simple_vector<T>::dealloc(T* data) {
    operator delete (data);
}

template<class T>
constexpr void
simple_vector<T>::realloc(size_type n) {
    T* new_data = this->alloc(n);
    mem::umove(new_data, this->m_data, this->m_size);
    mem::destroy(this->m_data, this->m_size);
    this->dealloc(this->m_data);

    this->m_data = new_data;
    this->m_capacity = n;
}


/*
 * non-member operators 
 */
template<class T>
UWR_FORCEINLINE constexpr bool operator==(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
UWR_FORCEINLINE constexpr bool operator!=(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
constexpr bool operator<(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
UWR_FORCEINLINE constexpr bool operator<=(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
UWR_FORCEINLINE constexpr bool operator>(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
UWR_FORCEINLINE constexpr bool operator>=(const simple_vector<T>& lhs, const simple_vector<T>& rhs);
template<class T>
constexpr std::ostream& operator<<(std::ostream& out, const simple_vector<T>& v);

#if CPP_ABOVE_17

template<class T>
constexpr auto operator<=>(const simple_vector<T>& lhs, const simple_vector<T>& rhs);

#endif


/*
 * non-member operators' implementations
 */
template<class T>
constexpr bool
operator==(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T>
constexpr bool
operator!=(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return !(lhs == rhs);
}

template<class T>
constexpr bool
operator<(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
constexpr bool
operator<=(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return !(rhs < lhs);
}

template<class T>
constexpr bool
operator>(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return rhs < lhs;
}

template<class T>
constexpr bool
operator>=(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
    return !(lhs < rhs);
}

template<class T>
constexpr std::ostream&
operator<<(std::ostream& out, const simple_vector<T>& v) {
    for (typename simple_vector<T>::size_type i = 0; i < v.size() - 1; ++i)
        out << v[i] << ' ';
    if (!v.empty())
        out << v.back();
    return out;
}

#if CPP_ABOVE_17

template<class T>
constexpr auto
operator<=>(const simple_vector<T>& lhs, const simple_vector<T>& rhs) {
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

template<class T>
constexpr void swap(uwr::simple_vector<T>& x, uwr::simple_vector<T>& y);

#if CPP_ABOVE_17

template<class T, class U>
constexpr typename uwr::simple_vector<T>::size_type
erase(uwr::simple_vector<T>& c, const U& value);
template<class T, class Pred>
constexpr typename uwr::simple_vector<T>::size_type
erase_if(uwr::simple_vector<T>& c, Pred pred);

#endif


/*
 * non-member functions' implementations
 */
template<class T>
constexpr void
swap(uwr::simple_vector<T>& x, uwr::simple_vector<T>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, class U>
constexpr typename uwr::simple_vector<T>::size_type
erase(uwr::simple_vector<T>& c, const U& value) {
    T* const c_end = c.end();
    T* const mid = std::remove(c.begin(), c_end, value);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

template<class T, class Pred>
constexpr typename uwr::simple_vector<T>::size_type
erase_if(uwr::simple_vector<T>& c, Pred pred) {
    T* const c_end = c.end();
    T* const mid = std::remove_if(c.begin(), c_end, pred);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

#endif // CPP_ABOVE_17

} // namespace uwr
