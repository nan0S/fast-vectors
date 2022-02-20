#pragma once

#include "vector_test_base_fixture.hpp"

#include "utils/random.hpp"

TYPED_TEST(VectorTestBaseFixture, DefaultConstructor) {
   typename TestFixture::vector v;

   EXPECT_EQ(v.size(), 0);
}

TYPED_TEST(VectorTestBaseFixture, CountConstructor) {
   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v(size);

      EXPECT_EQ(v.size(), size);
      for (const auto& x : v)
         EXPECT_EQ(x, typename TestFixture::value_type());
   }
}

TYPED_TEST(VectorTestBaseFixture, CountValueConstructor) {
   for (const auto& size : this->GetAllSizes()) {
      auto val = this->GetValue(3);

      typename TestFixture::vector v(size, val);

      EXPECT_EQ(v.size(), size);
      for (const auto& x : v)
         EXPECT_EQ(x, val);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeConstructorWithPointer) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto a = this->GetInitializedArrayOfSize(size);

      typename TestFixture::vector v(a.get(), a.get() + size);

      for (size_type i = 0; i < size; ++i)
         EXPECT_EQ(a[i], this->GetValue(i));
      EXPECT_EQ(v.size(), size);
      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], a[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeConstructorWithContinuousIterator) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto stl_vec = this->GetInitializedVectorOfSize(size);

      typename TestFixture::vector v(stl_vec.begin(), stl_vec.end());

      for (size_t i = 0; i < v.size(); ++i)
         EXPECT_EQ(stl_vec[i], this->GetValue(i));
      EXPECT_EQ(v.size(), size);
      for (size_t i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], stl_vec[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeConstructorWithNonContinuousIterator) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto stl_list = this->GetInitializedListOfSize(size);

      typename TestFixture::vector v(stl_list.begin(), stl_list.end());

      auto it = stl_list.begin();
      size_t i = 0;
      for (; it != stl_list.end(); ++it, ++i)
         EXPECT_EQ(*it, this->GetValue(i));
      EXPECT_EQ(v.size(), size);
      for (i = 0, it = stl_list.begin(); i < size; ++i, ++it)
         EXPECT_EQ(v[i], *it);
   }
}

TYPED_TEST(VectorTestBaseFixture, CopyConstructor) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);

      typename TestFixture::vector v2(v1);

      EXPECT_EQ(v1.size(), v2.size());
      for (size_type i = 0; i < v1.size(); ++i)
         EXPECT_EQ(v1[i], v2[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, MoveConstructor) {
   for (const auto& size : this->GetAllSizes()) {
      const auto val = this->GetValue(10);
      typename TestFixture::vector v1(size, val);

      typename TestFixture::vector v2(std::move(v1));

      EXPECT_EQ(v2.size(), size);
      for (const auto& x : v2)
         EXPECT_EQ(x, val);
   }
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
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      typename TestFixture::vector v2(3);

      v2 = v1;

      EXPECT_EQ(v1.size(), v2.size());
      for (size_type i = 0; i < v1.size(); ++i)
         EXPECT_EQ(v1[i], v2[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, MoveOperator) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto val = this->GetValue(10);
      typename TestFixture::vector v1(size, val);
      typename TestFixture::vector v2(size / 2);

      v2 = std::move(v1);

      EXPECT_EQ(v2.size(), size);
      for (size_type i = 0; i < size; ++i)
         EXPECT_EQ(v2[i], val);
   }
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

TYPED_TEST(VectorTestBaseFixture, CopyOperatorToTheSameVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto val = this->GetValue(10);
      typename TestFixture::vector v(size, val);
      auto& v1 = v;
      auto& v2 = v;

      v1 = v2;

      EXPECT_EQ(v.size(), size);
      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], val);
   }
}

TYPED_TEST(VectorTestBaseFixture, MoveOperatorToTheSameVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto val = this->GetValue(10);
      typename TestFixture::vector v(size, val);
      auto& v1 = v;
      auto& v2 = v;

      v1 = std::move(v2);

      EXPECT_EQ(v.size(), size);
      for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], val);
   }
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
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      int i = 0;
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
   for (const auto& size : this->GetAllSizes()) {
      v.resize(size);
      EXPECT_EQ(v.size(), size);
   }
}

