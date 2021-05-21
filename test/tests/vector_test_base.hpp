#pragma once

#include "vector_test_base_fixture.hpp"
#include <utils/utils.hpp>

template<>
void
VectorTestsBase<test_type>::SetUp() {
    test_type::instances = 0;
}

template<>
void
VectorTestsBase<test_type>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
}

template<>
std::string
VectorTestsBase<std::string>::GetValue(int id) {
    return "test" + std::to_string(id);
}

template<>
std::array<int, 10>
VectorTestsBase<std::array<int, 10>>::GetValue(int id) {
    std::array<int, 10> a;
    a.fill(id);

    return a;
}

TYPED_TEST(VectorTestsBase, DefaultConstructor) {
    typename TestFixture::vector v;

    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestsBase, CountConstructor) {
    typename TestFixture::vector v(5);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(VectorTestsBase, CountValueConstructor) {
    TypeParam val = this->GetValue(3);
    typename TestFixture::vector v(5, val);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestsBase, RangeConstructor) {
    const TypeParam a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    typename TestFixture::vector v(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(VectorTestsBase, CopyConstructor) {
    auto v1 = this->GetVectorOfSize(5);
    typename TestFixture::vector v2(v1);
    
    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(VectorTestsBase, MoveConstructor) {
    const TypeParam val = this->GetValue(10);
    const int size = 5;
    typename TestFixture::vector v1(size, val);
    typename TestFixture::vector v2(std::move(v1));
    
    EXPECT_EQ(v2.size(), size);
    for (const auto& x : v2)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestsBase, InitializerListConstructor) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2),
        this->GetValue(3), this->GetValue(4), this->GetValue(5)
    };
    typename TestFixture::vector v(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestsBase, CopyOperator) {
    auto v1 = this->GetVectorOfSize(5);
    typename TestFixture::vector v2(3);
    v2 = v1;

    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(VectorTestsBase, MoveOperator) {
    const TypeParam val = this->GetValue(10);
    const int size = 5;
    typename TestFixture::vector v1(size, val);
    typename TestFixture::vector v2(3);
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), size);
    for (typename TestFixture::size_type i = 0; i < size; ++i)
        EXPECT_EQ(v2[i], val);
}

TYPED_TEST(VectorTestsBase, InitializerListOperator) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    typename TestFixture::vector v(3);
    v = il;

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestsBase, BeginEndInEmptyVector) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(VectorTestsBase, BeginEndInEmptyConstVector) {
    const typename TestFixture::vector v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(VectorTestsBase, BeginEndMarkSize) {
    typename TestFixture::vector v(5);
    
    EXPECT_EQ(std::distance(v.begin(), v.end()), v.size());
    EXPECT_EQ(std::distance(v.cbegin(), v.cend()), v.size());
    EXPECT_EQ(std::distance(v.rbegin(), v.rend()), v.size());
    EXPECT_EQ(std::distance(v.crbegin(), v.crend()), v.size());
}

TYPED_TEST(VectorTestsBase, IteratorsPointCorrectly) {
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

TYPED_TEST(VectorTestsBase, IsSizeChanging) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.size(), 0);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 1);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 2);
    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestsBase, IsVectorInitiallyEmpty) {
    typename TestFixture::vector v;
    
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, IsNonEmptyVectorNonEmpty) {
    typename TestFixture::vector v(5);
    
    EXPECT_FALSE(v.empty());
}

TYPED_TEST(VectorTestsBase, DoesResizeChangeSize) {
    typename TestFixture::vector v;
    
    EXPECT_EQ(v.size(), 0);
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
}

TYPED_TEST(VectorTestsBase, ResizeEmptyWithDefaultConstruct) {
    typename TestFixture::vector v;

    v.resize(5);

    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(VectorTestsBase, ResizeEmptyWithFill) {
    const TypeParam val = this->GetValue(rand());
    typename TestFixture::vector v;

    v.resize(5, val);

    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestsBase, ResizeNonEmptyWithDefaultConstruct) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    v.resize(initial_size + 2);
    
    for (int i = 0; i < initial_size; ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
    for (int i = initial_size; i < initial_size + 2; ++i)
        EXPECT_EQ(v[i], TypeParam());

    v.resize(3);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], this->GetValue(i));
}

