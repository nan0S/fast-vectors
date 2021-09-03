#pragma once

#include "vector.hpp"
#include "../allocator/hybrid_allocator_orig2.hpp"

namespace uwr {

template<class T>
using vector_orig = vector<T, mem::hybrid_allocator_orig2<T>>;

}
