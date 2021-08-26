#pragma once

#include "vector.hpp"
#include "../allocator/hybrid_allocator_alt2.hpp"

namespace uwr {

template<class T>
using vector_alt2 = vector<T, mem::hybrid_allocator_alt2<T>>;

}