TYPED_TEST(VectorTestBaseFixture, ResizeEmptyWithDefaultConstruct) {
   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v;

      v.resize(size);

      for (const auto& x : v)
         EXPECT_EQ(x, typename TestFixture::value_type());
   }
}

TYPED_TEST(VectorTestBaseFixture, ResizeEmptyWithFill) {
   for (const auto& size : this->GetAllSizes()) {
      const auto val = this->GetValue(rand());
      typename TestFixture::vector v;

      v.resize(size, val);

      for (const auto& x : v)
         EXPECT_EQ(x, val);
   }
}

TYPED_TEST(VectorTestBaseFixture, ResizeNonEmptyWithDefaultConstruct) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      v.resize(size + 2);

      for (size_type i = 0; i < size; ++i)
         EXPECT_EQ(v[i], this->GetValue(i));
      for (size_type i = size; i < size + 2; ++i)
         EXPECT_EQ(v[i], typename TestFixture::value_type());

      v.resize(3);

      for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], this->GetValue(i));
   }
}

TYPED_TEST(VectorTestBaseFixture, ResizeNonEmptyWithFill) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto val = this->GetValue(13);
      auto v = this->GetVectorOfSize(size);

      v.resize(size + 2, val);

      for (size_type i = 0; i < size; ++i)
         EXPECT_EQ(v[i], this->GetValue(i));
      for (size_type i = size; i < size + 2; ++i)
         EXPECT_EQ(v[i], val);

      v.resize(3);

      for (typename TestFixture::size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], this->GetValue(i));
   }
}

TYPED_TEST(VectorTestBaseFixture, ReserveWithEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v;
      const size_type initial_size = v.size();

      for (size_type i = 1; i <= size; ++i) {
         v.reserve(i);
         EXPECT_EQ(v.size(), initial_size);
         EXPECT_GE(v.capacity(), i);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, ReserveWithNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v(size / 2);
      const size_type initial_size = v.size();

      for (size_type i = 1; i <= size; ++i) {
         v.reserve(i);
         EXPECT_EQ(v.size(), initial_size);
         EXPECT_GE(v.capacity(), i);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, ShrinkToFit) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto val = this->GetValue(13);
      const size_type ins = 2 * size;
      for (size_type i = 0; i < ins; ++i)
         v.push_back(val);

      v.shrink_to_fit();

      EXPECT_GE(v.capacity(), v.size());
      for (size_type i = 0; i < size; ++i)
         EXPECT_EQ(v[i], this->GetValue(i));
      for (size_type i = 0; i < ins; ++i)
         EXPECT_EQ(v[size + i], val);
   }
}

TYPED_TEST(VectorTestBaseFixture, AccessOperators) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      for (size_type i = 0; i < v.size(); ++i) {
         EXPECT_EQ(v[i], this->GetValue(i));
         EXPECT_EQ(v.at(i), this->GetValue(i));
      }

      // EXPECT_THROW(v.at(v.size()), std::out_of_range);
   }
}

TYPED_TEST(VectorTestBaseFixture, FrontBack) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      EXPECT_EQ(v.front(), this->GetValue(0));
      EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
   }
}

TYPED_TEST(VectorTestBaseFixture, ConstFrontBack) {
   for (const auto& size : this->GetAllSizes()) {
      const auto v = this->GetVectorOfSize(size);

      EXPECT_EQ(v.front(), this->GetValue(0));
      EXPECT_EQ(v.back(), this->GetValue(v.size() - 1));
   }
}

