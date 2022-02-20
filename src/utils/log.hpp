#pragma once

#include <iostream>

template<class First, class... Args>
void log(First&& first, Args&&... args) {
   std::cout << std::forward<First>(first) << " ";
   log(std::forward<Args>(args)...);
}
