#pragma once

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
using namespace uwr;

template<class T>
class StaticVectorTests : public ::testing::Test {
public:
    static constexpr len_t C = 10;
    using size_type = typename uwr::static_vector<T, C>::size_type;

private:
    void SetUp() {}
    void TearDown() {}

public:
    T GetValue(int id) { return id; }
    static_vector<T, C> GetVectorOfSize(size_type size);
};

template<>
void StaticVectorTests<test_type>::SetUp();
template<>
void StaticVectorTests<test_type>::TearDown();
template<>
std::string StaticVectorTests<std::string>::GetValue(int id);
template<>
std::array<int, 10> StaticVectorTests<std::array<int, 10>>::GetValue(int id);

template<class T>
static_vector<T, StaticVectorTests<T>::C>
StaticVectorTests<T>::GetVectorOfSize(size_type size) {
    static_vector<T, C> v(size);
    for (size_type i = 0; i < size; ++i)
        v[i] = GetValue(i);

    return v;
}

class TypeNames {
public:
    template<class T>
    static std::string GetName(int id);
};

template<class T>
std::string TypeNames::GetName(int id) {
    if (std::is_same_v<T, int>) return "int";
    if (std::is_same_v<T, std::string>) return "string";
    if (std::is_same_v<T, test_type>) return "test_type";
    if (std::is_same_v<T, std::array<int, 10>>) return "array<int, 10>";
    return std::to_string(id);
}

using TestedTypes = ::testing::Types<int, test_type, std::string, std::array<int, 10>>;
TYPED_TEST_SUITE(StaticVectorTests, TestedTypes, TypeNames);