TYPED_TEST(VectorTestBaseFixture, Data) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      auto* data = v.data();

      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(data[i], v[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, ConstData) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto v = this->GetVectorOfSize(size);

      const auto* data = v.data();

      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(data[i], v[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeAssignWithPointer) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const auto a = this->GetInitializedArrayOfSize(size);
      auto v = this->GetVectorOfSize(2);

      v.assign(a.get(), a.get() + size);

      EXPECT_EQ(v.size(), size);
      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], a[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeAssignWithContinuousIterator) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto stl_vec = this->GetInitializedVectorOfSize(size);
      auto v =  this->GetVectorOfSize(3);

      v.assign(stl_vec.begin(), stl_vec.end());

      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(stl_vec[i], this->GetValue(i));
      EXPECT_EQ(v.size(), size);
      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], stl_vec[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, RangeAssignWithNonContinuousIterator) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto stl_list = this->GetInitializedListOfSize(size);
      auto v = this->GetVectorOfSize(5);

      v.assign(stl_list.begin(), stl_list.end());

      auto it = stl_list.begin();
      size_type i = 0;
      for (; it != stl_list.end(); ++it, ++i)
         EXPECT_EQ(*it, this->GetValue(i));
      EXPECT_EQ(v.size(), size);
      for (i = 0, it = stl_list.begin(); i < size; ++i, ++it)
         EXPECT_EQ(v[i], *it);
   }
}

TYPED_TEST(VectorTestBaseFixture, FillAssign) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(3);
      const auto val = this->GetValue(rand());

      v.assign(size, val);

      EXPECT_EQ(v.size(), size);
      for (const auto& x : v)
         EXPECT_EQ(x, val);
   }
}

TYPED_TEST(VectorTestBaseFixture, InitializerListAssign) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(size);
      auto il = {
         this->GetValue(0),
         this->GetValue(13),
         this->GetValue(3)
      };

      v.assign(il);

      EXPECT_EQ(v.size(), il.size());
      for (size_type i = 0; i < v.size(); ++i)
         EXPECT_EQ(v[i], il.begin()[i]);
   }
}

TYPED_TEST(VectorTestBaseFixture, PushBackByCopy) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();
         const auto val = this->GetValue(id);

         v.push_back(val);

         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), val);
         EXPECT_EQ(val, this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, PushBackByMove) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();
         v.push_back(this->GetValue(id));

         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, FastPushBackByCopy) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (int i = 0; i + initial_size < size; ++i) {
         int id = rand();
         const auto val = this->GetValue(id);

         // v.fast_push_back(val);
         v.push_back(val);

         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), val);
         EXPECT_EQ(val, this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, FastPushBackByMove) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();

         // v.fast_push_back(this->GetValue(id));
         v.push_back(this->GetValue(id));

         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceBackByCopy) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();
         const auto val = this->GetValue(id);

         auto& ret = v.emplace_back(val);

         EXPECT_EQ(val, this->GetValue(id));
         EXPECT_EQ(ret, this->GetValue(id));
         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceBackByMove) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v(3);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();

         auto& ret = v.emplace_back(this->GetValue(id));

         EXPECT_EQ(ret, this->GetValue(id));
         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, FastEmplaceBack) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      typename TestFixture::vector v(2);
      const size_type initial_size = v.size();

      for (size_type i = 0; i + initial_size < size; ++i) {
         int id = rand();
         const auto val = this->GetValue(id);

         // v.fast_emplace_back(val);
         v.emplace_back(val);

         EXPECT_EQ(v.size(), initial_size + i + 1);
         EXPECT_EQ(v.back(), val);
         EXPECT_EQ(val, this->GetValue(id));
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, PopBack) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      for (size_type i = size; i >= 1; --i) {
         EXPECT_EQ(v.back(), this->GetValue(i - 1));
         v.pop_back();
         EXPECT_EQ(v.size(), i - 1);
      }

      EXPECT_TRUE(v.empty());
   }
}

