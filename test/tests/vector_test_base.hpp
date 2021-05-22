#pragma once

#include "vector_test_base_fixture.hpp"
#include <utils/utils.hpp>

template<>
void
VectorTestBaseFixture<tested_vector_t<test_type>>::SetUp() {
    test_type::instances = 0;
}

template<>
void
VectorTestBaseFixture<tested_vector_t<test_type>>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
}

template<>
void
VectorTestBaseFixture<compare_vector_t<test_type>>::SetUp() {
    test_type::instances = 0;
}

template<>
void
VectorTestBaseFixture<compare_vector_t<test_type>>::TearDown() {
    test_type::instances = 0;
}

TYPED_TEST(VectorTestBaseFixture, DefaultConstructor) {
    typename TestFixture::vector v;

    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestBaseFixture, CountConstructor) {
    typename TestFixture::vector v(5);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, typename TestFixture::value_type());
}

TYPED_TEST(VectorTestBaseFixture, CountValueConstructor) {
    auto val = this->GetValue(3);
    typename TestFixture::vector v(5, val);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestBaseFixture, RangeConstructor) {
    const typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    typename TestFixture::vector v(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(VectorTestBaseFixture, CopyConstructor) {
    auto v1 = this->GetVectorOfSize(5);
    typename TestFixture::vector v2(v1);
    
    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(VectorTestBaseFixture, MoveConstructor) {
    const auto val = this->GetValue(10);
    const int size = 5;
    typename TestFixture::vector v1(size, val);
    typename TestFixture::vector v2(std::move(v1));
    
    EXPECT_EQ(v2.size(), size);
    for (const auto& x : v2)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestBaseFixture, InitializerListConstructor) {
    auto il = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2),
        this->GetValue(3), this->GetValue(4), this->GetValue(5)
    };
    typename TestFixture::vector v(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestBaseFixture, CopyOperator) {
    auto v1 = this->GetVectorOfSize(5);
    typename TestFixture::vector v2(3);
    v2 = v1;

    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(VectorTestBaseFixture, MoveOperator) {
    const auto val = this->GetValue(10);
    const int size = 5;
    typename TestFixture::vector v1(size, val);
    typename TestFixture::vector v2(3);
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), size);
    for (typename TestFixture::size_type i = 0; i < size; ++i)
        EXPECT_EQ(v2[i], val);
}

TYPED_TEST(VectorTestBaseFixture, InitializerListOperator) {
    auto il = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    typename TestFixture::vector v(3);
    v = il;

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestBaseFixture, BeginEndInEmptyVector) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(VectorTestBaseFixture, BeginEndInEmptyConstVector) {
    const typename TestFixture::vector v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(VectorTestBaseFixture, BeginEndMarkSize) {
    typename TestFixture::vector v(5);
    
    EXPECT_EQ(std::distance(v.begin(), v.end()), v.size());
    EXPECT_EQ(std::distance(v.cbegin(), v.cend()), v.size());
    EXPECT_EQ(std::distance(v.rbegin(), v.rend()), v.size());
    EXPECT_EQ(std::distance(v.crbegin(), v.crend()), v.size());
}

TYPED_TEST(VectorTestBaseFixture, IteratorsPointCorrectly) {
    const typename TestFixture::size_type size = 5;
    auto v = this->GetVectorOfSize(size);
    int i;
    
    i = 0;
    for (auto it = v.begin(); it != v.end(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(i));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.cbegin(); it != v.cend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(i));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(size - i - 1));
    EXPECT_EQ(i, v.size());

    i = 0;
    for (auto it = v.crbegin(); it != v.crend(); ++it, ++i)
        EXPECT_EQ(*it, this->GetValue(size - i - 1));
    EXPECT_EQ(i, v.size());
}

TYPED_TEST(VectorTestBaseFixture, IsSizeChanging) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.size(), 0);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 1);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 2);
    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestBaseFixture, IsVectorInitiallyEmpty) {
    typename TestFixture::vector v;
    
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, IsNonEmptyVectorNonEmpty) {
    typename TestFixture::vector v(5);
    
    EXPECT_FALSE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, DoesResizeChangeSize) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.size(), 0);
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
}

TYPED_TEST(VectorTestBaseFixture, ResizeEmptyWithDefaultConstruct) {
    typename TestFixture::vector v;

    v.resize(5);

    for (const auto& x : v)
        EXPECT_EQ(x, typename TestFixture::value_type());
}

