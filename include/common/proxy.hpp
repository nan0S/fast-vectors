#pragma once

#include "memory.hpp"

namespace uwr {

template<class T>
class default_construct_proxy {
public:
    UWR_FORCEINLINE default_construct_proxy(mem::len_t n)
        : n(n) {}

    UWR_FORCEINLINE void construct(T* begin, T* end) {
        mem::construct(begin, end);
    }

public:
    mem::len_t n;
};

template<class T>
class unitialized_fill_proxy {
public:
    UWR_FORCEINLINE explicit unitialized_fill_proxy(mem::len_t n, const T& value)
        : value(value), n(n) {}

    UWR_FORCEINLINE void construct(T* begin, T* end) {
        mem::ufill(begin, end, value);
    }

private:
    const T& value;

public:
    mem::len_t n;
};

template<class T, class InputIterator>
class copy_assign_range_proxy {
public:
    UWR_FORCEINLINE copy_assign_range_proxy(InputIterator first, InputIterator last, mem::len_t n)
        : first(first), last(last), n(n) {
        UWR_ASSERT(n == static_cast<mem::len_t>(std::distance(first, last)));
    }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len < this->n);

        InputIterator split = std::next(this->first, len);

        mem::copy(begin, this->first, split);
        mem::ucopy(end, split, this->last);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, UWR_UNUSED mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len >= this->n);

        mem::destroy(begin + this->n, end);
        mem::copy(begin, this->first, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    mem::len_t n;
};

template<class T, class InputIterator>
class move_assign_range_proxy {
public:
    UWR_FORCEINLINE move_assign_range_proxy(InputIterator first, InputIterator last, mem::len_t n)
        : first(first), last(last), n(n) {
        UWR_ASSERT(n == static_cast<mem::len_t>(std::distance(first, last)));
    }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len < this->n);

        InputIterator split = std::next(this->first, len);

        mem::move(begin, this->first, split);
        mem::umove(end, split, this->last);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len >= this->n);

        mem::destroy(begin + len, end);
        mem::move(begin, this->first, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    mem::len_t n;
};

template<class T>
class assign_fill_proxy {
public:
    UWR_FORCEINLINE assign_fill_proxy(mem::len_t n, const T& value)
        : value(value), n(n) { }

    UWR_FORCEINLINE void shorter_assign(T* begin, T* end, UWR_UNUSED mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len < n);

        mem::fill(begin, end, this->value);
        mem::ufill(end, begin + this->n, this->value);
    }

    UWR_FORCEINLINE void longer_assign(T* begin, T* end, UWR_UNUSED mem::len_t len) {
        UWR_ASSERT(std::distance(begin, end) == len);
        UWR_ASSERT(len >= this->n);

        T* const split = begin + this->n;

        mem::destroy(split, end);
        mem::fill(begin, split, this->value);
    }

private:
    const T& value;

public:
    mem::len_t n;
};

template<class T>
class insert_fill_proxy {
public:
    UWR_FORCEINLINE insert_fill_proxy(mem::len_t count, const T& value)
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
    mem::len_t count;
};

template<class T, class InputIterator>
class insert_copy_range_proxy {
public:
    UWR_FORCEINLINE insert_copy_range_proxy(InputIterator first, InputIterator last, mem::len_t count)
        : first(first), last(last), count(count) {
        UWR_ASSERT(count == static_cast<mem::len_t>(std::distance(first, last)));
    }

    UWR_FORCEINLINE void insert_without_spill(T* begin, T*) {
        mem::copy(begin, this->first, this->last);
    }

    UWR_FORCEINLINE void insert_with_spill(T* position, T* end, T*) {
        mem::len_t rest = static_cast<mem::len_t>(std::distance(position, end));
        InputIterator split = std::next(this->first, rest);

        mem::copy(position, this->first, split);
        mem::ucopy(end, split, this->last);
    }

private:
    InputIterator first;
    InputIterator last;

public:
    mem::len_t count;
};

} // namespace uwr
