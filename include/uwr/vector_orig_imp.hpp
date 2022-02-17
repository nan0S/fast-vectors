#pragma once

#include "vector.hpp"
#include "allocator/hybrid_allocator_orig_imp.hpp"

namespace uwr {

template<class T>
using vector_orig_imp = vector<T, mem::hybrid_allocator_orig_imp<T>>;

}
