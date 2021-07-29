#include <gtest/gtest.h>
#include <common/memory.hpp>

using namespace uwr::mem;

// should be trivial type
using T = int;

class OptFillTests : public ::testing::Test {
private:
    void SetUp() {}
    void TearDown() {}

public:
    void FillAndExpectAllTheSameAs(T* begin, int n, T val) {
        opt_fill(begin, n, val);
        ExpectAllTheSameAs(begin, n, val);
    }

    void UFillAndExpectAllTheSameAs(T* begin, int n, T val) {
        opt_ufill(begin, n, val);
        ExpectAllTheSameAs(begin, n, val);
    }

    void ExpectAllTheSameAs(T* begin, int n, T val) {
        EXPECT_TRUE(std::all_of(begin, begin + n, [&](T x){
            return x == val;
        }));
    }
};

TEST_F(OptFillTests, OptFillAndSmallValues) {
    const int N = 10;
    T t[N];

    FillAndExpectAllTheSameAs(t, 3, 1);
    FillAndExpectAllTheSameAs(t, 4, 2);
    FillAndExpectAllTheSameAs(t, 5, 3);
    FillAndExpectAllTheSameAs(t, 6, 4);
    FillAndExpectAllTheSameAs(t, 9, 10);
    FillAndExpectAllTheSameAs(t, 10, 11);
}

TEST_F(OptFillTests, OptFillBigValues) {
    const int N = 1e6;
    T t[N];

    FillAndExpectAllTheSameAs(t, 1e3, 1);
    FillAndExpectAllTheSameAs(t, 5e3, 2);
    FillAndExpectAllTheSameAs(t, 5e4, 3);
    FillAndExpectAllTheSameAs(t, 1e5, 4);
    FillAndExpectAllTheSameAs(t, 5e5, 10);
    FillAndExpectAllTheSameAs(t, 1e6, 11);
}

TEST_F(OptFillTests, OptUninitializedFillSmallValues) {
    const int N = 10;
    char t[N * sizeof(T)];

    UFillAndExpectAllTheSameAs((T*)t, 3, 1);
    UFillAndExpectAllTheSameAs((T*)t, 4, 2);
    UFillAndExpectAllTheSameAs((T*)t, 5, 3);
    UFillAndExpectAllTheSameAs((T*)t, 6, 4);
    UFillAndExpectAllTheSameAs((T*)t, 9, 10);
    UFillAndExpectAllTheSameAs((T*)t, 10, 11);
}

TEST_F(OptFillTests, OptUninitializedFillBigValues) {
    const int N = 1e6;
    char t[N * sizeof(T)];

    UFillAndExpectAllTheSameAs((T*)t, 1e3, 1);
    UFillAndExpectAllTheSameAs((T*)t, 5e3, 2);
    UFillAndExpectAllTheSameAs((T*)t, 5e4, 3);
    UFillAndExpectAllTheSameAs((T*)t, 1e5, 4);
    UFillAndExpectAllTheSameAs((T*)t, 5e5, 10);
    UFillAndExpectAllTheSameAs((T*)t, 1e6, 11);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
