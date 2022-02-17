#pragma once

#include <list>

#include <gtest/gtest.h>

#include "uwr/common/define.hpp"
#include "uwr/common/types.hpp"

#include "test_type/test_type.hpp"
#include "utils/value.hpp"

template<class V>
class VectorTestBaseFixture : public ::testing::Test {
public:
    using vector = V;
    using value_type = typename vector::value_type;
    using size_type = typename vector::size_type;

    static constexpr uwr::u64 C = 10;
    static constexpr uwr::u64 SMALL_SIZE = 5;
    static constexpr uwr::u64 MEDIUM_SIZE = 100;
    static constexpr uwr::u64 BIG_SIZE = 5000;

    static_assert(2 * SMALL_SIZE <= C,
            "SMALL_SIZE should be small enough to fit into static_vector of capacity C twice!");
    static_assert(SMALL_SIZE > 2,
            "SMALL_SIZE should be more than 2!");

    // in ascending order
    static constexpr uwr::u64 ALL_SIZES[] = {
        SMALL_SIZE, MEDIUM_SIZE, BIG_SIZE
    };

protected:
    void SetUp() {}
    void TearDown() {}

    size_type GetMaxVectorSize();
    std::vector<size_type> GetAllSizes();

    value_type GetValue(int id);
    vector GetVectorOfSize(size_type size);

    std::unique_ptr<value_type[]> GetInitializedArrayOfSize(size_type size);
    std::vector<value_type> GetInitializedVectorOfSize(size_type size);
    std::list<value_type> GetInitializedListOfSize(size_type size);

    void InsertOneElementByCopy(vector& v, size_type pos, int id);
    void InsertOneElementByMove(vector& v, size_type pos, int id);
    void InsertMultipleElementsByFill(vector& v, size_type pos,
            size_type count, int id);
    template<class InIt>
    void InsertMultipleElementsByRange(vector& v, size_type pos,
            InIt begin, InIt end);
    void InsertMultipleElementsByInitializerList(vector& v, size_type pos,
            std::initializer_list<value_type> ilist);

    void EmplaceAt(vector& v, size_type pos, int id);

    void EraseOneElement(vector& v, size_type pos);
    void EraseMultipleElements(vector& v, size_type pos, size_type count);

    void ExpectErasedInAt(const vector& v, size_type pos,
            size_type count, const vector& save);

    void ExpectInsertedInAt(const vector& v, size_type pos,
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
typename VectorTestBaseFixture<V>::size_type
VectorTestBaseFixture<V>::GetMaxVectorSize() {
    const vector v;
    return v.max_size();
}

template<class V>
std::vector<typename VectorTestBaseFixture<V>::size_type>
VectorTestBaseFixture<V>::GetAllSizes() {
    std::vector<size_type> all_sizes;
    size_type max_size = GetMaxVectorSize();

    for (const auto& size : ALL_SIZES)
        if (size <= max_size)
            all_sizes.emplace_back(size);
        else
            break;

    return all_sizes;
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
        v[i] = GetValue(i);

    return v;
}

template<class V>
std::unique_ptr<typename VectorTestBaseFixture<V>::value_type[]>
VectorTestBaseFixture<V>::GetInitializedArrayOfSize(size_type size) {
    auto arr = std::make_unique<value_type[]>(size);
    for (size_type i = 0; i < size; ++i)
        arr[i] = GetValue(i);

    return arr;
}

template<class V>
std::vector<typename VectorTestBaseFixture<V>::value_type>
VectorTestBaseFixture<V>::GetInitializedVectorOfSize(size_type size) {
    std::vector<value_type> v;
    v.reserve(size);

    for (size_type i = 0; i < size; ++i)
        v.push_back(GetValue(i));

    return v;
}

template<class V>
std::list<typename VectorTestBaseFixture<V>::value_type>
VectorTestBaseFixture<V>::GetInitializedListOfSize(size_type size) {
    std::list<value_type> l;
    for (size_type i = 0; i < size; ++i)
        l.push_back(GetValue(i));

    return l;
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
template<class InIt>
void
VectorTestBaseFixture<V>::InsertMultipleElementsByRange(vector& v, size_type pos,
        InIt begin, InIt end) {
    const vector save = v;
    const size_type count = std::distance(begin, end);
    value_type save_range[count];
    std::copy(begin, end, save_range);

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
