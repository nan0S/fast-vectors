#pragma once

#include <type_traits>

namespace uwr {

template<typename T>
struct is_trivially_copyable {
	static constexpr bool value =
		std::is_trivially_copyable_v<T>;
};

template<typename T>
struct is_trivially_moveable {
	static constexpr bool value =
		std::is_trivially_move_constructible_v<T> &&
		std::is_trivially_move_assignable_v<T>;
};

template<typename T>
struct is_trivially_copy_constructible {
	static constexpr bool value =
		std::is_trivially_copy_constructible_v<T>;
};

template<typename T>
struct is_trivially_move_constructible {
	static constexpr bool value =
		std::is_trivially_move_constructible_v<T>;
};

template<typename T>
struct is_trivially_copy_assignable {
	static constexpr bool value =
		std::is_trivially_copy_assignable_v<T>;
};

template<typename T>
struct is_trivially_move_assignable {
	static constexpr bool value =
		std::is_trivially_move_assignable_v<T>;
};

template<typename T>
inline constexpr bool is_trivially_copyable_v =
	is_trivially_copyable<T>::value;

template<typename T>
inline constexpr bool is_trivially_moveable_v =
	is_trivially_moveable<T>::value;

template<typename T>
inline constexpr bool is_trivially_copy_constructible_v =
	is_trivially_copy_constructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_move_constructible_v =
	is_trivially_move_constructible<T>::value;

template<typename T>
inline constexpr bool is_trivially_copy_assignable_v =
	is_trivially_copy_assignable<T>::value;

template<typename T>
inline constexpr bool is_trivially_move_assignable_v =
	is_trivially_move_assignable<T>::value;

template<typename T>
using T_Move =
	std::enable_if_t<is_trivially_moveable_v<T>>;
template<typename T>
using NT_Move =
	std::enable_if_t<!is_trivially_moveable_v<T>>;

template<typename T>
using T_Copy_C =
	std::enable_if_t<is_trivially_copy_constructible_v<T>>;
template<typename T>
using NT_Copy_C =
	std::enable_if_t<!is_trivially_copy_constructible_v<T>>;

template<typename T>
using T_Move_C =
	std::enable_if_t<is_trivially_move_constructible_v<T>>;
template<typename T>
using NT_Move_C =
	std::enable_if_t<!is_trivially_move_constructible_v<T>>;

template<typename T>
using T_Copy_A =
	std::enable_if_t<is_trivially_copy_assignable_v<T>>;
template<typename T>
using NT_Copy_A =
	std::enable_if_t<!is_trivially_copy_assignable_v<T>>;

template<typename T>
using T_Move_A =
	std::enable_if_t<is_trivially_move_assignable_v<T>>;
template<typename T>
using NT_Move_A =
	std::enable_if_t<!is_trivially_move_assignable_v<T>>;

template<typename T>
using T_Copy =
	std::enable_if_t<is_trivially_copyable_v<T>>;
template<typename T>
using NT_Copy =
	std::enable_if_t<!is_trivially_copyable_v<T>>;

} // namespace uwr