#include "static_vector_tests.hpp"
#include <utils/utils.hpp>

template<>
void
StaticVectorTests<test_type>::SetUp() {
    test_type::instances = 0;
}

template<>
void
StaticVectorTests<test_type>::TearDown() {
    EXPECT_EQ(test_type::instances, 0);
}

template<>
std::string
StaticVectorTests<std::string>::GetValue(int id) {
    return "test" + std::to_string(id);
}

template<>
std::array<int, 10>
StaticVectorTests<std::array<int, 10>>::GetValue(int id) {
    std::array<int, 10> a;
    a.fill(id);

    return a;
}

TYPED_TEST(StaticVectorTests, DefaultConstructor) {
    static_vector<TypeParam, this->C> v;

    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(StaticVectorTests, CountConstructor) {
    static_vector<TypeParam, this->C> v(5);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(StaticVectorTests, CountValueConstructor) {
    TypeParam val;
    static_vector<TypeParam, this->C> v(5, val);

    EXPECT_EQ(v.size(), 5);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, RangeConstructor) {
    const TypeParam a[] = {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    const int s = sizeof(a) / sizeof(a[0]);
    static_vector<TypeParam, this->C> v(a, a + s);

    EXPECT_EQ(v.size(), s);
    for (size_t i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], a[i]);
}

TYPED_TEST(StaticVectorTests, CopyConstructor) {
    auto v1 = this->GetVectorOfSize(5);
    static_vector<TypeParam, this->C> v2(v1);
    
    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(StaticVectorTests, MoveConstructor) {
    const TypeParam val = this->GetValue(10);
    const int size = 5;
    static_vector<TypeParam, this->C> v1(size, val);
    static_vector<TypeParam, this->C> v2(std::move(v1));
    
    EXPECT_EQ(v2.size(), size);
    for (const auto& x : v2)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, InitializerListConstructor) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2),
        this->GetValue(3), this->GetValue(4), this->GetValue(5)
    };
    static_vector<TypeParam, this->C> v(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(StaticVectorTests, CopyOperator) {
    auto v1 = this->GetVectorOfSize(5);
    static_vector<TypeParam, this->C> v2(3);
    v2 = v1;

    EXPECT_EQ(v1.size(), v2.size());
    for (typename TestFixture::size_type i = 0; i < v1.size(); ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TYPED_TEST(StaticVectorTests, MoveOperator) {
    const TypeParam val = this->GetValue(10);
    const int size = 5;
    static_vector<TypeParam, this->C> v1(size, val);
    static_vector<TypeParam, this->C> v2(3);
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), size);
    for (typename TestFixture::size_type i = 0; i < size; ++i)
        EXPECT_EQ(v2[i], val);
}

TYPED_TEST(StaticVectorTests, InitializerListOperator) {
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(1), this->GetValue(2)
    };
    static_vector<TypeParam, this->C> v(3);
    v = il;

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(StaticVectorTests, BeginEndInEmptyVector) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(StaticVectorTests, BeginEndInEmptyConstVector) {
    const static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

TYPED_TEST(StaticVectorTests, BeginEndMarkSize) {
    static_vector<TypeParam, this->C> v(5);
    
    EXPECT_EQ(std::distance(v.begin(), v.end()), v.size());
    EXPECT_EQ(std::distance(v.cbegin(), v.cend()), v.size());
    EXPECT_EQ(std::distance(v.rbegin(), v.rend()), v.size());
    EXPECT_EQ(std::distance(v.crbegin(), v.crend()), v.size());
}

TYPED_TEST(StaticVectorTests, IteratorsPointCorrectly) {
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

TYPED_TEST(StaticVectorTests, IsSizeChanging) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.size(), 0);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 1);
    v.push_back(this->GetValue(0));
    EXPECT_EQ(v.size(), 2);
    v.clear();
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(StaticVectorTests, IsMaxSizeConstantAndCorrect) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.max_size(), this->C);
    v.resize(5);
    EXPECT_EQ(v.max_size(), this->C);
}

TYPED_TEST(StaticVectorTests, IsCapacityConstantAndCorrect) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.capacity(), this->C);
    v.resize(5);
    EXPECT_EQ(v.capacity(), this->C);
}

TYPED_TEST(StaticVectorTests, IsVectorInitiallyEmpty) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_TRUE(v.empty());
}

TYPED_TEST(StaticVectorTests, IsNonEmptyVectorNonEmpty) {
    static_vector<TypeParam, this->C> v(5);
    
    EXPECT_FALSE(v.empty());
}

TYPED_TEST(StaticVectorTests, DoesResizeChangeSize) {
    static_vector<TypeParam, this->C> v;
    
    EXPECT_EQ(v.size(), 0);
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    v.resize(3);
    EXPECT_EQ(v.size(), 3);
}

