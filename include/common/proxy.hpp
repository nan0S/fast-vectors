#pragma once

#include "memory.hpp"

namespace uwr {

template<class T>
class default_construct_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE default_construct_proxy(size_type n)
        : n(n) {}

    UWR_FORCEINLINE void construct(T* begin, T* end) {
        mem::construct(begin, end);
    }

public:
    size_type n;
};

template<class T>
class unitialized_fill_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE explicit unitialized_fill_proxy(size_type n, const T& value)
        : value(value), n(n) {}

    UWR_FORCEINLINE void construct(T* begin, T* end) {
        mem::ufill(begin, end, value);
    }

private:
    const T& value;

public:
    size_type n;
};

template<class T, class InputIterator>
class copy_assign_range_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE copy_assign_range_proxy(InputIterator first, InputIterator last, size_type n)
        : first(first), last(last), n(n) {
        UWR_ASSERT(this->n == static_cast<size_type>(
                    std::distance(this->first, this->last)));
    }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, size_type len) {
        UWR_ASSERT(len < this->n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        InputIterator split = std::next(this->first, len);

        mem::copy(begin, this->first, split);
        mem::ucopy(end, split, this->last);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, UWR_UNUSED size_type len) {
        UWR_ASSERT(len >= this->n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        mem::destroy(begin + this->n, end);
        mem::copy(begin, this->first, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    size_type n;
};

template<class T, class InputIterator>
class move_assign_range_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE move_assign_range_proxy(InputIterator first, InputIterator last, size_type n)
        : first(first), last(last), n(n) {
        UWR_ASSERT(this->n == static_cast<size_type>(
                    std::distance(this->first, this->last)));
    }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, size_type len) {
        UWR_ASSERT(len < this->n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        InputIterator split = std::next(this->first, len);

        mem::move(begin, this->first, split);
        mem::umove(end, split, this->last);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, size_type len) {
        UWR_ASSERT(len >= this->n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        mem::destroy(begin + len, end);
        mem::move(begin, this->first, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    size_type n;
};

template<class T>
class assign_fill_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE assign_fill_proxy(size_type n, const T& value)
        : value(value), n(n) { }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, UWR_UNUSED size_type len) {
        UWR_ASSERT(len < n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        mem::fill(begin, end, this->value);
        mem::ufill(end, begin + this->n, this->value);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, UWR_UNUSED size_type len) {
        UWR_ASSERT(len >= this->n);
        UWR_ASSERT(len == static_cast<size_type>(
                    std::distance(begin, end)));

        T* const split = begin + this->n;

        mem::destroy(split, end);
        mem::fill(begin, split, this->value);
    }

private:
    const T& value;

public:
    size_type n;
};

template<class T>
class insert_fill_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE insert_fill_proxy(size_type count, const T& value)
        : value(value), count(count) {}

    UWR_FORCEINLINE void insert_without_spill(T* begin, T* end) {
        mem::fill(begin, end, this->value);
    }

    UWR_FORCEINLINE void insert_with_spill(T* position, T* end, T* spill) {
        mem::fill(position, end, this->value);
        mem::ufill(end, spill, this->value);
    }

private:
    const T& value;

public:
    size_type count;
};

template<class T, class InputIterator>
class insert_copy_range_proxy {
private:
    using size_type = mem::len_t;

public:
    UWR_FORCEINLINE insert_copy_range_proxy(InputIterator first, InputIterator last, size_type count)
        : first(first), last(last), count(count) {
        UWR_ASSERT(this->count == static_cast<size_type>(
                    std::distance(this->first, this->last)));
    }

    UWR_FORCEINLINE void insert_without_spill(T* begin, T*) {
        mem::copy(begin, this->first, this->last);
    }

    UWR_FORCEINLINE void insert_with_spill(T* position, T* end, T*) {
        size_type rest = static_cast<size_type>(
                std::distance(position, end));
        InputIterator split = std::next(this->first, rest);

        mem::copy(position, this->first, split);
        mem::ucopy(end, split, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    size_type count;
};

} // namespace uwr
