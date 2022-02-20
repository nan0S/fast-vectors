#pragma once

#include <string>
#include <array>

/*
 * map int to class T, e.g. get_value<str>(10) -> "test10"
 * feel free to specify for your own types
 */
template<class T>
T get_value(int id) {
   return id;
}

template<>
std::string get_value(int id);
template<>
std::array<int, 10> get_value(int id);
template<>
std::array<int, 100> get_value(int id);

template<class V>
V get_container(int size) {
   using T = typename V::value_type;
   V v;
   v.reserve(size);
   for (int i = 0; i < size; ++i)
      v.emplace_back(get_value<T>(i));
   return v;
}