TYPED_TEST(VectorTestBaseFixture, ResizeEmptyWithFill) {
    const auto val = this->GetValue(rand());
    typename TestFixture::vector v;

    v.resize(5, val);

    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestBaseFixture, ResizeNonEmptyWithDefaultConstruct) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    v.resize(initial_size + 2);
    
    for (int i = 0; i < initial_size; ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
    for (int i = initial_size; i < initial_size + 2; ++i)
        EXPECT_EQ(v[i], typename TestFixture::value_type());

    v.resize(3);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
}

TYPED_TEST(VectorTestBaseFixture, ResizeNonEmptyWithFill) {
    const int initial_size = 5;
    const auto val = this->GetValue(13);
    auto v = this->GetVectorOfSize(initial_size);

    v.resize(initial_size + 2, val);
    
    for (int i = 0; i < initial_size; ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
    for (int i = initial_size; i < initial_size + 2; ++i)
        EXPECT_EQ(v[i], val);

    v.resize(3);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
}

TYPED_TEST(VectorTestBaseFixture, Reserve) {
    typename TestFixture::vector v;
    const typename TestFixture::size_type initial_size = v.size();
    
    for (typename TestFixture::size_type i = 1; i <= this->C; ++i) {
        v.reserve(i);
        EXPECT_EQ(v.size(), initial_size);
        EXPECT_GE(v.capacity(), i);
    }
}

TYPED_TEST(VectorTestBaseFixture, ShrinkToFit) {
    typename TestFixture::vector v(5);
    v.shrink_to_fit();

    EXPECT_GE(v.capacity(), v.size());
}

TYPED_TEST(VectorTestBaseFixture, AccessOperators) {
    auto v = this->GetVectorOfSize(5);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], this->GetValue(i));
        EXPECT_EQ(v.at(i), this->GetValue(i));
    }

    EXPECT_THROW(v.at(-1), std::out_of_range);
    EXPECT_THROW(v.at(v.size()), std::out_of_range);
}

TYPED_TEST(VectorTestBaseFixture, FrontBack) {
    auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(VectorTestBaseFixture, ConstFrontBack) {
    const auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(VectorTestBaseFixture, Data) {
    auto v = this->GetVectorOfSize(5);
    typename TestFixture::value_type* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(VectorTestBaseFixture, ConstData) {
    const auto v = this->GetVectorOfSize(5);
    const auto* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(VectorTestBaseFixture, RangeAssign) {
    const typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    auto v = this->GetVectorOfSize(5);

    v.assign(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(VectorTestBaseFixture, FillAssign) {
    auto v = this->GetVectorOfSize(3);
    const auto val = this->GetValue(rand());
    typename TestFixture::size_type size = 5;
    
    v.assign(size, val);

    EXPECT_EQ(v.size(), size);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestBaseFixture, InitializerListAssign) {
    typename TestFixture::vector v(2);
    auto il = {
        this->GetValue(0), this->GetValue(13), this->GetValue(3)
    };
    
    v.assign(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestBaseFixture, PushBackByCopy) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const auto val = this->GetValue(id);
        v.push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, PushBackByMove) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, FastPushBackByCopy) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (int i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const auto val = this->GetValue(id);
        // v.fast_push_back(val);
        v.push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, FastPushBackByMove) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        // v.fast_push_back(this->GetValue(id));
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceBack) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.emplace_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, FastEmplaceBack) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const auto val = this->GetValue(id);
        // v.fast_emplace_back(val);
        v.emplace_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestBaseFixture, PopBack) {
    const int initial_size = 3;
    auto v = this->GetVectorOfSize(initial_size);

    for (int i = initial_size - 1; i >= 0; --i) {
        EXPECT_EQ(v.back(), this->GetValue(i));
        v.pop_back();
        EXPECT_EQ(v.size(), i);
    }

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, SafePopBack) {
    const int initial_size = 3;
    auto v = this->GetVectorOfSize(initial_size);

    for (int i = initial_size - 1; i >= 0; --i) {
        EXPECT_EQ(v.back(), this->GetValue(i));
        // v.safe_pop_back();
        v.pop_back();
        EXPECT_EQ(v.size(), i);
    }

    EXPECT_TRUE(v.empty());
    // EXPECT_NO_THROW(v.safe_pop_back());
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestBaseFixture, PushPopBackMixed) {
    typename TestFixture::vector v;
    const typename TestFixture::value_type values[] = {
        this->GetValue(13), this->GetValue(81), this->GetValue(12)
    };
    
    v.push_back(values[0]);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.back(), values[0]);

    v.push_back(values[1]);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), values[1]);

    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.back(), values[0]);

    v.pop_back();
    v.push_back(values[2]);
    v.push_back(values[2]);
    v.push_back(values[2]);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back(), values[2]);

    v.pop_back();

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), values[2]);

    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.back(), values[2]);

    v.pop_back();

    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestBaseFixture, TwoSwap) {
    const int v1_size = 5;
    const int v2_size = 3;
    auto v1 = this->GetVectorOfSize(v1_size);
    auto v2 = this->GetVectorOfSize(v2_size);

    std::reverse(v1.begin(), v1.end());
    v1.swap(v2);
    v2.swap(v1);

    EXPECT_EQ(v1.size(), v1_size);
    EXPECT_EQ(v2.size(), v2_size);
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], this->GetValue(v1.size() - 1 - i));
    for (typename TestFixture::size_type i = 0; i < v2.size(); ++i)
        EXPECT_EQ(v2[i], this->GetValue(i));
}

