#pragma once

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>
#include <static_vector.hpp>
using namespace uwr;

template<class T>
class StaticVectorTests : public ::testing::Test {
public:
    static constexpr len_t C = 10;
    using sv = static_vector<T, C>;
    using size_type = typename sv::size_type;

private:
    void SetUp() {}
    void TearDown() {}

public:
    T GetValue(int id) { return id; }
    sv GetVectorOfSize(size_type size);

    void InsertOneElementByCopy(sv& v, size_type pos, int id);
    void InsertOneElementByMove(sv& v, size_type pos, int id);
    void InsertMultipleElementsByFill(sv& v, size_type pos, size_type count, int id);
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
typename StaticVectorTests<T>::sv
StaticVectorTests<T>::GetVectorOfSize(size_type size) {
    static_vector<T, C> v(size);
    for (size_type i = 0; i < size; ++i)
        v[i] = GetValue(i);

    return v;
}

template<class T>
void
StaticVectorTests<T>::InsertOneElementByCopy(sv& v, size_type pos, int id) {
    const sv save = v;
    const T val = GetValue(id);
    const size_type initial_size = v.size();
    
    auto it = v.insert(v.begin() + pos, val);

    EXPECT_EQ(v.size(), initial_size + 1);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, val);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < initial_size; ++i)
        EXPECT_EQ(v[i + 1], save[i]);
}

template<class T>
void
StaticVectorTests<T>::InsertOneElementByMove(sv& v, size_type pos, int id) {
    const sv save = v;
    const size_type initial_size = v.size();
    
    auto it = v.insert(v.begin() + pos, GetValue(id));

    EXPECT_EQ(v.size(), initial_size + 1);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, GetValue(id));
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < initial_size; ++i)
        EXPECT_EQ(v[i + 1], save[i]);
}

template<class T>
void
StaticVectorTests<T>::InsertMultipleElementsByFill(sv& v, size_type pos, size_type count, int id) {
    const sv save = v;
    const size_type initial_size = v.size();
    const T val = GetValue(id);

    auto it = v.insert(v.begin() + pos, count, val);
    
    EXPECT_EQ(v.size(), initial_size + count);
    EXPECT_EQ(it, v.begin() + pos);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = 0; i < count; ++i)
        EXPECT_EQ(v[pos + i], val);
    for (size_type i = pos; i < initial_size; ++i)
        EXPECT_EQ(v[count + i], save[i]);
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