TYPED_TEST(VectorTestBaseFixture, SafePopBack) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      for (size_type i = size; i >= 1; --i) {
         EXPECT_EQ(v.back(), this->GetValue(i - 1));
         // v.safe_pop_back();
         v.pop_back();
         EXPECT_EQ(v.size(), i - 1);
      }

      EXPECT_TRUE(v.empty());
      // EXPECT_NO_THROW(v.safe_pop_back());
   }
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
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const size_type v1_size = size;
      const size_type v2_size = size * 2 / 3;
      auto v1 = this->GetVectorOfSize(v1_size);
      auto v2 = this->GetVectorOfSize(v2_size);

      std::reverse(v1.begin(), v1.end());
      v1.swap(v2);
      v2.swap(v1);

      EXPECT_EQ(v1.size(), v1_size);
      EXPECT_EQ(v2.size(), v2_size);
      for (size_type i = 0; i < v1.size(); ++i)
         EXPECT_EQ(v1[i], this->GetValue(v1.size() - 1 - i));
      for (size_type i = 0; i < v2.size(); ++i)
         EXPECT_EQ(v2[i], this->GetValue(i));
   }
}

TYPED_TEST(VectorTestBaseFixture, OneSwap) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const size_type v1_size = size;
      const size_type v2_size = 5 * size / 3;
      auto v1 = this->GetVectorOfSize(v1_size);
      auto v2 = this->GetVectorOfSize(v2_size);

      std::reverse(v1.begin(), v1.end());
      v1.swap(v2);

      EXPECT_EQ(v1.size(), v2_size);
      EXPECT_EQ(v2.size(), v1_size);
      for (size_type i = 0; i < v1.size(); ++i)
         EXPECT_EQ(v1[i], this->GetValue(i));
      for (size_type i = 0; i < v2.size(); ++i)
         EXPECT_EQ(v2[i], this->GetValue(v2.size() - 1 - i));
   }
}

TYPED_TEST(VectorTestBaseFixture, StdTwoSwap) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const size_type v1_size = size;
      const size_type v2_size = size / 4;
      auto v1 = this->GetVectorOfSize(v1_size);
      auto v2 = this->GetVectorOfSize(v2_size);

      std::reverse(v1.begin(), v1.end());
      std::swap(v1, v2);
      std::swap(v2, v1);

      EXPECT_EQ(v1.size(), v1_size);
      EXPECT_EQ(v2.size(), v2_size);
      for (size_type i = 0; i < v1.size(); ++i)
         EXPECT_EQ(v1[i], this->GetValue(v1.size() - 1 - i));
      for (size_type i = 0; i < v2.size(); ++i)
         EXPECT_EQ(v2[i], this->GetValue(i));
   }
}

TYPED_TEST(VectorTestBaseFixture, StdOneSwap) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      const size_type v1_size = size;
      const size_type v2_size = size * 3 / 4;
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
}

