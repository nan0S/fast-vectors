#pragma once

#include "vector.hpp"
#include "allocator/hybrid_allocator_lin.hpp"

namespace uwr {

template<class T>
using vector_lin = vector<T, mem::hybrid_allocator_lin<T>>;

}