TYPED_TEST(VectorTestsBase, ResizeNonEmptyWithFill) {
    const int initial_size = 5;
    const TypeParam val = this->GetValue(13);
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

TYPED_TEST(VectorTestsBase, Reserve) {
    typename TestFixture::vector v;
    const typename TestFixture::size_type initial_size = v.size();
    
    for (typename TestFixture::size_type i = 1; i <= this->C; ++i) {
        v.reserve(i);
        EXPECT_EQ(v.size(), initial_size);
        EXPECT_GE(v.capacity(), i);
    }
}

TYPED_TEST(VectorTestsBase, ShrinkToFit) {
    typename TestFixture::vector v(5);
    v.shrink_to_fit();

    EXPECT_GE(v.capacity(), v.size());
}

TYPED_TEST(VectorTestsBase, AccessOperators) {
    auto v = this->GetVectorOfSize(5);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], this->GetValue(i));
        EXPECT_EQ(v.at(i), this->GetValue(i));
    }

    EXPECT_THROW(v.at(-1), std::out_of_range);
    EXPECT_THROW(v.at(v.size()), std::out_of_range);
}

TYPED_TEST(VectorTestsBase, FrontBack) {
    auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(VectorTestsBase, ConstFrontBack) {
    const auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(VectorTestsBase, Data) {
    auto v = this->GetVectorOfSize(5);
    TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(VectorTestsBase, ConstData) {
    const auto v = this->GetVectorOfSize(5);
    const TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(VectorTestsBase, RangeAssign) {
    const TypeParam a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    auto v = this->GetVectorOfSize(5);

    v.assign(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(VectorTestsBase, FillAssign) {
    auto v = this->GetVectorOfSize(3);
    const TypeParam val = this->GetValue(rand());
    typename TestFixture::size_type size = 5;
    
    v.assign(size, val);

    EXPECT_EQ(v.size(), size);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(VectorTestsBase, InitializerListAssign) {
    typename TestFixture::vector v(2);
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(13), this->GetValue(3)
    };
    
    v.assign(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(VectorTestsBase, PushBackByCopy) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        v.push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, PushBackByMove) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, FastPushBackByCopy) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (int i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        // v.fast_push_back(val);
        v.push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, FastPushBackByMove) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, EmplaceBack) {
    typename TestFixture::vector v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.emplace_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, FastEmplaceBack) {
    typename TestFixture::vector v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        // v.fast_emplace_back(val);
        v.emplace_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(VectorTestsBase, PopBack) {
    const int initial_size = 3;
    auto v = this->GetVectorOfSize(initial_size);

    for (int i = initial_size - 1; i >= 0; --i) {
        EXPECT_EQ(v.back(), this->GetValue(i));
        v.pop_back();
        EXPECT_EQ(v.size(), i);
    }

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, SafePopBack) {
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

TYPED_TEST(VectorTestsBase, PushPopBackMixed) {
    typename TestFixture::vector v;
    const TypeParam values[] = {
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

TYPED_TEST(VectorTestsBase, TwoSwap) {
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

TYPED_TEST(VectorTestsBase, OneSwap) {
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

TYPED_TEST(VectorTestsBase, StdTwoSwap) {
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

TYPED_TEST(VectorTestsBase, StdOneSwap) {
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

TYPED_TEST(VectorTestsBase, Clear) {
    auto v = this->GetVectorOfSize(5);
    
    v.clear();

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, InsertOneElementByCopyToEmptyVector) {
    typename TestFixture::vector v;

    this->InsertOneElementByCopy(v, 0, rand());
}

TYPED_TEST(VectorTestsBase, InsertOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByCopy(v, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByCopy(v, initial_size, 13);
}

TYPED_TEST(VectorTestsBase, InsertOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertOneElementByCopy(v, pos, 13);
}

TYPED_TEST(VectorTestsBase, InsertOneElementByMoveAtBeginToEmptyVector) {
    typename TestFixture::vector v;
    
    this->InsertOneElementByMove(v, 0, rand());
}

TYPED_TEST(VectorTestsBase, InsertOneElementByMoveAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByMove(v, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertOneElementByMoveAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByMove(v, initial_size, 13);
}

TYPED_TEST(VectorTestsBase, InsertOneElementByMoveInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    
    this->InsertOneElementByMove(v, pos, 13);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByFillToEmptyVector) {
    typename TestFixture::vector v;

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByFillAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, 0, 13);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByFillToEmptyVector) {
    typename TestFixture::vector v;
    int count = Random::rand<int>(1, this->C);

    this->InsertMultipleElementsByFill(v, 0, count, rand());
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByFillAtBeginToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, 0, count, 13);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, count, 13);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, count, 13);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByRangeToEmptyVector) {
    typename TestFixture::vector v;
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, initial_size, a, a);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, pos, a, a);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, 0, a, a + count);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);

    this->InsertMultipleElementsByRange(v, initial_size, a, a + count);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, pos, a, a + count);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByInitializerListToEmptyVector) {
    typename TestFixture::vector v;
    std::initializer_list<TypeParam> ilist;
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(VectorTestsBase, InsertZeroElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };

    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(VectorTestsBase, InsertMultipleElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(VectorTestsBase, EmplaceOneElementByCopyToEmptyVector) {
    typename TestFixture::vector v;

    this->EmplaceAt(v, 0, rand());
}

TYPED_TEST(VectorTestsBase, EmplaceOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->EmplaceAt(v, 0, 13);
}

TYPED_TEST(VectorTestsBase, EmplaceOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->EmplaceAt(v, initial_size, 13);
}

TYPED_TEST(VectorTestsBase, EmplaceOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->EmplaceAt(v, pos, 13);
}

TYPED_TEST(VectorTestsBase, EraseBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->EraseOneElement(v, 0);
}

TYPED_TEST(VectorTestsBase, EraseEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseOneElement(v, initial_size - 1);
}

TYPED_TEST(VectorTestsBase, EraseZeroElementsFromEmptyVector) {
    typename TestFixture::vector v;
    
    this->EraseMultipleElements(v, 0, 0);
}

TYPED_TEST(VectorTestsBase, EraseZeroElementsAtBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    
    this->EraseMultipleElements(v, 0, 0);
}

TYPED_TEST(VectorTestsBase, EraseZeroElemensAtEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseMultipleElements(v, initial_size, 0);
}

TYPED_TEST(VectorTestsBase, EraseAllElementsFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    
    this->EraseMultipleElements(v, 0, initial_size);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, EraseMultipleElementsAtBeginFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand(1, initial_size - 1);
    
    this->EraseMultipleElements(v, 0, count);
}

TYPED_TEST(VectorTestsBase, EraseMultipleElementsToEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    
    this->EraseMultipleElements(v, pos, initial_size - pos);
}

TYPED_TEST(VectorTestsBase, EraseMutipleElementsInMiddleFromNonEmptyVector) {
    const int initial_size = 7;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 2);
    const int count = Random::rand(1, initial_size - 1 - pos);

    this->EraseMultipleElements(v, pos, count);
}

#if CPP_ABOVE_17

TYPED_TEST(VectorTestsBase, EraseByValueFromEmptyVector) {
    typename TestFixture::vector v;
    
    auto count = std::erase(v, this->GetValue(0));

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, EraseByValueNothingFromNonEmptyVector) {
    const int initial_size =  5;
    auto v = this->GetVectorOfSize(initial_size);

    auto count = std::erase(v, this->GetValue(13));

    EXPECT_EQ(count, 0);
    EXPECT_EQ(v.size(), initial_size);
}

TYPED_TEST(VectorTestsBase, EraseByValueOneValueFromBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(0));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 0, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseByValueOneValueFromEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(4));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, initial_size - 1, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseByValueOneValueInMiddleFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase(v, this->GetValue(2));

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 2, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseByValueMultipleValuesInOneBlockFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(2), this->GetValue(2), this->GetValue(2)
    };
    const auto save = v;

    auto count = std::erase(v, this->GetValue(2));

    ASSERT_EQ(count, 3);
    this->ExpectErasedInAt(v, 1, 3, save);
}

TYPED_TEST(VectorTestsBase, EraseByValueMultipleValuesScatteredFromNonEmptyVector) {
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

TYPED_TEST(VectorTestsBase, EraseIfByValueFromEmptyVector) {
    typename TestFixture::vector v;
    
    auto count = std::erase_if(v, [](const TypeParam&){
        return false;
    });

    EXPECT_EQ(count, 0);
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestsBase, EraseIfByValueNothingFromNonEmptyVector) {
    const int initial_size =  5;
    auto v = this->GetVectorOfSize(initial_size);

    auto count = std::erase_if(v, [this](const TypeParam& x) {
        return x == this->GetValue(13);
    });

    EXPECT_EQ(count, 0);
    EXPECT_EQ(v.size(), initial_size);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueOneValueFromBeginFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x == this->GetValue(0);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 0, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueOneValueFromEndFromNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const auto save = v;

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x == this->GetValue(4);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, initial_size - 1, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueOneValueInMiddleFromNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);
    const auto save = v;

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x == this->GetValue(2);
    });

    ASSERT_EQ(count, 1);
    this->ExpectErasedInAt(v, 2, 1, save);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueMultipleValuesInOneBlockFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(2), this->GetValue(2), this->GetValue(2)
    };
    const auto save = v;

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x == this->GetValue(2);
    });

    ASSERT_EQ(count, 3);
    this->ExpectErasedInAt(v, 1, 3, save);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueMultipleValuesScatteredFromNonEmptyVector) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(0), this->GetValue(13),
        this->GetValue(4), this->GetValue(0), this->GetValue(1),
        this->GetValue(0), this->GetValue(1), this->GetValue(0)
    };
    const typename TestFixture::vector expected_result {
        this->GetValue(13), this->GetValue(4), this->GetValue(1),
        this->GetValue(1)
    };

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x == this->GetValue(0);
    });

    ASSERT_EQ(count, 5);
    EXPECT_EQ(v, expected_result);
}

