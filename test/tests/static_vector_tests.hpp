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

    void ExpectInsertedInAt(const sv& v, size_type pos,
            size_type count, const sv& save);

public:
    T GetValue(int id) { return id; }
    sv GetVectorOfSize(size_type size);

    void InsertOneElementByCopy(sv& v, size_type pos, int id);
    void InsertOneElementByMove(sv& v, size_type pos, int id);
    void InsertMultipleElementsByFill(sv& v, size_type pos,
            size_type count, int id);
    template<class InputIterator>
    void InsertMultipleElementsByRange(sv& v, size_type pos,
            InputIterator begin, InputIterator end);
    void InsertMultipleElementsByInitializerList(sv& v, size_type pos,
            std::initializer_list<T> ilist);

    void EmplaceAt(sv& v, size_type pos, int id);
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
StaticVectorTests<T>::ExpectInsertedInAt(const sv& v, size_type pos,
        size_type count, const sv& save) {
    EXPECT_EQ(v.size(), save.size() + count);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < save.size(); ++i)
        EXPECT_EQ(v[count + i], save[i]);
}

template<class T>
void
StaticVectorTests<T>::InsertOneElementByCopy(sv& v, size_type pos, int id) {
    const sv save = v;
    const T val = GetValue(id);
    
    auto it = v.insert(v.begin() + pos, val);

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, val);
}

template<class T>
void
StaticVectorTests<T>::InsertOneElementByMove(sv& v, size_type pos, int id) {
    const sv save = v;
    
    auto it = v.insert(v.begin() + pos, GetValue(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, GetValue(id));
}

template<class T>
void
StaticVectorTests<T>::InsertMultipleElementsByFill(sv& v, size_type pos, size_type count, int id) {
    const sv save = v;
    const T val = GetValue(id);

    auto it = v.insert(v.begin() + pos, count, val);

    ExpectInsertedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
    for (size_type i = 0; i < count; ++i)
        EXPECT_EQ(v[pos + i], val);
}

template<class T>
template<class InputIterator>
void
StaticVectorTests<T>::InsertMultipleElementsByRange(sv& v, size_type pos,
        InputIterator begin, InputIterator end) {
    const sv save = v;
    const size_type count = std::distance(begin, end);
    T save_range[count];
    std::copy(&*begin, &*end, save_range);

    auto it = v.insert(v.begin() + pos, begin, end);

    ExpectInsertedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
    for (size_type i = 0; i < count; ++i, ++begin) {
        EXPECT_EQ(*begin, save_range[i]);
        EXPECT_EQ(v[pos + i], *begin);
    }
}

template<class T>
void
StaticVectorTests<T>::InsertMultipleElementsByInitializerList(sv& v, size_type pos,
        std::initializer_list<T> ilist) {
    const sv save = v;
    const size_type count = ilist.size();
    T save_range[count];
    std::copy(ilist.begin(), ilist.end(), save_range);

    auto it = v.insert(v.begin() + pos, ilist);

    ExpectInsertedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
    for (size_type i = 0; i < count; ++i) {
        EXPECT_EQ(ilist.begin()[i], save_range[i]);
        EXPECT_EQ(v[pos + i], ilist.begin()[i]);
    }
}

template<class T>
void
StaticVectorTests<T>::EmplaceAt(sv& v, size_type pos, int id) {
    const sv save = v;
    
    auto it = v.emplace(v.begin() + pos, GetValue(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, GetValue(id));
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
