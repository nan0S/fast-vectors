#pragma once

#include "uwr/vector.hpp"

namespace uwr {

template<class T>
struct is_uwr_vector {
   static constexpr bool value = false;
};

template<class T, class A>
struct is_uwr_vector<vector<T, A>> {
   static constexpr bool value = true;
};

} // namespace uwr