TYPED_TEST(VectorTestBaseFixture, OneSwap) {
    const int v1_size = 5;
    const int v2_size = 3;
    auto v1 = this->GetVectorOfSize(v1_size);
    auto v2 = this->GetVectorOfSize(v2_size);

    std::reverse(v1.begin(), v1.end());
    v1.swap(v2);

    EXPECT_EQ(v1.size(), v2_size);
    EXPECT_EQ(v2.size(), v1_size);
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], this->GetValue(i));
    for (typename TestFixture::size_type i = 0; i < v2.size(); ++i)
        EXPECT_EQ(v2[i], this->GetValue(v2.size() - 1 - i));
}

TYPED_TEST(VectorTestBaseFixture, StdTwoSwap) {
    const int v1_size = 5;
    const int v2_size = 3;
    auto v1 = this->GetVectorOfSize(v1_size);
    auto v2 = this->GetVectorOfSize(v2_size);

    std::reverse(v1.begin(), v1.end());
    std::swap(v1, v2);
    std::swap(v2, v1);

    EXPECT_EQ(v1.size(), v1_size);
    EXPECT_EQ(v2.size(), v2_size);
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], this->GetValue(v1.size() - 1 - i));
    for (typename TestFixture::size_type i = 0; i < v2.size(); ++i)
        EXPECT_EQ(v2[i], this->GetValue(i));
}

TYPED_TEST(VectorTestBaseFixture, StdOneSwap) {
    const int v1_size = 5;
    const int v2_size = 3;
    auto v1 = this->GetVectorOfSize(v1_size);
    auto v2 = this->GetVectorOfSize(v2_size);

    std::reverse(v1.begin(), v1.end());
    std::swap(v1, v2);

    EXPECT_EQ(v1.size(), v2_size);
    EXPECT_EQ(v2.size(), v1_size);
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], this->GetValue(i));
    for (typename TestFixture::size_type i = 0; i < v2.size(); ++i)
        EXPECT_EQ(v2[i], this->GetValue(v2.size() - 1 - i));
}

