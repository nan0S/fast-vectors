#pragma once

#include <type_traits>

namespace uwr {

template<typename T>
struct is_trivially_copy_constructible {
	static constexpr bool value = std::is_trivially_copy_constructible_v<T>;
};

template<typename T>
struct is_trivially_move_constructible {
	static constexpr bool value = std::is_trivially_move_constructible_v<T>;
};

template<typename T>
constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

template<typename T>
constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

template<typename T>
using T_Copy = std::enable_if_t<is_trivially_copy_constructible_v<T>>;
template<typename T>
using NT_Copy = std::enable_if_t<!is_trivially_copy_constructible_v<T>>;

template<typename T>
using T_Move = std::enable_if_t<is_trivially_move_constructible_v<T>>;
template<typename T>
using NT_Move = std::enable_if_t<!is_trivially_move_constructible_v<T>>;

} // namespace uwr