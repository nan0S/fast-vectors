#pragma once

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>

template<class T>
class StaticVectorTests : public ::testing::Test {

private:
    void SetUp() {}
    void TearDown() {}

public:
    T GetValue(int id) { return id; }
public:
    static constexpr int C = 10;
    using size_type = typename uwr::static_vector<T, C>::size_type;
};

template<>
void StaticVectorTests<test_type>::SetUp();
template<>
void StaticVectorTests<test_type>::TearDown();
template<>
std::string StaticVectorTests<std::string>::GetValue(int id);
template<>
std::array<int, 10> StaticVectorTests<std::array<int, 10>>::GetValue(int id);

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
