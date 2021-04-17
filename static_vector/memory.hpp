#pragma once

#include "trivial.hpp"

#include <algorithm>
#include <cstring>

namespace uwr {

typedef unsigned int uint;
// TODO: restore to uint
// using len_t = uint;
using len_t = size_t;

template<typename T>
void construct(T* begin, T* end);
template<typename T>
void construct(T* data, len_t n);

/* regular fill */
template<typename T>
void fill(T* begin, T* end, const T& val);
template<typename T>
void fill(T* data, len_t n, const T& val);

/* uninitialized fill */
template<typename T>
void ufill(T* begin, T* end, const T& val);
template<typename T>
void ufill(T* data, len_t n, const T& val);

/* regular copy */
template<typename T, typename InputIterator>
T_Copy_A<T> copy(T* data, InputIterator begin, len_t n);
template<typename T, typename InputIterator>
NT_Copy_A<T> copy(T* data, InputIterator begin, len_t n);

/* uninitialized copy */
template<typename T, typename InputIterator>
T_Copy_C<T> ucopy(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Copy_C<T> ucopy(T* data, InputIterator begin, InputIterator end);

/* regular move */
template<typename T, typename InputIterator>
T_Move_A<T> move(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Move_A<T> move(T* data, InputIterator begin, InputIterator end);

/* uninitialized move */
template<typename T, typename InputIterator>
T_Move_C<T> umove(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Move_C<T> umove(T* data, InputIterator begin, InputIterator end);

template<typename T>
void destroy(T* begin, T* end);
template<typename T>
void destroy(T* data, len_t n);

template<typename T>
void destroy_at(T* addr);

/* assumption: begin < dest <= end and addresses >= end are uninitialized*/
template<typename T>
T_Move<T> shiftr(T* dest, T* begin, T* end);
template<typename T>
NT_Move<T> shiftr(T* dest, T* begin, T* end);

/* assumption: dest < begin */
template<typename T>
T_Move_A<T> shiftl(T* dest, T* begin, T* end);
template<typename T>
NT_Move_A<T> shiftl(T* dest, T* begin, T* end);

template<typename T>
void construct(T* begin, T* end) {
	std::uninitialized_default_construct(begin, end);
}

template<typename T>
void construct(T* data, len_t n) {
	std::uninitialized_default_construct_n(data, n);
}

template<typename T>
void fill(T* begin, T* end, const T& val) {
	std::fill(begin, end, val);
}

template<typename T>
void fill(T* data, len_t n, const T& val) {
	std::fill_n(data, n, val);
}

template<typename T>
void ufill(T* begin, T* end, const T& val) {
	std::uninitialized_fill(begin, end, val);
}

template<typename T>
void ufill(T* data, len_t n, const T& val) {
	std::uninitialized_fill_n(data, n, val);
}

template<typename T, typename InputIterator>
T_Copy_A<T> copy(T* data, InputIterator begin, len_t n) {
	std::memcpy(data, &*begin, n * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Copy_A<T> copy(T* data, InputIterator begin, len_t n) {
	std::copy_n(begin, n, data);
}

template<typename T, typename InputIterator>
T_Copy_C<T> ucopy(T* data, InputIterator begin, InputIterator end) {
	std::memcpy(data, &*begin, (end - begin) * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Copy_C<T> ucopy(T* data, InputIterator begin, InputIterator end) {
	std::uninitialized_copy(begin, end, data);
}

template<typename T, typename InputIterator>
T_Move_A<T> move(T* data, InputIterator begin, len_t n) {
	std::memcpy(data, &*begin, n * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Move_A<T> move(T* data, InputIterator begin, len_t n) {
	std::move(begin, begin + n, data);
}

template<typename T, typename InputIterator>
T_Move_C<T> umove(T* data, InputIterator begin, InputIterator end) {
	std::memcpy(data, &*begin, (end - begin) * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Move_C<T> umove(T* data, InputIterator begin, InputIterator end) {
	std::uninitialized_move(begin, end, data);
}

template<typename T>
void destroy(T* begin, T* end) {
	std::destroy(begin, end);
}

template<typename T>
void destroy(T* data, len_t n) {
	std::destroy_n(data, n);
}

template<typename T>
void destroy_at(T* addr) {
	std::destroy_at(addr);
}

template<typename T>
T_Move_C<T> shiftr(T* dest, T* begin, T* end) {
	std::memmove(dest, begin, (end - begin) * sizeof(T));
}

template<typename T>
NT_Move_C<T> shiftr(T* dest, T* begin, T* end) {
	T* seg = end - (dest - begin);
	umove(end, seg, end);
	std::move_backward(begin, seg, end);
}

template<typename T>
T_Move_A<T> shiftl(T* dest, T* begin, T* end) {
	std::memmove(dest, begin, (end - begin) * sizeof(T));
}

template<typename T>
NT_Move_A<T> shiftl(T* dest, T* begin, T* end) {
	std::move(begin, end, dest);
}

} // namespace uwr