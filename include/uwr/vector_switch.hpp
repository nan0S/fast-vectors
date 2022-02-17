#pragma once

#include "vector.hpp"
#include "allocator/hybrid_allocator_switch.hpp"

namespace uwr {

template<class T>
using vector_switch = vector<T, mem::hybrid_allocator_switch<T>>;

}
