#include <common.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::DoubleEq;
using testing::Ne;
using testing::Ge;
using testing::Lt;

TEST(common, degree_to_radians) {
    EXPECT_THAT(degree_to_radians(180), DoubleEq(pi));
}

TEST(common, random_double_is_random) {
    EXPECT_THAT(random_double(), Ne(random_double()));
}

TEST(common, random_double_is_in_zero_one_range) {
    auto x = random_double();
    EXPECT_THAT(x, Ge(0.0));
    EXPECT_THAT(x, Lt(1.0));
}

TEST(common, random_double_is_in_range) {
    double min = 2.0, max = 3.0;
    auto x = random_double(min, max);
    EXPECT_THAT(x, Ge(min));
    EXPECT_THAT(x, Lt(max));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
