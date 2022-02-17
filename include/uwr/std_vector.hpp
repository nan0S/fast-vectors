#pragma once

#include "vector.hpp"
#include "allocator/std_allocator.hpp"

namespace uwr {

template<class T>
using std_vector = vector<T, mem::std_allocator<T>>;

}
