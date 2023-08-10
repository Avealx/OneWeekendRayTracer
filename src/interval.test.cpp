#include <interval.hpp>

#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


using DoubleInterval = TypedInterval<double, struct DoubleIntervalTag>;

TEST(TypedInterval, can_be_created) {
    double const min = -1.0, max = 1.0;
    DoubleInterval interval{min, max};
    EXPECT_THAT(interval.min, Eq(min));
    EXPECT_THAT(interval.max, Eq(max));
}

TEST(TypedInterval, has_length_for_buildin_types) {
    DoubleInterval const interval{1.0, 3.0};
    EXPECT_THAT(interval.length(), Eq(2.0));
}

using Vec3Interval = TypedInterval<vec3, struct Vec3IntervalTag>;

TEST(TypedInterval, has_length_for_vec3) {
    Vec3Interval const interval{vec3{1.0, 1.0, 1.0}, vec3{1.0, 4.0, 5.0}};
    EXPECT_THAT(interval.length(), Eq(5.0));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
