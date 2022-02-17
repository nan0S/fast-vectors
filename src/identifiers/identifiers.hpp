#pragma once

#include "uwr/vector_orig.hpp"
#include "uwr/vector_orig_imp.hpp"
#include "uwr/vector_switch.hpp"
#include "uwr/vector_lin.hpp"
#include "uwr/vector_bs.hpp"
#include "uwr/vector_exp.hpp"
#include "uwr/rvector.h"

namespace uwr {

template<class T>
struct is_uwr_vector {
    static constexpr bool value = false;
};

template<class T>
struct is_uwr_vector<vector_orig<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_uwr_vector<vector_orig_imp<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_uwr_vector<vector_switch<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_uwr_vector<vector_lin<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_uwr_vector<vector_bs<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_uwr_vector<vector_exp<T>> {
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

} // namespace uwr