TYPED_TEST(VectorTestBaseFixture, Clear) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      v.clear();

      EXPECT_TRUE(v.empty());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyToEmptyVector) {
   typename TestFixture::vector v;

   this->InsertOneElementByCopy(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertOneElementByCopy(v, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertOneElementByCopy(v, size, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByCopyInMiddleToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const int pos = 3;

      this->InsertOneElementByCopy(v, pos, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtBeginToEmptyVector) {
   typename TestFixture::vector v;

   this->InsertOneElementByMove(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertOneElementByMove(v, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertOneElementByMove(v, size, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertOneElementByMoveInMiddleToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const int pos = 3;

      this->InsertOneElementByMove(v, pos, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillToEmptyVector) {
   typename TestFixture::vector v;

   this->InsertMultipleElementsByFill(v, 0, 0, 13);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertMultipleElementsByFill(v, 0, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->InsertMultipleElementsByFill(v, size, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByFillInMiddleToNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const size_type pos = Random::rand<size_type>(1, size - 1);

      this->InsertMultipleElementsByFill(v, pos, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillToEmptyVector) {
   for (const auto& count : this->GetAllSizes()) {
      typename TestFixture::vector v;

      this->InsertMultipleElementsByFill(v, 0, count, rand());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         auto v = this->GetVectorOfSize(size);

         this->InsertMultipleElementsByFill(v, 0, count, 13);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         auto v = this->GetVectorOfSize(size);

         this->InsertMultipleElementsByFill(v, size, count, 13);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByFillInMiddleToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         auto v = this->GetVectorOfSize(size);
         const int pos = 2;

         this->InsertMultipleElementsByFill(v, pos, count, 13);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithPointerToEmptyVector) {
   typename TestFixture::vector v;
   typename TestFixture::value_type a[0];

   this->InsertMultipleElementsByRange(v, 0, a, a);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithCotinuousIteratorAtBeginToEmptyVector) {
   typename TestFixture::vector v;
   std::vector<typename TestFixture::value_type> stl_vec;

   this->InsertMultipleElementsByRange(v, 0,
         stl_vec.begin(), stl_vec.begin());
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithNonContinuousIteratorAtBeginToEmptyVector) {
   typename TestFixture::vector v;
   std::list<typename TestFixture::value_type> stl_list;

   this->InsertMultipleElementsByRange(v, 0,
         stl_list.begin(), stl_list.begin());
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithPointerAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      typename TestFixture::value_type a[0];

      this->InsertMultipleElementsByRange(v, 0, a, a);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithCotinuousIteratorAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::vector<typename TestFixture::value_type> stl_vec;

      this->InsertMultipleElementsByRange(v, 0,
            stl_vec.begin(), stl_vec.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithNonContinuousIteratorAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::list<typename TestFixture::value_type> stl_list;

      this->InsertMultipleElementsByRange(v, 0,
            stl_list.begin(), stl_list.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithPointerAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      typename TestFixture::value_type a[0];

      this->InsertMultipleElementsByRange(v, size, a, a);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithCotinuousIteratorAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::vector<typename TestFixture::value_type> stl_vec;

      this->InsertMultipleElementsByRange(v, size,
            stl_vec.begin(), stl_vec.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithNonContinuousIteratorAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::list<typename TestFixture::value_type> stl_list;

      this->InsertMultipleElementsByRange(v, size,
            stl_list.begin(), stl_list.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithPointerInMiddleToNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const size_type pos = Random::rand<size_type>(1, size);
      typename TestFixture::value_type a[0];

      this->InsertMultipleElementsByRange(v, pos, a, a);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithCotinuousIteratorInMiddleToNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const size_type pos = Random::rand<size_type>(1, size);
      std::vector<typename TestFixture::value_type> stl_vec;

      this->InsertMultipleElementsByRange(v, pos,
            stl_vec.begin(), stl_vec.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByRangeWithNonContinuousIteratorInMiddleToNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      // const size_type pos = Random::rand<size_type>(1, size);
      const size_type pos = 1;
      std::list<typename TestFixture::value_type> stl_list;

      this->InsertMultipleElementsByRange(v, pos,
            stl_list.begin(), stl_list.begin());
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithPointerAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         auto v = this->GetVectorOfSize(size);
         auto a = this->GetInitializedArrayOfSize(count);

         this->InsertMultipleElementsByRange(v, 0, a.get(), a.get() + count);
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithCotinuousIteratorAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         if (size + count <= this->GetMaxVectorSize()) {
            auto v = this->GetVectorOfSize(size);
            auto stl_vec = this->GetInitializedVectorOfSize(count);

            this->InsertMultipleElementsByRange(v, 0,
                  stl_vec.begin(), stl_vec.end());
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithNonContinuousIteratorAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         if (size + count <= this->GetMaxVectorSize()) {
            auto v = this->GetVectorOfSize(size);
            auto stl_list = this->GetInitializedListOfSize(count);

            this->InsertMultipleElementsByRange(v, 0,
                  stl_list.begin(), stl_list.end());
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithPointerAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         if (size + count <= this->GetMaxVectorSize()) {
            auto v = this->GetVectorOfSize(size);
            auto a = this->GetInitializedArrayOfSize(count);

            this->InsertMultipleElementsByRange(v, size, a.get(), a.get() + count);
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithContinuousIteratorAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         if (size + count <= this->GetMaxVectorSize()) {
            auto v = this->GetVectorOfSize(size);
            auto stl_vec = this->GetInitializedVectorOfSize(count);

            this->InsertMultipleElementsByRange(v, size,
                  stl_vec.begin(), stl_vec.end());
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByRangeWithNonContinuousIteratorAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (const auto& count : this->GetAllSizes()) {
         if (size + count <= this->GetMaxVectorSize()) {
            auto v = this->GetVectorOfSize(size);
            auto stl_list = this->GetInitializedListOfSize(count);

            this->InsertMultipleElementsByRange(v, size,
                  stl_list.begin(), stl_list.end());
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListToEmptyVector) {
   typename TestFixture::vector v;
   std::initializer_list<typename TestFixture::value_type> ilist;

   this->InsertMultipleElementsByInitializerList(v, 0, ilist);
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::initializer_list<typename TestFixture::value_type> ilist;

      this->InsertMultipleElementsByInitializerList(v, 0, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::initializer_list<typename TestFixture::value_type> ilist;

      this->InsertMultipleElementsByInitializerList(v, size, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertZeroElementsByInitializerListInMiddleToNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      std::initializer_list<typename TestFixture::value_type> ilist;
      const size_type pos = Random::rand<size_type>(1, size);

      this->InsertMultipleElementsByInitializerList(v, pos, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      auto ilist = { this->GetValue(49), this->GetValue(13) };

      this->InsertMultipleElementsByInitializerList(v, 0, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      auto ilist = { this->GetValue(49), this->GetValue(13) };

      this->InsertMultipleElementsByInitializerList(v, size, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, InsertMultipleElementsByInitializerListInMiddleToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      auto ilist = { this->GetValue(49), this->GetValue(13) };
      const int pos = 3;

      this->InsertMultipleElementsByInitializerList(v, pos, ilist);
   }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyToEmptyVector) {
   typename TestFixture::vector v;

   this->EmplaceAt(v, 0, rand());
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyAtBeginToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EmplaceAt(v, 0, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyAtEndToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EmplaceAt(v, size, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, EmplaceOneElementByCopyInMiddleToNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const int pos = 3;

      this->EmplaceAt(v, pos, 13);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseBeginFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EraseOneElement(v, 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseEndFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EraseOneElement(v, size - 1);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElementsFromEmptyVector) {
   typename TestFixture::vector v;

   this->EraseMultipleElements(v, 0, 0);
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElementsAtBeginFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EraseMultipleElements(v, 0, 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseZeroElemensAtEndFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EraseMultipleElements(v, size, 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseAllElementsFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      this->EraseMultipleElements(v, 0, size);
      EXPECT_TRUE(v.empty());
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseMultipleElementsAtBeginFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const size_type count = std::min<size_type>(size, 3);

      this->EraseMultipleElements(v, 0, count);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseMultipleElementsToEndFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const size_type pos = Random::rand<size_type>(1, size - 1);

      this->EraseMultipleElements(v, pos, size - pos);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseMutipleElementsInMiddleFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      for (auto count : this->GetAllSizes()) {
         if (count <= size) {
            auto v = this->GetVectorOfSize(size);
            const int pos = 1;
            if (count == size)
               --count;

            this->EraseMultipleElements(v, pos, count);
         }
      }
   }
}

#if CPP_ABOVE_17

TYPED_TEST(VectorTestBaseFixture, EraseByValueFromEmptyVector) {
   typename TestFixture::vector v;

   auto count = std::erase(v, this->GetValue(0));

   EXPECT_EQ(count, 0);
   EXPECT_TRUE(v.empty());
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueNothingFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      auto count = std::erase(v, this->GetValue(-1));

      EXPECT_EQ(count, 0);
      EXPECT_EQ(v.size(), size);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueAtBeginFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;

      auto count = std::erase(v, this->GetValue(0));

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, 0, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueAtEndFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;

      auto count = std::erase(v, this->GetValue(size - 1));

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, size - 1, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueOneValueInMiddleFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;
      const size_type id = Random::rand<size_type>(1, size - 1);

      auto count = std::erase(v, this->GetValue(id));

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, id, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueMultipleValuesInOneBlockAtBeginFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) --block;
            for (size_type i = 0; i < block; ++i)
               v[i] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase(v, this->GetValue(-1));

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, 0, block, save);
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueMultipleValuesInOneBlockAtEndFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) --block;
            for (size_type i = size - block; i < size; ++i)
               v[i] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase(v, this->GetValue(-1));

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, size - block, block, save);
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseByValueMultipleValuesInOneBlockInMiddleFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) block -= 2;
            const size_type pos = Random::rand<size_type>(1, size - block);
            for (size_type i = 0; i < block; ++i)
               v[i + pos] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase(v, this->GetValue(-1));

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, pos, block, save);
         }
      }
   }
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
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);

      auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x) {
         return x == this->GetValue(-1);
      });

      EXPECT_EQ(count, 0);
      EXPECT_EQ(v.size(), size);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueAtBeginFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;

      auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
         return x == this->GetValue(0);
      });

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, 0, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueAtEndFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;

      auto count = std::erase_if(v, [&](const typename TestFixture::value_type& x){
         return x == this->GetValue(size - 1);
      });

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, size - 1, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueOneValueInMiddleFromNonEmptyVector) {
   for (const auto& size : this->GetAllSizes()) {
      auto v = this->GetVectorOfSize(size);
      const auto save = v;

      auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
         return x == this->GetValue(1);
      });

      ASSERT_EQ(count, 1);
      this->ExpectErasedInAt(v, 1, 1, save);
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesInOneBlockAtBeginFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) --block;
            for (size_type i = 0; i < block; ++i)
               v[i] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
               return x == this->GetValue(-1);
            });

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, 0, block, save);
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesInOneBlockAtEndFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) --block;
            for (size_type i = size - block; i < size; ++i)
               v[i] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
               return x == this->GetValue(-1);
            });

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, size - block, block, save);
         }
      }
   }
}

TYPED_TEST(VectorTestBaseFixture, EraseIfByValueMultipleValuesInOneBlockInMiddleFromNonEmptyVector) {
   using size_type = typename TestFixture::size_type;

   for (const size_type& size : this->GetAllSizes()) {
      for (size_type block : this->GetAllSizes()) {
         if (block <= size) {
            auto v = this->GetVectorOfSize(size);
            if (block == size) block -= 2;
            const size_type pos = Random::rand<size_type>(1, size - block);
            for (size_type i = 0; i < block; ++i)
               v[i + pos] = this->GetValue(-1);
            const auto save = v;

            auto count = std::erase_if(v, [this](const typename TestFixture::value_type& x){
               return x == this->GetValue(-1);
            });

            ASSERT_EQ(count, block);
            this->ExpectErasedInAt(v, pos, block, save);
         }
      }
   }
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
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 == v2);
   }
}

TYPED_TEST(VectorTestBaseFixture, CopiedVectorIsEqualToOriginal) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = v1;

      EXPECT_TRUE(v1 == v2);
   }
}

TYPED_TEST(VectorTestBaseFixture, EmptyVectorsAreNotDifferent) {
   typename TestFixture::vector v1, v2;

   EXPECT_FALSE(v1 != v2);
}

TYPED_TEST(VectorTestBaseFixture, EqualIndependentNonEmptyVectorsAreNotDifferent) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_FALSE(v1 != v2);
   }
}