TYPED_TEST(VectorTestBaseFixture, Clear) {
    auto v = this->GetVectorOfSize(5);
    
    v.clear();

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyToEmptyVector) {
    typename TestFixture::vector v;

    this->InsertOneElementByCopy(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByCopy(v, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByCopy(v, initial_size, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertOneElementByCopy(v, pos, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtBeginToEmptyVector) {
    typename TestFixture::vector v;
    
    this->InsertOneElementByMove(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByMove(v, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByMove(v, initial_size, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    
    this->InsertOneElementByMove(v, pos, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillToEmptyVector) {
    typename TestFixture::vector v;

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillToEmptyVector) {
    typename TestFixture::vector v;
    int count = Random::rand<int>(1, this->C);

    this->InsertMultipleElementsByFill(v, 0, count, rand());
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillAtBeginToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, 0, count, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, count, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, count, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeToEmptyVector) {
    typename TestFixture::vector v;
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, initial_size, a, a);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, pos, a, a);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, 0, a, a + count);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);

    this->InsertMultipleElementsByRange(v, initial_size, a, a + count);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    typename TestFixture::value_type a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, pos, a, a + count);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListToEmptyVector) {
    typename TestFixture::vector v;
    std::initializer_list<typename TestFixture::value_type> ilist;
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };

    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    auto ilist = {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyToEmptyVector) {
    typename TestFixture::vector v;

    this->EmplaceAt(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->EmplaceAt(v, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->EmplaceAt(v, initial_size, 13);
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->EmplaceAt(v, pos, 13);
}

TYPED_TEST(VectorTestBaseFixture, EraseBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->EraseOneElement(v, 0);
}

TYPED_TEST(VectorTestBaseFixture, EraseEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseOneElement(v, initial_size - 1);
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElementsFromEmptyVector) {
    typename TestFixture::vector v;
    
    this->EraseMultipleElements(v, 0, 0);
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElementsAtBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    
    this->EraseMultipleElements(v, 0, 0);
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElemensAtEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseMultipleElements(v, initial_size, 0);
}

TYPED_TEST(VectorTestBaseFixture, EraseAllElementsFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseMultipleElements(v, 0, initial_size);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, EraseMultipleElementsAtBeginFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand(1, initial_size - 1);
    
    this->EraseMultipleElements(v, 0, count);
}

TYPED_TEST(VectorTestBaseFixture, EraseMultipleElementsToEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    
    this->EraseMultipleElements(v, pos, initial_size - pos);
}

TYPED_TEST(VectorTestBaseFixture, EraseMutipleElementsInMiddleFromNonEmptyVector) {
    const int initial_size = 7;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 2);
    const int count = Random::rand(1, initial_size - 1 - pos);

    this->EraseMultipleElements(v, pos, count);
}

#if CPP_ABOVE_17

TYPED_TEST(VectorTestBaseFixture, EraseByValueFromEmptyVector) {
    typename TestFixture::vector v;
    
    auto count = std::erase(v, this->GetValue(0));

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueNothingFromNonEmptyVector) {
    const int initial_size =  5;
    auto v = this->GetVectorOfSize(initial_size);

    auto count = std::erase(v, this->GetValue(13));

    EXPECT_EQ(count, 0);
    EXPECT_EQ(v.size(), initial_size);
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueFromBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(0));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 0, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueFromEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(4));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, initial_size - 1, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueInMiddleFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(2));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 2, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueMultipleValuesInOneBlockFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(2), this->GetValue(2), this->GetValue(2)
    };
    const auto save = v;

    auto count = std::erase(v, this->GetValue(2));

    ASSERT_EQ(count, 3);
    this->ExpectErasedInAt(v, 1, 3, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueMultipleValuesScatteredFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(0), this->GetValue(13),
        this->GetValue(4), this->GetValue(0), this->GetValue(1),
        this->GetValue(0), this->GetValue(1), this->GetValue(0)
    };
    const typename TestFixture::vector expected_result {
        this->GetValue(13), this->GetValue(4), this->GetValue(1),
        this->GetValue(1)
    };

    auto count = std::erase(v, this->GetValue(0));

    ASSERT_EQ(count, 5);
    EXPECT_EQ(v, expected_result);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueFromEmptyVector) {
    typename TestFixture::vector v;
    
    auto count = std::erase_if(v, [](const typename TestFixture::value_type&){
        return false;
    });

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueNothingFromNonEmptyVector) {
    const int initial_size =  5;
    auto v = this->GetVectorOfSize(initial_size);

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x) {
        return x == this->GetValue(13);
    });

    EXPECT_EQ(count, 0);
    EXPECT_EQ(v.size(), initial_size);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueFromBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(0);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 0, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueFromEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const auto save = v;

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(4);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, initial_size - 1, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueInMiddleFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(2);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 2, 1, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesInOneBlockFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(2), this->GetValue(2), this->GetValue(2)
    };
    const auto save = v;

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(2);
    });

    ASSERT_EQ(count, 3);
    this->ExpectErasedInAt(v, 1, 3, save);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesScatteredFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(0), this->GetValue(13),
        this->GetValue(4), this->GetValue(0), this->GetValue(1),
        this->GetValue(0), this->GetValue(1), this->GetValue(0)
    };
    const typename TestFixture::vector expected_result {
        this->GetValue(13), this->GetValue(4), this->GetValue(1),
        this->GetValue(1)
    };

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(0);
    });

    ASSERT_EQ(count, 5);
    EXPECT_EQ(v, expected_result);
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesScatteredFromNonEmptyVector2) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(0), this->GetValue(13),
        this->GetValue(4), this->GetValue(0), this->GetValue(1),
        this->GetValue(0), this->GetValue(1), this->GetValue(0)
    };

    auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
        return x != this->GetValue(0);
    });

    ASSERT_EQ(count, 4);
    ASSERT_EQ(v.size(), 5);
    EXPECT_TRUE(std::all_of(v.begin(), v.end(), [this](const typename TestFixture::value_type& x){
        return x == this->GetValue(0);
    }));
}