TYPED_TEST(StaticVectorTests, ResizeEmptyWithDefaultConstruct) {
    static_vector<TypeParam, this->C> v;

    v.resize(5);

    for (const auto& x : v)
        EXPECT_EQ(x, TypeParam());
}

TYPED_TEST(StaticVectorTests, ResizeEmptyWithFill) {
    const TypeParam val = this->GetValue(rand());
    static_vector<TypeParam, this->C> v;

    v.resize(5, val);

    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, ResizeNonEmptyWithDefaultConstruct) {
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

TYPED_TEST(StaticVectorTests, ResizeNonEmptyWithFill) {
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

TYPED_TEST(StaticVectorTests, Reserve) {
    static_vector<TypeParam, this->C> v;
    const typename TestFixture::size_type initial_size = v.size();
    
    for (typename TestFixture::size_type i = 1; i <= this->C; ++i) {
        v.reserve(i);
        EXPECT_EQ(v.size(), initial_size);
        EXPECT_GE(v.capacity(), i);
    }
}

TYPED_TEST(StaticVectorTests, ShrinkToFit) {
    static_vector<TypeParam, this->C> v(5);
    v.shrink_to_fit();

    EXPECT_GE(v.capacity(), v.size());
}

TYPED_TEST(StaticVectorTests, AccessOperators) {
    auto v = this->GetVectorOfSize(5);

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], this->GetValue(i));
        EXPECT_EQ(v.at(i), this->GetValue(i));
    }

    EXPECT_THROW(v.at(-1), std::out_of_range);
    EXPECT_THROW(v.at(v.size()), std::out_of_range);
}

TYPED_TEST(StaticVectorTests, FrontBack) {
    auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(StaticVectorTests, ConstFrontBack) {
    const auto v = this->GetVectorOfSize(5);

    EXPECT_EQ(v.front(), this->GetValue(0));
    EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
}

TYPED_TEST(StaticVectorTests, Data) {
    auto v = this->GetVectorOfSize(5);
    TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(StaticVectorTests, ConstData) {
    const auto v = this->GetVectorOfSize(5);
    const TypeParam* data = v.data();

    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(data[i], v[i]);
}

TYPED_TEST(StaticVectorTests, RangeAssign) {
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

TYPED_TEST(StaticVectorTests, FillAssign) {
    auto v = this->GetVectorOfSize(3);
    const TypeParam val = this->GetValue(rand());
    typename TestFixture::size_type size = 5;
    
    v.assign(size, val);

    EXPECT_EQ(v.size(), size);
    for (const auto& x : v)
        EXPECT_EQ(x, val);
}

TYPED_TEST(StaticVectorTests, InitializerListAssign) {
    static_vector<TypeParam, this->C> v(2);
    std::initializer_list<TypeParam> il {
        this->GetValue(0), this->GetValue(13), this->GetValue(3)
    };
    
    v.assign(il);

    EXPECT_EQ(v.size(), il.size());
    for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
        EXPECT_EQ(v[i], il.begin()[i]);
}

TYPED_TEST(StaticVectorTests, PushBackByCopy) {
    static_vector<TypeParam, this->C> v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        v.push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }

    const TypeParam val = this->GetValue(0);
    EXPECT_THROW(v.push_back(val),
        std::out_of_range);
}

TYPED_TEST(StaticVectorTests, PushBackByMove) {
    static_vector<TypeParam, this->C> v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }

    EXPECT_THROW(v.push_back(this->GetValue(0)),
        std::out_of_range);
}

TYPED_TEST(StaticVectorTests, FastPushBackByCopy) {
    static_vector<TypeParam, this->C> v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (int i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        v.fast_push_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(StaticVectorTests, FastPushBackByMove) {
    static_vector<TypeParam, this->C> v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.push_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }
}

TYPED_TEST(StaticVectorTests, EmplaceBack) {
    static_vector<TypeParam, this->C> v(3);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        v.emplace_back(this->GetValue(id));

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), this->GetValue(id));
    }

    EXPECT_THROW(v.push_back(this->GetValue(0)),
        std::out_of_range);
}

TYPED_TEST(StaticVectorTests, FastEmplaceBack) {
    static_vector<TypeParam, this->C> v(2);
    const typename TestFixture::size_type initial_size = v.size();

    for (typename TestFixture::size_type i = 0; i + initial_size < this->C; ++i) {
        int id = rand();
        const TypeParam val = this->GetValue(id);
        v.fast_emplace_back(val);

        EXPECT_EQ(v.size(), initial_size + i + 1);
        EXPECT_EQ(v.back(), val);
        EXPECT_EQ(val, this->GetValue(id));
    }
}