TYPED_TEST(VectorTestBaseFixture, CopiedVectorIsNotDifferentToOriginal) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = v1;

      EXPECT_FALSE(v1 != v2);
   }
}

TYPED_TEST(VectorTestBaseFixture, DifferentVectorsAreDifferentWhereOneIsEmpty) {
   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v1;
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 != v2);
      EXPECT_TRUE(v2 != v1);
   }
}

TYPED_TEST(VectorTestBaseFixture, DifferentVectorsOfTheSameSizeAreDifferent) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);
      std::reverse(v2.begin(), v2.end());

      EXPECT_TRUE(v1 != v2);
   }
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
   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v1; 
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 < v2);
      EXPECT_TRUE(v1 <= v2);
      EXPECT_FALSE(v1 > v2);
      EXPECT_FALSE(v1 >= v2);
      EXPECT_FALSE(v2 < v1);
      EXPECT_FALSE(v2 <= v1);
      EXPECT_TRUE(v2 > v1);
      EXPECT_TRUE(v2 >= v1);
   }
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyEqualVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 <= v2);
      EXPECT_TRUE(v1 >= v2);
      EXPECT_TRUE(v2 <= v1);
      EXPECT_TRUE(v2 >= v1);
      EXPECT_FALSE(v1 < v2);
      EXPECT_FALSE(v1 > v2);
      EXPECT_FALSE(v2 < v1);
      EXPECT_FALSE(v2 > v1);
   }
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyEqualSizedVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      v1[2] = this->GetValue(1);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 <= v2);
      EXPECT_FALSE(v1 >= v2);
      EXPECT_FALSE(v2 <= v1);
      EXPECT_TRUE(v2 >= v1);
      EXPECT_TRUE(v1 < v2);
      EXPECT_FALSE(v1 > v2);
      EXPECT_FALSE(v2 < v1);
      EXPECT_TRUE(v2 > v1);
   }
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyDifferentSizedVectorsOnePrefixOfAnother) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size - 1);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE(v1 <= v2);
      EXPECT_FALSE(v1 >= v2);
      EXPECT_FALSE(v2 <= v1);
      EXPECT_TRUE(v2 >= v1);
      EXPECT_TRUE(v1 < v2);
      EXPECT_FALSE(v1 > v2);
      EXPECT_FALSE(v2 < v1);
      EXPECT_TRUE(v2 > v1);
   }
}

