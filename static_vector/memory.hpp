#pragma once

#include "trivial.hpp"

#include <memory>
#include <cstring>

namespace uwr {

typedef unsigned int uint;
using len_t = uint;

template<typename T>
void construct(T* begin, T* end);
template<typename T>
void construct(T* data, len_t n);

template<typename T>
void fill(T* begin, T* end, const T& val);
template<typename T>
void fill(T* data, len_t n, const T& val);

/* regular copy */
template<typename T, typename InputIterator>
T_Copy<T> copy(T* data, InputIterator begin, len_t n);
template<typename T, typename InputIterator>
NT_Copy<T> copy(T* data, InputIterator begin, len_t n);

/* uninitialized copy */
template<typename T, typename InputIterator>
T_Copy<T> ucopy(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Copy<T> ucopy(T* data, InputIterator begin, InputIterator end);

/* regular move */
template<typename T, typename InputIterator>
T_Move<T> move(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Move<T> move(T* data, InputIterator begin, InputIterator end);

/* uninitialized move */
template<typename T, typename InputIterator>
T_Move<T> umove(T* data, InputIterator begin, InputIterator end);
template<typename T, typename InputIterator>
NT_Move<T> umove(T* data, InputIterator begin, InputIterator end);

template<typename T>
void destroy(T* begin, T* end);
template<typename T>
void destroy(T* data, len_t n);

template<typename T>
void destroy_at(T* addr);

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
	std::uninitialized_fill(begin, end, val);
}

template<typename T>
void fill(T* data, len_t n, const T& val) {
	std::uninitialized_fill_n(data, n, val);
}

template<typename T, typename InputIterator>
T_Copy<T> copy(T* data, InputIterator begin, len_t n) {
	std::memcpy(data, &*begin, n * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Copy<T> copy(T* data, InputIterator begin, len_t n) {
	std::copy_n(begin, n, data);
}

template<typename T, typename InputIterator>
T_Copy<T> ucopy(T* data, InputIterator begin, InputIterator end) {
	std::memcpy(data, &*begin, (end - begin) * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Copy<T> ucopy(T* data, InputIterator begin, InputIterator end) {
	std::uninitialized_copy(begin, end, data);
}

template<typename T, typename InputIterator>
T_Move<T> move(T* data, InputIterator begin, len_t n) {
	std::memcpy(data, &*begin, n * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Move<T> move(T* data, InputIterator begin, len_t n) {
	for (len_t i = 0; i < n; ++i, ++data, ++begin)
		*data = std::move(*begin);
}

template<typename T, typename InputIterator>
T_Move<T> umove(T* data, InputIterator begin, InputIterator end) {
	std::memcpy(data, &*begin, (end - begin) * sizeof(T));
}

template<typename T, typename InputIterator>
NT_Move<T> umove(T* data, InputIterator begin, InputIterator end) {
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

} // namespace uwr