TYPED_TEST(StaticVectorTests, PopBack) {
    const int initial_size = 3;
    auto v = this->GetVectorOfSize(initial_size);

    for (int i = initial_size - 1; i >= 0; --i) {
        EXPECT_EQ(v.back(), this->GetValue(i));
        v.pop_back();
        EXPECT_EQ(v.size(), i);
    }

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(StaticVectorTests, SafePopBack) {
    const int initial_size = 3;
    auto v = this->GetVectorOfSize(initial_size);

    for (int i = initial_size - 1; i >= 0; --i) {
        EXPECT_EQ(v.back(), this->GetValue(i));
        v.safe_pop_back();
        EXPECT_EQ(v.size(), i);
    }

    EXPECT_TRUE(v.empty());
    EXPECT_NO_THROW(v.safe_pop_back());
    EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(StaticVectorTests, PushPopBackMixed) {
    static_vector<TypeParam, this->C> v;
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

TYPED_TEST(StaticVectorTests, TwoSwap) {
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

TYPED_TEST(StaticVectorTests, OneSwap) {
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

TYPED_TEST(StaticVectorTests, Clear) {
    auto v = this->GetVectorOfSize(5);
    
    v.clear();

    EXPECT_TRUE(v.empty());
}

TYPED_TEST(StaticVectorTests, InsertOneElementByCopyToEmptyVector) {
    static_vector<TypeParam, this->C> v;

    this->InsertOneElementByCopy(v, 0, rand());
}

TYPED_TEST(StaticVectorTests, InsertOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByCopy(v, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByCopy(v, initial_size, 13);
}

TYPED_TEST(StaticVectorTests, InsertOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertOneElementByCopy(v, pos, 13);
}

TYPED_TEST(StaticVectorTests, InsertOneElementByMoveAtBeginToEmptyVector) {
    static_vector<TypeParam, this->C> v;
    
    this->InsertOneElementByMove(v, 0, rand());
}

TYPED_TEST(StaticVectorTests, InsertOneElementByMoveAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertOneElementByMove(v, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertOneElementByMoveAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertOneElementByMove(v, initial_size, 13);
}

TYPED_TEST(StaticVectorTests, InsertOneElementByMoveInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    
    this->InsertOneElementByMove(v, pos, 13);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByFillToEmptyVector) {
    static_vector<TypeParam, this->C> v;

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByFillAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, 0, 13);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByFillToEmptyVector) {
    static_vector<TypeParam, this->C> v;
    int count = Random::rand<int>(1, this->C);

    this->InsertMultipleElementsByFill(v, 0, count, rand());
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByFillAtBeginToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, 0, count, 13);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByFillAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);

    this->InsertMultipleElementsByFill(v, initial_size, count, 13);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByFillInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int count = Random::rand<int>(1, this->C - initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->InsertMultipleElementsByFill(v, pos, count, 13);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByRangeToEmptyVector) {
    static_vector<TypeParam, this->C> v;
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, initial_size, a, a);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    
    this->InsertMultipleElementsByRange(v, pos, a, a);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByRangeAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, 0, a, a + count);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByRangeAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);

    this->InsertMultipleElementsByRange(v, initial_size, a, a + count);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByRangeInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    TypeParam a[] = {
        this->GetValue(0), this->GetValue(13), this->GetValue(10)
    };
    const int count = sizeof(a) / sizeof(a[0]);
    
    this->InsertMultipleElementsByRange(v, pos, a, a + count);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByInitializerListToEmptyVector) {
    static_vector<TypeParam, this->C> v;
    std::initializer_list<TypeParam> ilist;
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(StaticVectorTests, InsertZeroElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByInitializerListAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(6);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByInitializerListAtEndToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };

    this->InsertMultipleElementsByInitializerList(v, initial_size, ilist);
}

TYPED_TEST(StaticVectorTests, InsertMultipleElementsByInitializerListInMiddleToNonEmptyVector) {
    const int initial_size = 6;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);
    std::initializer_list<TypeParam> ilist {
        this->GetValue(49), this->GetValue(13)
    };
    
    this->InsertMultipleElementsByInitializerList(v, pos, ilist);
}

TYPED_TEST(StaticVectorTests, EmplaceOneElementByCopyToEmptyVector) {
    static_vector<TypeParam, this->C> v;

    this->EmplaceAt(v, 0, rand());
}

TYPED_TEST(StaticVectorTests, EmplaceOneElementByCopyAtBeginToNonEmptyVector) {
    auto v = this->GetVectorOfSize(5);

    this->EmplaceAt(v, 0, 13);
}

TYPED_TEST(StaticVectorTests, EmplaceOneElementByCopyAtEndToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);

    this->EmplaceAt(v, initial_size, 13);
}

TYPED_TEST(StaticVectorTests, EmplaceOneElementByCopyInMiddleToNonEmptyVector) {
    const int initial_size = 5;
    auto v = this->GetVectorOfSize(initial_size);
    const int pos = Random::rand(1, initial_size - 1);

    this->EmplaceAt(v, pos, 13);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
