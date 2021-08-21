#pragma once

#include <vector.hpp>
#include <rvector.h>

namespace uwr {

template<class T>
struct is_uwr_vector {
    static constexpr bool value = false;
};

template<class T>
struct is_uwr_vector<vector<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_rvector {
    static constexpr bool value = false;
};

template<class T>
struct is_rvector<rvector<T>> {
    static constexpr bool value = true;
};

}