TYPED_TEST(VectorTestsBase, EraseIfByValueMultipleValuesScatteredFromNonEmptyVector2) {
    typename TestFixture::vector v {
        this->GetValue(0), this->GetValue(0), this->GetValue(13),
        this->GetValue(4), this->GetValue(0), this->GetValue(1),
        this->GetValue(0), this->GetValue(1), this->GetValue(0)
    };

    auto count = std::erase_if(v, [this](const TypeParam& x){
        return x != this->GetValue(0);
    });

    ASSERT_EQ(count, 4);
    ASSERT_EQ(v.size(), 5);
    EXPECT_TRUE(std::all_of(v.begin(), v.end(), [this](const TypeParam& x){
        return x == this->GetValue(0);
    }));
}

#endif // CPP_ABOVE_17

TYPED_TEST(VectorTestsBase, EmptyVectorsAreEqual) {
    typename TestFixture::vector v1, v2;
    
    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestsBase, EqualIndependentNonEmptyVectorsAreEqual) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(13)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(13)
    };

    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestsBase, CopiedVectorIsEqualToOriginal) {
    const int initial_size = Random::rand(2, 8);
    typename TestFixture::vector v1;
    for (int i = 0; i < initial_size; ++i)
        v1.push_back(this->GetValue(i));
    auto v2 = v1;

    EXPECT_TRUE(v1 == v2);
}

