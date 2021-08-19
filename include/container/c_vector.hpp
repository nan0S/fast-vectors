#pragma once

#include "vector.hpp"
#include "../allocator/malloc_allocator.hpp"

namespace uwr {

template<class T>
using c_vector = vector<T, mem::malloc_allocator<T>>;

}
