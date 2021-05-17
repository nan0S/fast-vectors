#pragma once

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>

// template<class T>
// using vector_t = std::vector<T>;

template<class T>
class VectorTestsBase : public ::testing::Test {
public:
    using vector = vector_t<T>;
    using value_type = typename vector::value_type;
    using size_type = typename vector::size_type;
    static constexpr size_type C = 10;

private:
    void SetUp() {}
    void TearDown() {}

    void ExpectInsertedInAt(const vector& v, size_type pos,
            size_type count, const vector& save);

public:
    value_type GetValue(int id) { return id; }
    vector GetVectorOfSize(size_type size);

    void InsertOneElementByCopy(vector& v, size_type pos, int id);
    void InsertOneElementByMove(vector& v, size_type pos, int id);
    void InsertMultipleElementsByFill(vector& v, size_type pos,
            size_type count, int id);
    template<class InputIterator>
    void InsertMultipleElementsByRange(vector& v, size_type pos,
            InputIterator begin, InputIterator end);
    void InsertMultipleElementsByInitializerList(vector& v, size_type pos,
            std::initializer_list<value_type> ilist);

    void EmplaceAt(vector& v, size_type pos, int id);

    void EraseOneElement(vector& v, size_type pos);
    void EraseMultipleElements(vector& v, size_type pos, size_type count);

    void ExpectErasedInAt(const vector& v, size_type pos,
            size_type count, const vector& save);
};

template<>
void VectorTestsBase<test_type>::SetUp();
template<>
void VectorTestsBase<test_type>::TearDown();
template<>
std::string VectorTestsBase<std::string>::GetValue(int id);
template<>
std::array<int, 10> VectorTestsBase<std::array<int, 10>>::GetValue(int id);

template<class T>
typename VectorTestsBase<T>::vector
VectorTestsBase<T>::GetVectorOfSize(size_type size) {
    vector v(size);
    for (size_type i = 0; i < size; ++i)
        v[i] = GetValue(i);

    return v;
}

template<class T>
void
VectorTestsBase<T>::ExpectInsertedInAt(const vector& v, size_type pos,
        size_type count, const vector& save) {
    EXPECT_EQ(v.size(), save.size() + count);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < save.size(); ++i)
        EXPECT_EQ(v[count + i], save[i]);
}

template<class T>
void
VectorTestsBase<T>::ExpectErasedInAt(const vector& v, size_type pos,
        size_type count, const vector& save) {
    EXPECT_EQ(v.size(), save.size() - count);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < v.size(); ++i)
        EXPECT_EQ(v[i], save[i + count]);
}

template<class T>
void
VectorTestsBase<T>::InsertOneElementByCopy(vector& v, size_type pos, int id) {
    const vector save = v;
    const value_type val = GetValue(id);
    
    auto it = v.insert(v.begin() + pos, val);

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, val);
}

template<class T>
void
VectorTestsBase<T>::InsertOneElementByMove(vector& v, size_type pos, int id) {
    const vector save = v;
    
    auto it = v.insert(v.begin() + pos, GetValue(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, GetValue(id));
}

template<class T>
void
VectorTestsBase<T>::InsertMultipleElementsByFill(vector& v, size_type pos, size_type count, int id) {
    const vector save = v;
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
VectorTestsBase<T>::InsertMultipleElementsByRange(vector& v, size_type pos,
        InputIterator begin, InputIterator end) {
    const vector save = v;
    const size_type count = std::distance(begin, end);
    value_type save_range[count];
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
VectorTestsBase<T>::InsertMultipleElementsByInitializerList(vector& v, size_type pos,
        std::initializer_list<value_type> ilist) {
    const vector save = v;
    const size_type count = ilist.size();
    value_type save_range[count];
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
VectorTestsBase<T>::EmplaceAt(vector& v, size_type pos, int id) {
    const vector save = v;
    
    auto it = v.emplace(v.begin() + pos, GetValue(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, GetValue(id));
}

template<class T>
void
VectorTestsBase<T>::EraseOneElement(vector& v, size_type pos) {
    const vector save = v;
    const size_type initial_size = v.size();

    auto it = v.erase(v.begin() + pos);

    ExpectErasedInAt(v, pos, pos == initial_size ? 0 : 1, save);
    EXPECT_EQ(it, v.begin() + pos);
}

template<class T>
void
VectorTestsBase<T>::EraseMultipleElements(vector& v, size_type pos, size_type count) {
    ASSERT_LE(pos + count, v.size());

    const vector save = v;

    auto it = v.erase(v.begin() + pos, v.begin() + pos + count);

    ExpectErasedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
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

using TestedTypes = ::testing::Types<
    int, test_type, std::string, std::array<int, 10>
>;

TYPED_TEST_SUITE(VectorTestsBase, TestedTypes, TypeNames);