TYPED_TEST(VectorTestBaseFixture, RelationalOperatorsWithTwoNonEmptyDifferentVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size - 1);
      v1[1] = this->GetValue(2);
      v1[2] = this->GetValue(1);
      v2[1] = this->GetValue(9);

      EXPECT_TRUE(v1 <= v2);
      EXPECT_FALSE(v1 >= v2);
      EXPECT_FALSE(v2 <= v1);
      EXPECT_TRUE(v2 >= v1);
      EXPECT_TRUE(v1 < v2);
      EXPECT_FALSE(v1 > v2);
      EXPECT_FALSE(v2 < v1);
      EXPECT_TRUE(v2 > v1);
   }
}

#if CPP_ABOVE_17

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfEmptyVectors) {
   typename TestFixture::vector v1;
   typename TestFixture::vector v2;

   EXPECT_TRUE((v1 <=> v2) == 0);
   EXPECT_TRUE((v2 <=> v1) == 0);
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfOneEmpty) {
   for (const auto& size : this->GetAllSizes()) {
      typename TestFixture::vector v1;
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE((v1 <=> v2) < 0);
      EXPECT_TRUE((v2 <=> v1) > 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyEqualVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);

      EXPECT_TRUE((v1 <=> v2) == 0);
      EXPECT_TRUE((v2 <=> v1) == 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyEqualSizedVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size);
      v2[1] = this->GetValue(size);

      EXPECT_TRUE((v1 <=> v2) < 0);
      EXPECT_TRUE((v2 <=> v1) > 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyDifferentSizedVectorsOnePrefixOfOther) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size + 1);

      EXPECT_TRUE((v1 <=> v2) < 0);
      EXPECT_TRUE((v2 <=> v1) > 0);
   }
}

TYPED_TEST(VectorTestBaseFixture, ThreeWayCompareOfNonEmptyDifferentVectors) {
   for (const auto& size : this->GetAllSizes()) {
      auto v1 = this->GetVectorOfSize(size);
      auto v2 = this->GetVectorOfSize(size - 1);
      v1[1] = this->GetValue(2);
      v1[2] = this->GetValue(1);
      v2[1] = this->GetValue(9);

      EXPECT_TRUE((v1 <=> v2) < 0);
      EXPECT_TRUE((v2 <=> v1) > 0);
   }
}

#endif // CPP_ABOVE_17
