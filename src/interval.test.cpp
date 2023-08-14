#include <interval.hpp>

#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::Test;


using DoubleInterval = TypedInterval<double, struct DoubleIntervalTag>;
using Vec3Interval   = TypedInterval<vec3,   struct Vec3IntervalTag>;

struct ADoubleInterval : Test {
    double const min = -1.0, max = 1.0;
    DoubleInterval const interval{min, max};
};

struct AVec3Interval : Test {
    vec3 const min{1.0, 1.0, 1.0};
    vec3 const max{1.0, 4.0, 5.0};
    Vec3Interval const interval{min, max};
};

TEST_F(ADoubleInterval, can_be_created) {
    EXPECT_THAT(interval.min, Eq(min));
    EXPECT_THAT(interval.max, Eq(max));
}

TEST_F(ADoubleInterval, can_be_expanded) {
    auto const delta = 0.2;
    auto const expanded = interval.expand(delta);
    EXPECT_THAT(expanded.min, Eq(min - 0.5 * delta));
    EXPECT_THAT(expanded.max, Eq(max + 0.5 * delta));
}

TEST_F(AVec3Interval, can_be_expanded) {
    auto const delta = 0.2;
    auto const expanded = interval.expand(vec3{delta});
    EXPECT_THAT(expanded.min, Eq(min - 0.5 * vec3{delta}));
    EXPECT_THAT(expanded.max, Eq(max + 0.5 * vec3{delta}));
}

TEST_F(ADoubleInterval, has_length) {
    EXPECT_THAT(interval.length(), Eq(max - min));
}

TEST_F(AVec3Interval, has_length) {
    EXPECT_THAT(interval.length(), Eq((max - min).length()));
}

TEST_F(ADoubleInterval, can_be_compared_for_equality) {
    EXPECT_THAT(interval, Eq(DoubleInterval{min, max}));
}

TEST_F(AVec3Interval, can_be_compared_for_equality) {
    EXPECT_THAT(interval, Eq(Vec3Interval{min, max}));
}

TEST_F(ADoubleInterval, can_be_translated) {
    double const translation{1.0};
    EXPECT_THAT(interval + translation,
                Eq(decltype(interval){min + translation, max + translation}));
}

TEST_F(AVec3Interval, can_be_translated) {
    vec3 const translation{1.0};
    EXPECT_THAT(interval + translation,
                Eq(decltype(interval){min + translation, max + translation}));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
