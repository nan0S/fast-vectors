#pragma once

#include "vector.hpp"
#include "allocator/hybrid_allocator_exp.hpp"

namespace uwr {

template<class T>
using vector_exp = vector<T, mem::hybrid_allocator_exp<T>>;

}
