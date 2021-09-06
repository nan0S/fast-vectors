#pragma once

#include "vector.hpp"
#include "../allocator/hybrid_allocator_bs.hpp"

namespace uwr {

template<class T>
using vector_bs = vector<T, mem::hybrid_allocator_bs<T>>;

}
