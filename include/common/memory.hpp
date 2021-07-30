#pragma once

#include <algorithm>
#include <cstring>
#include "trivial.hpp"
#include "define.hpp"

namespace uwr::mem {

typedef unsigned int uint;
// TODO: restore to uint
// using len_t = uint;
using len_t = std::size_t;

/*
 * construct (so should be uninitialized)
 */
template<class T>
UWR_FORCEINLINE
T_Cons_D<T> construct(T* begin, T* end);
template<class T>
UWR_FORCEINLINE
NT_Cons_D<T> construct(T* begin, T* end);
template<class T>
UWR_FORCEINLINE
T_Cons_D<T> construct(T* begin, len_t n);
template<class T>
UWR_FORCEINLINE
NT_Cons_D<T> construct(T* begin, len_t n);

/*
 * fill initialized memory
 */
template<class T>
UWR_FORCEINLINE
void fill(T* begin, T* end, const T& val);
template<class T>
UWR_FORCEINLINE
void fill(T* begin, len_t n, const T& val);

// TODO: remove
#define HYBRID_THRESHOLD 1
template<class T>
UWR_FORCEINLINE
void _hybrid_fill(T* begin, len_t n, const T& val);

/*
 * hybrid fill initialized memory
 */
template<class T>
UWR_FORCEINLINE
T_Copy_A<T> hybrid_fill(T* begin, len_t n, const T& val);

/*
 * fill unitialized memory
 */
template<class T>
UWR_FORCEINLINE
void ufill(T* begin, T* end, const T& val);
template<class T>
UWR_FORCEINLINE
void ufill(T* begin, len_t n, const T& val);

/*
 * optimized fill uninitialized memory
 */
template<class T>
UWR_FORCEINLINE
T_Copy_C<T> hybrid_ufill(T* begin, len_t n, const T& val);

/*
 * copy into initialized memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Copy_A<T> copy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Copy_A<T> copy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Copy_A<T> copy(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Copy_A<T> copy(T* dest, InputIterator begin, len_t n);

/*
 * copy into uninitialized memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Copy_C<T> ucopy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Copy_C<T> ucopy(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Copy_C<T> ucopy(T* dest, InputIterator begin, len_t n);

/*
 * move into initialized memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Move_A<T> move(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Move_A<T> move(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Move_A<T> move(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Move_A<T> move(T* dest, InputIterator begin, len_t n);

/*
 * move into uninitialized memor
 */
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE
T_Move_C<T> umove(T* dest, InputIterator begin, len_t n);
template<class T, class InputIterator>
UWR_FORCEINLINE
NT_Move_C<T> umove(T* dest, InputIterator begin, len_t n);

/*
 * destroy range of objects (so are initialized)
 */
template<class T>
UWR_FORCEINLINE
void destroy(T* begin, T* end);
template<class T>
UWR_FORCEINLINE
void destroy(T* begin, len_t n);

/*
 * destroy at specified adderss
 */
template<class T>
UWR_FORCEINLINE
void destroy_at(T* addr);

/*
 * create object of type T (call its constructor)
 * only if args are proper constructor arguemnts,
 * if args is one object of type T, just return that
 * object and don't call constructor
 * note: would hapilly remove that function
 */
template<class T, class V, class... Args>
UWR_FORCEINLINE
T create(V&& x, Args&&... args);
template<class T>
UWR_FORCEINLINE
const T& create(const T& x);
template<class T>
UWR_FORCEINLINE
T&& create(T&& x);
template<class T>
UWR_FORCEINLINE
T create();

/*
 * shifts data to the right,
 * assumption: begin < dest <= end and addresses >= end are uninitialized 
 */
template<class T>
UWR_FORCEINLINE
T_Move<T> shiftr(T* dest, T* begin, T* end);
template<class T>
UWR_FORCEINLINE
NT_Move<T> shiftr(T* dest, T* begin, T* end);

/*
 * shifts data to the left, assumption: dest < begin
 */
template<class T>
UWR_FORCEINLINE
T_Move_A<T> shiftl(T* dest, T* begin, T* end);
template<class T>
UWR_FORCEINLINE
NT_Move_A<T> shiftl(T* dest, T* begin, T* end);


/*
 * implementations
 */

// TODO: remove
template<class T>
UWR_FORCEINLINE
T_Move<T> shiftr_data(T* begin, len_t end)
{
    memmove(begin + 1, begin, end * sizeof(T));
}

template<class T>
UWR_FORCEINLINE
NT_Move<T> shiftr_data(T* begin, len_t end)
{
    auto end_p = begin + end;
    new (end_p) T(std::move(*(end_p - 1)));
    std::move_backward(begin, end_p - 1, end_p);
    begin->~T();
}

template<class T>
T_Cons_D<T> construct(T* begin, T* end) {
    std::memset(begin, 0, (end - begin) * sizeof(T));
}

template<class T>
NT_Cons_D<T> construct(T* begin, T* end) {
    std::uninitialized_default_construct(begin, end);
}

template<class T>
T_Cons_D<T> construct(T* begin, len_t n) {
    std::memset(begin, 0, n * sizeof(T));
}

template<class T>
NT_Cons_D<T> construct(T* begin, len_t n) {
    std::uninitialized_default_construct_n(begin, n);
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
T_Copy_A<T> hybrid_fill(T* begin, len_t n, const T& val) {
    if (n <= HYBRID_THRESHOLD)
        fill(begin, n, val);
    else
        _hybrid_fill(begin, n, val);
}

template<class T>
void ufill(T* begin, T* end, const T& val) {
    std::uninitialized_fill(begin, end, val);
}

template<class T>
void ufill(T* begin, len_t n, const T& val) {
    std::uninitialized_fill_n(begin, n, val);
}

template<class T>
T_Copy_C<T> hybrid_ufill(T* begin, len_t n, const T& val) {
    if (n <= HYBRID_THRESHOLD)
        ufill(begin, n, val);
    else
        _hybrid_fill(begin, n, val);
}

// TODO: temporary
template<class T>
void _hybrid_fill(T* begin, len_t n, const T& val) {
    *begin = val;
    len_t cur = 1;
    while (2 * cur <= n) {
        std::memcpy(begin + cur, begin, cur * sizeof(T));
        cur *= 2;
    }
    int last = n - cur;
    std::memcpy(begin + cur, begin + cur - last, last * sizeof(T));
}

template<class T, class InputIterator>
T_Copy_A<T> copy(T* dest, InputIterator begin, InputIterator end) {
    std::memcpy(dest, &*begin, (end - begin) * sizeof(T));
}

template<class T, class InputIterator>
NT_Copy_A<T> copy(T* dest, InputIterator begin, InputIterator end) {
    std::copy(begin, end, dest);
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
T_Copy_C<T> ucopy(T* dest, InputIterator begin, len_t n) {
    std::memcpy(dest, &*begin, n * sizeof(T));
}

template<class T, class InputIterator>
NT_Copy_C<T> ucopy(T* dest, InputIterator begin, len_t n) {
    std::uninitialized_copy_n(begin, n, dest);
}

template<class T, class InputIterator>
T_Move_A<T> move(T* dest, InputIterator begin, InputIterator end) {
    std::memcpy(dest, &*begin, (end - begin) * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_A<T> move(T* dest, InputIterator begin, InputIterator end) {
    std::move(begin, end, dest);
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
T_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end) {
    std::memcpy(dest, &*begin, (end - begin) * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, InputIterator end) {
    std::uninitialized_move(begin, end, dest);
}

template<class T, class InputIterator>
T_Move_C<T> umove(T* dest, InputIterator begin, len_t n) {
    std::memcpy(dest, &*begin, n * sizeof(T));
}

template<class T, class InputIterator>
NT_Move_C<T> umove(T* dest, InputIterator begin, len_t n) {
    std::uninitialized_move_n(begin, n, dest);
}

template<class T>
void destroy(T* begin, T* end) {
    std::destroy(begin, end);
}

template<class T>
void destroy(T* begin, len_t n) {
    std::destroy_n(begin, n);
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

template<class T, class V, class... Args>
T create(V&& x, Args&&... args) {
    return T(std::forward<V>(x), std::forward<Args>(args)...);
}

template<class T>
const T& create(const T& x) {
    return x;
}

template<class T>
T&& create(T&& x) {
    return std::forward<T>(x);
}

template<class T>
T create() {
    return T();
}

} // namespace uwr::mem