#endif // CPP_ABOVE_17

TYPED_TEST(VectorTestBaseFixture, EmptyVectorsAreEqual) {
    typename TestFixture::vector v1, v2;
    
    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestBaseFixture, EqualIndependentNonEmptyVectorsAreEqual) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(13)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(13)
    };

    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestBaseFixture, CopiedVectorIsEqualToOriginal) {
    const int initial_size = Random::rand(2, 8);
    typename TestFixture::vector v1;
    for (int i = 0; i < initial_size; ++i)
        v1.push_back(this->GetValue(i));
    auto v2 = v1;

    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestBaseFixture, EmptyVectorsAreNotDifferent) {
    typename TestFixture::vector v1, v2;
    
    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestBaseFixture, EqualIndependentNonEmptyVectorsAreNotDifferent) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(13)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(13)
    };

    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestBaseFixture, CopiedVectorIsNotDifferentToOriginal) {
    const int initial_size = Random::rand(2, 8);
    typename TestFixture::vector v1;
    for (int i = 0; i < initial_size; ++i)
        v1.push_back(this->GetValue(i));
    auto v2 = v1;

    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestBaseFixture, DifferentVectorsAreDifferentWhereOneIsEmpty) {
    typename TestFixture::vector v1;
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v2 != v1);
}

TYPED_TEST(VectorTestBaseFixture, DifferentVectorsOfTheSameSizeAreDifferent) {
    typename TestFixture::vector v1 {
        this->GetValue(2), this->GetValue(1), this->GetValue(0)
    };
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE(v1 != v2);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsTwoEmptyVectors) {
    typename TestFixture::vector v1, v2;
    
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v2 <= v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_FALSE(v1 < v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v2 > v1);
    EXPECT_FALSE(v2 > v1);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithOneEmptyVector) {
    typename TestFixture::vector v1; 
    auto v2 = this->GetVectorOfSize(5);
 
    EXPECT_TRUE(v1 < v2);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v1 >= v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_FALSE(v2 <= v1);
    EXPECT_TRUE(v2 > v1);
    EXPECT_TRUE(v2 >= v1);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyEqualVectors) {
    auto v1 = this->GetVectorOfSize(5);
    auto v2 = this->GetVectorOfSize(5);

    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v2 <= v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_FALSE(v1 < v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_FALSE(v2 > v1);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyEqualSizedVectors) {
    auto v1 = this->GetVectorOfSize(5);
    v1[2] = this->GetValue(1);
    auto v2 = this->GetVectorOfSize(5);

    EXPECT_TRUE(v1 <= v2);
    EXPECT_FALSE(v1 >= v2);
    EXPECT_FALSE(v2 <= v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_TRUE(v1 < v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_TRUE(v2 > v1);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyDifferentSizedVectorsOnePrefixOfAnother) {
    auto v1 = this->GetVectorOfSize(4);
    auto v2 = this->GetVectorOfSize(5);

    EXPECT_TRUE(v1 <= v2);
    EXPECT_FALSE(v1 >= v2);
    EXPECT_FALSE(v2 <= v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_TRUE(v1 < v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_TRUE(v2 > v1);
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyDifferentVectors) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(2), this->GetValue(1)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(9)
    };
    
    EXPECT_TRUE(v1 <= v2);
    EXPECT_FALSE(v1 >= v2);
    EXPECT_FALSE(v2 <= v1);
    EXPECT_TRUE(v2 >= v1);
    EXPECT_TRUE(v1 < v2);
    EXPECT_FALSE(v1 > v2);
    EXPECT_FALSE(v2 < v1);
    EXPECT_TRUE(v2 > v1);
}

#if CPP_ABOVE_17

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfEmptyVectors) {
    typename TestFixture::vector v1;
    typename TestFixture::vector v2;

    EXPECT_TRUE((v1 <=> v2) == 0);
    EXPECT_TRUE((v2 <=> v1) == 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfOneEmpty) {
    typename TestFixture::vector v1;
    auto v2 = this->GetVectorOfSize(3);
    
    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyEqualVectors) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE((v1 <=> v2) == 0);
    EXPECT_TRUE((v2 <=> v1) == 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyEqualSizedVectors) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(3);
    v2[1] = this->GetValue(3);

    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyDifferentSizedVectorsOnePrefixOfOther) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(4);

    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyDifferentVectors) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(2), this->GetValue(1)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(9)
    };

    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

#endif // CPP_ABOVE_17
