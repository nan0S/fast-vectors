#pragma once

#include "vector.hpp"
#include "../common/big_allocator.hpp"

namespace uwr {

template<class T>
using big_vector = vector<T, mem::big_allocator<T>>;

}
