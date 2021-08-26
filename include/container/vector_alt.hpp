#pragma once

#include "vector.hpp"
#include "../allocator/hybrid_allocator_alt.hpp"

namespace uwr {

template<class T>
using vector_alt = vector<T, mem::hybrid_allocator_alt<T>>;

}
