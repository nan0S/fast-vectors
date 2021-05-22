#pragma once

#define CPP_ABOVE_17 __cplusplus > 201703L

#include <gtest/gtest.h>
#include <test_type/test_type.hpp>
#include <utils/utils.hpp>

template<class V>
class VectorTestBaseFixture : public ::testing::Test {
public:
    using vector = V;
    using value_type = typename vector::value_type;
    using size_type = typename vector::size_type;
    static constexpr size_type C = 10;

private:
    void SetUp() {}
    void TearDown() {}

    void ExpectInsertedInAt(const vector& v, size_type pos,
            size_type count, const vector& save);

public:
    value_type GetValue(int id);
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
void
VectorTestBaseFixture<tested_vector_t<test_type>>::SetUp() {
    test_type::instances = 0;
    test_type::start_recording();
}

template<>
void
VectorTestBaseFixture<tested_vector_t<test_type>>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
    test_type::stop_recording();
}

template<>
void
VectorTestBaseFixture<compare_vector_t<test_type>>::SetUp() {
    test_type::instances = 0;
    test_type::start_recording();
}

template<>
void
VectorTestBaseFixture<compare_vector_t<test_type>>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
    EXPECT_TRUE(test_type::is_current_not_better());
}

template<class V>
typename VectorTestBaseFixture<V>::value_type
VectorTestBaseFixture<V>::GetValue(int id) {
    return get_value<value_type>(id);
}

template<class V>
typename VectorTestBaseFixture<V>::vector
VectorTestBaseFixture<V>::GetVectorOfSize(size_type size) {
    vector v(size);
    for (size_type i = 0; i < size; ++i)
        v[i] = get_value<value_type>(i);

    return v;
}

template<class V>
void
VectorTestBaseFixture<V>::ExpectInsertedInAt(const vector& v, size_type pos,
        size_type count, const vector& save) {
    EXPECT_EQ(v.size(), save.size() + count);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < save.size(); ++i)
        EXPECT_EQ(v[count + i], save[i]);
}

template<class V>
void
VectorTestBaseFixture<V>::ExpectErasedInAt(const vector& v, size_type pos,
        size_type count, const vector& save) {
    EXPECT_EQ(v.size(), save.size() - count);
    for (size_type i = 0; i < pos; ++i)
        EXPECT_EQ(v[i], save[i]);
    for (size_type i = pos; i < v.size(); ++i)
        EXPECT_EQ(v[i], save[i + count]);
}

template<class V>
void
VectorTestBaseFixture<V>::InsertOneElementByCopy(vector& v, size_type pos, int id) {
    const vector save = v;
    const value_type val = get_value<value_type>(id);
    
    auto it = v.insert(v.begin() + pos, val);

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, val);
}

template<class V>
void
VectorTestBaseFixture<V>::InsertOneElementByMove(vector& v, size_type pos, int id) {
    const vector save = v;
    
    auto it = v.insert(v.begin() + pos, get_value<value_type>(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, get_value<value_type>(id));
}

template<class V>
void
VectorTestBaseFixture<V>::InsertMultipleElementsByFill(vector& v, size_type pos, size_type count, int id) {
    const vector save = v;
    const auto val = get_value<value_type>(id);

    auto it = v.insert(v.begin() + pos, count, val);

    ExpectInsertedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
    for (size_type i = 0; i < count; ++i)
        EXPECT_EQ(v[pos + i], val);
}

template<class V>
template<class InputIterator>
void
VectorTestBaseFixture<V>::InsertMultipleElementsByRange(vector& v, size_type pos,
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

template<class V>
void
VectorTestBaseFixture<V>::InsertMultipleElementsByInitializerList(vector& v, size_type pos,
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

template<class V>
void
VectorTestBaseFixture<V>::EmplaceAt(vector& v, size_type pos, int id) {
    const vector save = v;
    
    auto it = v.emplace(v.begin() + pos, get_value<value_type>(id));

    ExpectInsertedInAt(v, pos, 1, save);
    EXPECT_EQ(it, v.begin() + pos);
    EXPECT_EQ(*it, get_value<value_type>(id));
}

template<class V>
void
VectorTestBaseFixture<V>::EraseOneElement(vector& v, size_type pos) {
    const vector save = v;
    const size_type initial_size = v.size();

    auto it = v.erase(v.begin() + pos);

    ExpectErasedInAt(v, pos, pos == initial_size ? 0 : 1, save);
    EXPECT_EQ(it, v.begin() + pos);
}

template<class V>
void
VectorTestBaseFixture<V>::EraseMultipleElements(vector& v, size_type pos, size_type count) {
    ASSERT_LE(pos + count, v.size());

    const vector save = v;

    auto it = v.erase(v.begin() + pos, v.begin() + pos + count);

    ExpectErasedInAt(v, pos, count, save);
    EXPECT_EQ(it, v.begin() + pos);
}

class TypeNames {
public:
    template<class V>
    static std::string GetName(int id);
};

template<class V>
std::string TypeNames::GetName(int id) {
    using T = typename V::value_type;

    if (std::is_same_v<T, int>) return "int";
    if (std::is_same_v<T, std::string>) return "string";
    if (std::is_same_v<T, test_type>) return "test_type" + std::to_string(id);
    if (std::is_same_v<T, std::array<int, 10>>) return "array<int, 10>";

    return std::to_string(id);
}

using TestedTypes = ::testing::Types<
    tested_vector_t<int>,
    tested_vector_t<test_type>,
#if !(CPP_ABOVE_17) && !defined(DONT_COMPARE)
    compare_vector_t<test_type>,
#endif
    tested_vector_t<std::string>,
    tested_vector_t<std::array<int, 10>>
>;

TYPED_TEST_SUITE(VectorTestBaseFixture, TestedTypes, TypeNames);
