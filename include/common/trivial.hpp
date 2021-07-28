#pragma once

#include <type_traits>

/*
 * Custom markers if class is trivially:
 *     - default constructible
 *     - copyable
 *     - copy constructible
 *     - move constructible
 *     - moveable
 *     - copy assignable
 *     - move assignable 
 *       
 * By default these custom classes use corresponding
 * std variants, but in some cases your type might
 * actually be e.g. trivially moveable although it
 * has move constructor. In that case you can partially
 * specfiy is_trivially_moveable struct for that type
 * and hardcode the value to be true:
 * 
 * template<>
 * struct is_trivially_moveable<MySpecialType> {
 *     static constexpr bool value = true;
 * };
 * 
 * This way you still have your constructor
 * specified, you achieve better performance with
 * our containers and do not influence other parts
 * of your architecture as you may not want to
 * override std::is_trivially_moveable as this could
 * break something somewhere else.
 */

namespace uwr::mem {

template<class T>
struct is_trivially_default_constructible {
    static constexpr bool value =
        std::is_trivially_default_constructible_v<T>;
};

template<class T>
struct is_trivially_copyable {
    static constexpr bool value =
        std::is_trivially_copyable_v<T>;
};

template<class T>
struct is_trivially_moveable {
    static constexpr bool value =
        std::is_trivially_move_constructible_v<T> &&
        std::is_trivially_move_assignable_v<T>;
};

template<class T>
struct is_trivially_copy_constructible {
    static constexpr bool value =
        std::is_trivially_copy_constructible_v<T>;
};

template<class T>
struct is_trivially_move_constructible {
    static constexpr bool value =
        std::is_trivially_move_constructible_v<T>;
};

template<class T>
struct is_trivially_copy_assignable {
    static constexpr bool value =
        std::is_trivially_copy_assignable_v<T>;
};

template<class T>
struct is_trivially_move_assignable {
    static constexpr bool value =
        std::is_trivially_move_assignable_v<T>;
};

template<class T>
inline constexpr bool is_trivially_default_constructible_v =
    is_trivially_default_constructible<T>::value;

template<class T>
inline constexpr bool is_trivially_copyable_v =
    is_trivially_copyable<T>::value;

template<class T>
inline constexpr bool is_trivially_moveable_v =
    is_trivially_moveable<T>::value;

template<class T>
inline constexpr bool is_trivially_copy_constructible_v =
    is_trivially_copy_constructible<T>::value;

template<class T>
inline constexpr bool is_trivially_move_constructible_v =
    is_trivially_move_constructible<T>::value;

template<class T>
inline constexpr bool is_trivially_copy_assignable_v =
    is_trivially_copy_assignable<T>::value;

template<class T>
inline constexpr bool is_trivially_move_assignable_v =
    is_trivially_move_assignable<T>::value;

template<class T, class R = void>
using T_Cons_D =
    std::enable_if_t<is_trivially_default_constructible_v<T>, R>;
template<class T, class R = void>
using NT_Cons_D =
    std::enable_if_t<!is_trivially_default_constructible_v<T>, R>;

template<class T, class R = void>
using T_Move =
    std::enable_if_t<is_trivially_moveable_v<T>, R>;
template<class T, class R = void>
using NT_Move =
    std::enable_if_t<!is_trivially_moveable_v<T>, R>;

template<class T, class R = void>
using T_Copy_C =
    std::enable_if_t<is_trivially_copy_constructible_v<T>, R>;
template<class T, class R = void>
using NT_Copy_C =
    std::enable_if_t<!is_trivially_copy_constructible_v<T>, R>;

template<class T, class R = void>
using T_Move_C =
    std::enable_if_t<is_trivially_move_constructible_v<T>, R>;
template<class T, class R = void>
using NT_Move_C =
    std::enable_if_t<!is_trivially_move_constructible_v<T>, R>;

template<class T, class R = void>
using T_Copy_A =
    std::enable_if_t<is_trivially_copy_assignable_v<T>, R>;
template<class T, class R = void>
using NT_Copy_A =
    std::enable_if_t<!is_trivially_copy_assignable_v<T>, R>;

template<class T, class R = void>
using T_Move_A =
    std::enable_if_t<is_trivially_move_assignable_v<T>, R>;
template<class T, class R = void>
using NT_Move_A =
    std::enable_if_t<!is_trivially_move_assignable_v<T>, R>;

template<class T, class R = void>
using T_Copy =
    std::enable_if_t<is_trivially_copyable_v<T>, R>;
template<class T, class R = void>
using NT_Copy =
    std::enable_if_t<!is_trivially_copyable_v<T>, R>;

} // namespace uwr
