#pragma once

#include <iostream>

template<class T>
void info(const T& msg, int indent=0, bool reset=false) {
    if (reset)
        std::cout << '\r';
    std::cout << std::string(3 * indent, ' ') << msg;
    if (!reset)
        std::cout << std::endl;
    else
        std::cout.flush();
}