TYPED_TEST(VectorTestsBase, EmptyVectorsAreNotDifferent) {
    typename TestFixture::vector v1, v2;
    
    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestsBase, EqualIndependentNonEmptyVectorsAreNotDifferent) {
    typename TestFixture::vector v1 {
        this->GetValue(0), this->GetValue(13)
    };
    typename TestFixture::vector v2 {
        this->GetValue(0), this->GetValue(13)
    };

    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestsBase, CopiedVectorIsNotDifferentToOriginal) {
    const int initial_size = Random::rand(2, 8);
    typename TestFixture::vector v1;
    for (int i = 0; i < initial_size; ++i)
        v1.push_back(this->GetValue(i));
    auto v2 = v1;

    EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestsBase, DifferentVectorsAreDifferentWhereOneIsEmpty) {
    typename TestFixture::vector v1;
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE(v1 != v2);
    EXPECT_TRUE(v2 != v1);
}

TYPED_TEST(VectorTestsBase, DifferentVectorsOfTheSameSizeAreDifferent) {
    typename TestFixture::vector v1 {
        this->GetValue(2), this->GetValue(1), this->GetValue(0)
    };
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE(v1 != v2);
}

TYPED_TEST(VectorTestsBase, RelationalOperatorsTwoEmptyVectors) {
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

TYPED_TEST(VectorTestsBase, RelationalOperatorsWithOneEmptyVector) {
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

TYPED_TEST(VectorTestsBase, RelationalOperatorsWithTwoNonEmptyEqualVectors) {
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

TYPED_TEST(VectorTestsBase, RelationalOperatorsWithTwoNonEmptyEqualSizedVectors) {
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

TYPED_TEST(VectorTestsBase, RelationalOperatorsWithTwoNonEmptyDifferentSizedVectorsOnePrefixOfAnother) {
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

TYPED_TEST(VectorTestsBase, RelationalOperatorsWithTwoNonEmptyDifferentVectors) {
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

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfEmptyVectors) {
    typename TestFixture::vector v1;
    typename TestFixture::vector v2;

    EXPECT_TRUE((v1 <=> v2) == 0);
    EXPECT_TRUE((v2 <=> v1) == 0);
}

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfOneEmpty) {
    typename TestFixture::vector v1;
    auto v2 = this->GetVectorOfSize(3);
    
    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfNonEmptyEqualVectors) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(3);

    EXPECT_TRUE((v1 <=> v2) == 0);
    EXPECT_TRUE((v2 <=> v1) == 0);
}

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfNonEmptyEqualSizedVectors) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(3);
    v2[1] = this->GetValue(3);

    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfNonEmptyDifferentSizedVectorsOnePrefixOfOther) {
    auto v1 = this->GetVectorOfSize(3);
    auto v2 = this->GetVectorOfSize(4);

    EXPECT_TRUE((v1 <=> v2) < 0);
    EXPECT_TRUE((v2 <=> v1) > 0);
}

TYPED_TEST(VectorTestsBase, ThreeWayCompareOfNonEmptyDifferentVectors) {
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
