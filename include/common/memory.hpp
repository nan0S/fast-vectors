#pragma once

#include "trivial.hpp"
#include <algorithm>
#include <cstring>

namespace uwr::mem {

typedef unsigned int uint;
// TODO: restore to uint
// using len_t = uint;
using len_t = std::size_t;

template<class T>
void construct(T* begin, T* end);
template<class T>
void construct(T* begin, len_t n);

/* regular fill */
template<class T>
void fill(T* begin, T* end, const T& val);
template<class T>
void fill(T* begin, len_t n, const T& val);

/* uninitialized fill */
template<class T>
void ufill(T* begin, T* end, const T& val);
template<class T>
void ufill(T* begin, len_t n, const T& val);

/* regular copy */
template<class T, class InputIterator>
T_Copy_A<T> copy(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
NT_Copy_A<T> copy(T* dest, InputIterator begin, len_t n);

/* uninitialized copy */
template<class T, class InputIterator>
T_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
NT_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end);

/* regular move */
template<class T, class InputIterator>
T_Move_A<T> move(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
NT_Move_A<T> move(T* dest, InputIterator begin, InputIterator end);

/* uninitialized move */
template<class T, class InputIterator>
T_Move_C<T> umove(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
T_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end);

template<class T>
void destroy(T* begin, len_t n);
template<class T>
void destroy(T* begin, T* end);

template<class T>
void destroy_at(T* addr);

/* assumption: begin < dest <= end and addresses >= end are uninitialized*/
template<class T>
T_Move<T> shiftr(T* dest, T* begin, T* end);
template<class T>
NT_Move<T> shiftr(T* dest, T* begin, T* end);

/* assumption: dest < begin */
template<class T>
T_Move_A<T> shiftl(T* dest, T* begin, T* end);
template<class T>
NT_Move_A<T> shiftl(T* dest, T* begin, T* end);

template<class T>
void construct(T* begin, T* end) {
    while (begin != end)
        new (begin++) T();
}

template<class T>
void construct(T* begin, len_t n) {
    while (n--)
        new (begin++) T();
}

template<class T>
void fill(T* begin, T* end, const T& val) {
    std::fill(begin, end, val);
}

template<class T>
void fill(T* begin, len_t n, const T& val) {
    std::fill_n(begin, n, val);
}

template<class T>
void ufill(T* begin, T* end, const T& val) {
    std::uninitialized_fill(begin, end, val);
}

template<class T>
void ufill(T* begin, len_t n, const T& val) {
    std::uninitialized_fill_n(begin, n, val);
}

template<class T, class InputIterator>
T_Copy_A<T> copy(T* dest, InputIterator begin, len_t n) {
    std::memcpy(dest, &*begin, n * sizeof(T));
}

template<class T, class InputIterator>
NT_Copy_A<T> copy(T* dest, InputIterator begin, len_t n) {
    std::copy_n(begin, n, dest);
}

template<class T, class InputIterator>
T_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end) {
    std::memcpy(dest, &*begin, (end - begin) * sizeof(T));
}

template<class T, class InputIterator>
NT_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end) {
    std::uninitialized_copy(begin, end, dest);
}

template<class T, class InputIterator>
T_Move_A<T> move(T* dest, InputIterator begin, len_t n) {
    std::memcpy(dest, &*begin, n * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_A<T> move(T* dest, InputIterator begin, len_t n) {
    std::move(begin, begin + n, dest);
}

template<class T, class InputIterator>
T_Move_C<T> umove(T* dest, InputIterator begin, len_t n) {
    std::memcpy(dest, &*begin, n * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, len_t n) {
    std::uninitialized_move_n(begin, n, dest);
}

template<class T, class InputIterator>
T_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end) {
    std::memcpy(dest, &*begin, (end - begin) * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end) {
    std::uninitialized_move(begin, end, dest);
}

template<class T>
void destroy(T* begin, len_t n) {
    std::destroy_n(begin, n);
}

template<class T>
void destroy(T* begin, T* end) {
    std::destroy(begin, end);
}

template<class T>
void destroy_at(T* addr) {
    std::destroy_at(addr);
}

template<class T>
T_Move<T> shiftr(T* dest, T* begin, T* end) {
    std::memmove(dest, begin, (end - begin) * sizeof(T));
}

template<class T>
NT_Move<T> shiftr(T* dest, T* begin, T* end) {
    T* seg = end - (dest - begin);
    umove(end, seg, end);
    std::move_backward(begin, seg, end);
}

template<class T>
T_Move_A<T> shiftl(T* dest, T* begin, T* end) {
    std::memmove(dest, begin, (end - begin) * sizeof(T));
}

template<class T>
NT_Move_A<T> shiftl(T* dest, T* begin, T* end) {
    std::move(begin, end, dest);
}

} // namespace uwr
