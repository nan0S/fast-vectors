#pragma once

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>

template<class T>
class StaticVectorTests : public ::testing::Test {
    void SetUp() {}
    void TearDown() {}

public:
    static constexpr int C = 10;
};

template<>
void StaticVectorTests<test_type>::SetUp();
template<>
void StaticVectorTests<test_type>::TearDown();

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
