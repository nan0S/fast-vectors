#pragma once

#include <ratio>

namespace uwr {

using growth_factor_2_0 = std::ratio<2, 1>;
using growth_factor_1_6 = std::ratio<8, 5>;
using growth_factor_1_5 = std::ratio<3, 2>;

using default_growth_factor = growth_factor_2_0;

}
