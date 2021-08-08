#pragma once

#include <algorithm>
#include <cstring>

#include "type_traits.hpp"
#include "define.hpp"

namespace uwr::mem {

/*
 * For now most of functions like
 * construct, fill, ufill, copy,
 * ucopy, move, umove are wrappers.
 * So why even use them - one reason
 * is to be independent - it is easy to
 * replace one function with different
 * implementation. The other one is more
 * practical - std::uninitialized_default_construct
 * does not zero initialize memory for
 * trivial types - our construct(...) does.
 */

// TODO: toogle comments
// using len_t = std::uint_fast32_t;
using len_t = std::size_t;

// should use getpagesize() but want constexpr
inline constexpr len_t page_size = 4096;

/*
 * construct (so should be uninitialized) continuous memory
 */
template<class T>
UWR_FORCEINLINE constexpr
T_Cons_D<T> construct(T* begin, T* end);
template<class T>
UWR_FORCEINLINE constexpr
NT_Cons_D<T> construct(T* begin, T* end);
template<class T>
UWR_FORCEINLINE constexpr
T_Cons_D<T> construct(T* begin, len_t n);
template<class T>
UWR_FORCEINLINE constexpr
NT_Cons_D<T> construct(T* begin, len_t n);

/*
 * fill initialized, continuous memory
 */
template<class T>
UWR_FORCEINLINE constexpr
void fill(T* begin, T* end, const T& val);
template<class T>
UWR_FORCEINLINE constexpr
T* fill(T* begin, len_t n, const T& val);

/*
 * hybrid fill continuous memory,
 * initialized and unitialized variants
 */
template<class T>
UWR_FORCEINLINE constexpr
T_Copy_A<T> hybrid_fill(T* begin, len_t n, const T& val);
template<class T>
UWR_FORCEINLINE constexpr
T_Copy_C<T> hybrid_ufill(T* begin, len_t n, const T& val);
// TODO: temporary
#define HYBRID_THRESHOLD 1
template<class T>
constexpr
void _hybrid_fill(T* begin, len_t n, const T& val);

/*
 * fill unitialized continuous memory
 */
template<class T>
UWR_FORCEINLINE constexpr
void ufill(T* begin, T* end, const T& val);
template<class T>
UWR_FORCEINLINE constexpr
T* ufill(T* begin, len_t n, const T& val);

/*
 * copy into initialized memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* copy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* copy(T* dest, InputIterator begin, len_t n);

/*
 * copy into uninitialized, continuous memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* ucopy(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* ucopy(T* dest, InputIterator begin, len_t n);

/*
 * move into initialized, continuous memory
 */
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* move(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* move(T* dest, InputIterator begin, len_t n);

/*
 * move into uninitialized memor
 */
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* umove(T* dest, InputIterator begin, InputIterator end);
template<class T, class InputIterator>
UWR_FORCEINLINE constexpr
T* umove(T* dest, InputIterator begin, len_t n);

/*
 * destroy range of objects (so are initialized)
 */
template<class T>
UWR_FORCEINLINE constexpr
void destroy(T* begin, T* end);
template<class T>
UWR_FORCEINLINE constexpr
T* destroy(T* begin, len_t n);

/*
 * destroy at specified address
 */
template<class T>
UWR_FORCEINLINE constexpr
void destroy_at(T* addr);

/*
 * create object of type T (call its constructor)
 * only if args are proper constructor arguemnts,
 * if args is one object of type T, just return that
 * object and don't call constructor
 * NOTE: would hapilly remove that function
 */
template<class T, class V, class... Args>
UWR_FORCEINLINE constexpr
T create(V&& x, Args&&... args);
template<class T>
UWR_FORCEINLINE constexpr
const T& create(const T& x);
template<class T>
UWR_FORCEINLINE constexpr
T&& create(T&& x);
template<class T>
UWR_FORCEINLINE constexpr
T create();

/*
 * shifts data to the right,
 * assumption: begin < dest <= end 
 * and addresses >= end are uninitialized,
 */
template<class T>
UWR_FORCEINLINE constexpr
T_Move<T> shiftr(T* dest, T* begin, T* end);
template<class T>
UWR_FORCEINLINE constexpr
NT_Move<T> shiftr(T* dest, T* begin, T* end);

/*
 * shifts data to the left,
 * assumption: dest < begin
 */
template<class T>
UWR_FORCEINLINE constexpr
T* shiftl(T* dest, T* begin, T* end);


/*
 * implementations
 */
template<class T>
constexpr
T_Cons_D<T> construct(T* begin, T* end) {
    std::memset(begin, 0, (end - begin) * sizeof(T));
}

template<class T>
constexpr
NT_Cons_D<T> construct(T* begin, T* end) {
    std::uninitialized_default_construct(begin, end);
}

template<class T>
constexpr
T_Cons_D<T> construct(T* begin, len_t n) {
    std::memset(begin, 0, n * sizeof(T));
}

template<class T>
constexpr
NT_Cons_D<T> construct(T* begin, len_t n) {
    std::uninitialized_default_construct_n(begin, n);
}

template<class T>
constexpr
void fill(T* begin, T* end, const T& val) {
    std::fill(begin, end, val);
}

template<class T>
constexpr
T* fill(T* begin, len_t n, const T& val) {
    return std::fill_n(begin, n, val);
}

// TODO: remove or standarize
template<class T>
constexpr
T_Copy_A<T> hybrid_fill(T* begin, len_t n, const T& val) {
    if (n <= HYBRID_THRESHOLD)
        return fill(begin, n, val);
    else
        return _hybrid_fill(begin, n, val);
}

// TODO: remove or standarize
template<class T>
constexpr
T_Copy_C<T> hybrid_ufill(T* begin, len_t n, const T& val) {
    if (n <= HYBRID_THRESHOLD)
        ufill(begin, n, val);
    else
        _hybrid_fill(begin, n, val);
}

// TODO: remove or standarize
template<class T>
constexpr
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

template<class T>
constexpr
void ufill(T* begin, T* end, const T& val) {
    std::uninitialized_fill(begin, end, val);
}

template<class T>
constexpr
T* ufill(T* begin, len_t n, const T& val) {
    return std::uninitialized_fill_n(begin, n, val);
}

template<class T, class InputIterator>
constexpr
T* copy(T* dest, InputIterator begin, InputIterator end) {
    return std::copy(begin, end, dest);
}

template<class T, class InputIterator>
constexpr
T* copy(T* dest, InputIterator begin, len_t n) {
    return std::copy_n(begin, n, dest);
}

template<class T, class InputIterator>
constexpr
T* ucopy(T* dest, InputIterator begin, InputIterator end) {
    return std::uninitialized_copy(begin, end, dest);
}

template<class T, class InputIterator>
constexpr
T* ucopy(T* dest, InputIterator begin, len_t n) {
    return std::uninitialized_copy_n(begin, n, dest);
}

template<class T, class InputIterator>
constexpr
T* move(T* dest, InputIterator begin, InputIterator end) {
    return std::move(begin, end, dest);
}

template<class T, class InputIterator>
constexpr
T* move(T* dest, InputIterator begin, len_t n) {
    return std::move(begin, begin + n, dest);
}

template<class T, class InputIterator>
constexpr
T* umove(T* dest, InputIterator begin, InputIterator end) {
    return std::uninitialized_move(begin, end, dest);
}

template<class T, class InputIterator>
constexpr
T* umove(T* dest, InputIterator begin, len_t n) {
    return std::uninitialized_move_n(begin, n, dest);
}

template<class T>
constexpr
void destroy(T* begin, T* end) {
    std::destroy(begin, end);
}

template<class T>
constexpr
T* destroy(T* begin, len_t n) {
    return std::destroy_n(begin, n);
}

template<class T>
constexpr
void destroy_at(T* addr) {
    std::destroy_at(addr);
}

template<class T>
constexpr
T_Move<T> shiftr(T* dest, T* begin, T* end) {
    std::memmove(dest, begin, (end - begin) * sizeof(T));
}

template<class T>
constexpr
NT_Move<T> shiftr(T* dest, T* begin, T* end) {
    T* seg = end - (dest - begin);
    umove(end, seg, end);
    std::move_backward(begin, seg, end);
}

template<class T>
constexpr
T* shiftl(T* dest, T* begin, T* end) {
    return std::move(begin, end, dest);
}

template<class T, class V, class... Args>
constexpr
T create(V&& x, Args&&... args) {
    return T(std::forward<V>(x), std::forward<Args>(args)...);
}

template<class T>
constexpr
const T& create(const T& x) {
    return x;
}

template<class T>
constexpr
T&& create(T&& x) {
    return std::forward<T>(x);
}

template<class T>
constexpr
T create() {
    return T();
}

} // namespace uwr::mem
