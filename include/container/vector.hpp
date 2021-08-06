#pragma once

#include <iterator>

#include "../common/allocator.hpp"
#include "../common/define.hpp"
#include "../common/proxy.hpp"
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
    template<class InputIterator, class = typename std::iterator_traits<InputIterator>::value_type>
    constexpr vector(InputIterator first, InputIterator last);
    constexpr vector(const vector& x);
    constexpr vector(vector&& x) noexcept;
    constexpr vector(std::initializer_list<T> ilist);
    #if CPP_ABOVE_17
    constexpr
    #endif
    ~vector();

    UWR_FORCEINLINE constexpr vector& operator=(const vector& other) noexcept;
    UWR_FORCEINLINE constexpr vector& operator=(vector&& other) noexcept;
    UWR_FORCEINLINE constexpr vector& operator=(std::initializer_list<T> ilist) noexcept;

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

    UWR_FORCEINLINE constexpr void swap(vector& other);
    UWR_FORCEINLINE constexpr void clear() noexcept;

    template<class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template<class... Args>
    constexpr reference emplace_back(Args&&... args);
    template<class... Args>
    UWR_FORCEINLINE constexpr reference fast_emplace_back(Args&&... args) noexcept;

private:
    UWR_FORCEINLINE constexpr size_type next_capacity(size_type add) const noexcept;
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
      m_capacity(mem::fix_capacity<T>(this->m_length)),
      m_data(mem::allocate<T>(this->m_capacity)) {
    mem::construct(this->m_data, n);
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(size_type n, const T& val)
    : m_length(n),
      m_capacity(mem::fix_capacity<T>(n)),
      m_data(mem::allocate<T>(this->m_capacity)) {
    mem::ufill(this->m_data, n, val);
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr
vector<T, size_t>::vector(InputIterator first, InputIterator last)
    : m_length(std::distance(first, last)),
      m_capacity(mem::fix_capacity<T>(this->m_length)),
      m_data(mem::allocate<T>(this->m_capacity)) {
    mem::ucopy(this->m_data, first, last);
}

template<class T, class size_t>
constexpr
vector<T, size_t>::vector(const vector& x)
    : m_length(x.m_length),
      m_capacity(mem::fix_capacity<T>(this->m_length)),
      m_data(mem::allocate<T>(this->m_capacity)) {
    mem::ucopy(this->m_data, x.begin(), x.end());
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
      m_capacity(mem::fix_capacity<T>(this->m_length)),
      m_data(mem::allocate<T>(this->m_capacity)) {
    mem::ucopy(this->m_data, ilist.begin(), ilist.end());
}

template<class T, class size_t>
#if CPP_ABOVE_17
constexpr
#endif
vector<T, size_t>::~vector() {
    mem::destroy(this->m_data, this->m_length);
    mem::deallocate(this->m_data, this->m_capacity);
}

template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(const vector<T, size_t>& other) noexcept {
    if (UWR_LIKELY(this != &other))
        this->priv_copy_assign(other.begin(), other.end(), other.m_length);
    return *this;
}

// TODO: maybe deallocate moved
template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(vector<T, size_t>&& other) noexcept {
    std::swap(m_data, other.m_data);
    std::swap(m_length, other.m_length);
    std::swap(m_capacity, other.m_capacity);

    return *this;
}

template<class T, class size_t>
constexpr
vector<T, size_t>& vector<T, size_t>::operator=(std::initializer_list<T> ilist) noexcept {
    this->priv_copy_assign(ilist.begin(), ilist.end(), ilist.size());
    return *this;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::begin() noexcept {
    return this->m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::begin() const noexcept {
    return this->m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::end() noexcept {
    return this->m_data + this->m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::end() const noexcept {
    return this->m_data + this->m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reverse_iterator
vector<T, size_t>::rbegin() noexcept {
    return reverse_iterator(this->m_data + this->m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::rbegin() const noexcept {
    return const_reverse_iterator(this->m_data + this->m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reverse_iterator
vector<T, size_t>::rend() noexcept {
    return reverse_iterator(this->m_data);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::rend() const noexcept {
    return const_reverse_iterator(this->m_data);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::cbegin() const noexcept {
    return this->m_data;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_iterator
vector<T, size_t>::cend() const noexcept {
    return this->m_data + this->m_length;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::crbegin() const noexcept {
    return const_reverse_iterator(this->m_data + this->m_length);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reverse_iterator
vector<T, size_t>::crend() const noexcept {
    return const_reverse_iterator(this->m_data);
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::size() const noexcept {
    return this->m_length;
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(T);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::resize(size_type n) {
    this->priv_resize(default_construct_proxy<T>(n));
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::resize(size_type n, const T& val) {
    this->priv_resize(unitialized_fill_proxy(n, val));
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::capacity() const noexcept {
    return this->m_capacity;
}

template<class T, class size_t>
constexpr bool
vector<T, size_t>::empty() const noexcept {
    return this->m_length == 0;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::reserve(size_type n) noexcept {
    // TODO: likely here (?)
    if (n > this->m_capacity) {
        size_type new_capacity = mem::fix_capacity<T>(n);
        mem::reallocate(this->m_data, this->m_length, this->m_capacity, new_capacity);
        this->m_capacity = new_capacity;
    }
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::shrink_to_fit() noexcept {
    // TODO: now we don't shrink, maybe in the future
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::operator[](size_type n) {
    return this->m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::operator[](size_type n) const {
    return this->m_data[n];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::at(size_type n) {
    if (UWR_LIKELY(n < this->m_length))
        return this->m_data[n];
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::at(size_type n) const {
    if (UWR_LIKELY(n < this->m_length))
        return this->m_data[n];
    else
        throw std::out_of_range("Index out of range: " + std::to_string(n));
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::front() {
    return this->m_data[0];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::front() const {
    return this->m_data[0];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::back() {
    return this->m_data[this->m_length - 1];
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::const_reference
vector<T, size_t>::back() const {
    return this->m_data[this->m_length - 1];
}

template<class T, class size_t>
constexpr T*
vector<T, size_t>::data() noexcept {
    return this->m_data;
}

template<class T, class size_t>
constexpr const T*
vector<T, size_t>::data() const noexcept {
    return this->m_data;
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr void
vector<T, size_t>::assign(InputIterator first, InputIterator last) {
    this->priv_copy_assign(first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::assign(size_type n, const T& val) {
    this->priv_assign(assign_fill_proxy(n, val));
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::assign(std::initializer_list<T> ilist) {
    this->priv_copy_assign(ilist.begin(), ilist.end(), ilist.size());
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::push_back(const_reference value) {
    this->emplace_back(value);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::push_back(T&& value) {
    this->emplace_back(std::move(value));
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::fast_push_back(T&& value) noexcept {
    this->fast_emplace_back(std::forward<T>(value));
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::pop_back() {
    mem::destroy_at(this->data() + --this->m_length);
}

template<class T, class size_t>
constexpr bool
vector<T, size_t>::safe_pop_back() noexcept {
    if (UWR_LIKELY(this->m_length != 0)) {
        this->pop_back();
        return true;
    }
    else
        return false;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, const T& value) {
    return this->emplace(pos, value);
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, T&& value) {
    return this->emplace(pos, std::move(value));
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, size_type count, const T& value) {
    return this->priv_insert(pos, insert_fill_proxy(count, value));
}

template<class T, class size_t>
template<class InputIterator, class>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, InputIterator first, InputIterator last) {
    return this->priv_copy_insert(pos, first, last,
            static_cast<size_type>(std::distance(first, last)));
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return this->priv_copy_insert(pos, ilist.begin(), ilist.end(), ilist.size());
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::erase(const_iterator pos) {
    T* const position = const_cast<T* const>(pos);
    mem::shiftl(position, position + 1, this->end());
    this->pop_back();

    return position;
}

template<class T, class size_t>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::erase(const_iterator first, const_iterator last) {
    T* const f = const_cast<T* const>(first);
    size_type count = static_cast<size_type>(std::distance(first, last));

    if (UWR_LIKELY(count != 0)) {
        T* const l = const_cast<T* const>(last);
        T* const e = this->end();

        mem::shiftl(f, l, e);
        mem::destroy(e - count, e);
        this->m_length -= count;
    }

    return f;
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::swap(vector<T, size_t>& other) {
    std::swap(m_data, other.m_data);
    std::swap(m_length, other.m_length);
    std::swap(m_capacity, other.m_capacity);
}

template<class T, class size_t>
constexpr void
vector<T, size_t>::clear() noexcept {
    mem::destroy(this->m_data, this->m_length);
    this->m_length = 0;
}

template<class T, class size_t>
template<class... Args>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::emplace(const_iterator pos, Args&&... args) {
    T* const m_end = this->end();

    if (this->m_length == this->m_capacity) {
        size_type new_capacity = this->next_capacity(1);

        T* new_data;
        if (mem::expand_in_place_or_alloc_raw(
                    this->m_data, this->m_capacity,
                    new_capacity, new_data)) {
            T* const position = const_cast<T* const>(pos);
            
            if (position != m_end) {
                mem::shiftr(position + 1, position, m_end);
                *position = mem::create<T>(std::forward<Args>(args)...);
            }
            else
                new (position) T(std::forward<Args>(args)...);

            ++this->m_length;
            this->m_capacity = new_capacity;

            return position;
        }
        else {
            T* const e_begin = new_data + (pos - this->m_data);

            mem::umove<T, const T*>(new_data, this->m_data, pos);
            mem::umove<T, const T*>(e_begin + 1, pos, m_end);
            new (e_begin) T(std::forward<Args>(args)...);
            
            mem::destroy(this->m_data, m_end);
            mem::deallocate(this->m_data, this->m_capacity);

            ++this->m_length;
            this->m_capacity = new_capacity;
            this->m_data = new_data;

            return e_begin;
        }
    }
    else {
        T* const position = const_cast<T* const>(pos);
        
        if (position != m_end) {
            mem::shiftr(position + 1, position, m_end);
            *position = mem::create<T>(std::forward<Args>(args)...);
        }
        else
            new (position) T(std::forward<Args>(args)...);

        ++this->m_length;

        return position;
    }
}

template<class T, class size_t>
template<class... Args>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::emplace_back(Args&&... args) {
    // TODO: add unlikely (?)
    if (this->m_length == this->m_capacity) {
        size_type new_capacity = this->next_capacity(1);
        mem::reallocate(this->m_data, this->m_length, this->m_capacity, new_capacity);
        this->m_capacity = new_capacity;
    }
    return this->fast_emplace_back(std::forward<Args>(args)...);
}

template<class T, class size_t>
template<class... Args>
constexpr typename vector<T, size_t>::reference
vector<T, size_t>::fast_emplace_back(Args&&... args) noexcept {
    T* const last_end = this->m_data + this->m_length++;
    new (last_end) T(std::forward<Args>(args)...);
    return *last_end;
}

template<class T, class size_t>
constexpr size_t
vector<T, size_t>::next_capacity(size_type add) const noexcept {
    // constexpr int num = 2;
    // constexpr int den = 1;
    // return mem::fix_capacity<T>(std::max(2 * this->m_capacity, this->m_capacity + add));
    // return std::max(2 * this->m_capacity, this->m_capacity + add);
    return mem::fix_capacity<T>(this->m_capacity * 2 + add);
    // return mem::fix_capacity<T>(std::max(num * this->m_capacity / den, this->m_capacity + add));
}

template<class T, class size_t>
template<class InputIterator>
constexpr void
vector<T, size_t>::priv_copy_assign(InputIterator first, InputIterator last, size_type n) {
    this->priv_assign(copy_assign_range_proxy<T, InputIterator>(first, last, n));
}

template<class T, class size_t>
template<class InputIterator>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::priv_copy_insert(const_iterator pos, InputIterator first, InputIterator last, size_type n) {
    return this->priv_insert(pos, insert_copy_range_proxy<T, InputIterator>(first, last, n));
}

template<class T, class size_t>
template<class AssignProxy>
constexpr void
vector<T, size_t>::priv_assign(AssignProxy&& proxy) {
    if (proxy.n > this->m_capacity) {
        size_type new_capacity = mem::fix_capacity<T>(proxy.n);
        
        if (mem::expand_in_place_or_destroy_and_alloc_raw(
                    this->m_data, this->m_length,
                    this->m_capacity, new_capacity))
            proxy.shorter_assign(this->m_data, this->end(), this->m_length);
        else
            proxy.shorter_assign(this->m_data, this->m_data, 0);

        this->m_capacity = new_capacity;
    }
    else {
        T* const m_end = this->end();
        if (proxy.n > this->m_length)
            proxy.shorter_assign(this->m_data, m_end, this->m_length);
        else
            proxy.longer_assign(this->m_data, m_end, this->m_length);
    }

    this->m_length = proxy.n;
}

template<class T, class size_t>
template<class ResizeProxy>
constexpr void
vector<T, size_t>::priv_resize(ResizeProxy&& proxy) {
    if (proxy.n > this->m_capacity) {
        size_type new_capacity = mem::fix_capacity<T>(proxy.n);
        
        mem::reallocate(this->m_data, this->m_length, this->m_capacity, new_capacity);
        proxy.construct(this->end(), this->m_data + proxy.n);

        this->m_capacity = new_capacity;
    }
    else {
        T* const m_end = this->end();
        T* const new_end = this->m_data + proxy.n;

        if (proxy.n > this->m_length)
            proxy.construct(m_end, new_end);
        else
            mem::destroy(new_end, m_end);
    }

    this->m_length = proxy.n;
}

template<class T, class size_t>
template<class InsertProxy>
constexpr typename vector<T, size_t>::iterator
vector<T, size_t>::priv_insert(const_iterator pos, InsertProxy&& proxy) {
    T* const position = const_cast<T* const>(pos);

    if (UWR_UNLIKELY(!proxy.count))
        return position;

    T* const m_end = this->end();
    size_type new_length = this->m_length + proxy.count;

    if (new_length > this->m_capacity) {
        size_type new_capacity = mem::fix_capacity<T>(new_length);

        T* new_data;
        if (mem::expand_in_place_or_alloc_raw(
                    this->m_data, this->m_capacity,
                    new_capacity, new_data)) {
            T* const spill = position + proxy.count;

            if (spill < m_end) {
                mem::shiftr(spill, position, m_end);
                proxy.insert_without_spill(position, spill);
            }
            else {
                mem::umove(spill, position, m_end);
                proxy.insert_with_spill(position, m_end, spill);
            }

            this->m_capacity = new_capacity;
            this->m_length = new_length;

            return position;
        }
        else {
            T* const i_begin = new_data + (position - this->m_data);
            T* const i_end = i_begin + proxy.count;

            mem::umove<T, const T*>(new_data, this->m_data, pos);
            mem::umove<T, const T*>(i_end, pos, m_end);
            proxy.insert_with_spill(i_begin, i_begin, i_end);

            mem::destroy(this->m_data, m_end);
            mem::deallocate(this->m_data, this->m_capacity);

            this->m_data = new_data;
            this->m_capacity = new_capacity;
            this->m_length = new_length;
            
            return i_begin;
        }
    }
    else {
        T* const spill = position + proxy.count;

        if (spill < m_end) {
            mem::shiftr(spill, position, m_end);
            proxy.insert_without_spill(position, spill);
        }
        else {
            mem::umove(spill, position, m_end);
            proxy.insert_with_spill(position, m_end, spill);
        }

        this->m_length = new_length;

        return position;
    }
}


/*
 * non-member operators 
 */
template<class T, class size_t>
UWR_FORCEINLINE constexpr bool operator==(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
UWR_FORCEINLINE constexpr bool operator!=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr bool operator<(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
UWR_FORCEINLINE constexpr bool operator<=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
UWR_FORCEINLINE constexpr bool operator>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
UWR_FORCEINLINE constexpr bool operator>=(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);
template<class T, class size_t>
constexpr std::ostream& operator<<(std::ostream& out, const vector<T, size_t>& v);

#if CPP_ABOVE_17

template<class T, class size_t>
constexpr auto operator<=>(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs);

#endif


/*
 * non-member operators' implementations
 */
template<class T, class size_t>
constexpr bool
operator==(const vector<T, size_t>& lhs, const vector<T, size_t>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
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


/*
 * non-member functions
 */
namespace std {

template<class T, class size_t>
constexpr void swap(uwr::vector<T, size_t>& x, uwr::vector<T, size_t>& y);

#if CPP_ABOVE_17

template<class T, class size_t, class U>
constexpr size_t erase(uwr::vector<T, size_t>& c, const U& value);
template<class T, class size_t, class Pred>
constexpr size_t erase_if(uwr::vector<T, size_t>& c, Pred pred);

#endif


/*
 * non-member functions' implementations
 */
template<class T, class size_t>
constexpr void
swap(uwr::vector<T, size_t>& x, uwr::vector<T, size_t>& y) {
    x.swap(y);
}

#if CPP_ABOVE_17

template<class T, class size_t, class U>
constexpr size_t
erase(uwr::vector<T, size_t>& c, const U& value) {
    T* const c_end = c.end();
    T* const mid = std::remove(c.begin(), c_end, value);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

template<class T, class size_t, class Pred>
constexpr size_t
erase_if(uwr::vector<T, size_t>& c, Pred pred) {
    T* const c_end = c.end();
    T* const mid = std::remove_if(c.begin(), c_end, pred);
    c.erase(mid, c_end);

    return static_cast<size_t>(std::distance(mid, c_end));
}

#endif // CPP_ABOVE_17

} // namespace uwr
