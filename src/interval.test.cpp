#include <interval.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;



using DoubleInterval = TypedInterval<double, struct DoubleIntervalTag>;
TEST(common, typed_interval_can_be_created) {
    double const min = -1.0, max = 1.0;
    DoubleInterval interval{min, max};
    EXPECT_THAT(interval.min, Eq(min));
    EXPECT_THAT(interval.max, Eq(max));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
