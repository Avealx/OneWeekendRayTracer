// TODO: think about how to assure that assignment of strong types across
//       different classes does not compile.
#include <common.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::DoubleEq;
using testing::Eq;
using testing::Ge;
using testing::Lt;
using testing::Ne;


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

using MyBool = TypedBool<struct MyBoolTag>;

TEST(common, typed_bool_can_be_constructed) {
     MyBool{true};
}

TEST(common, typed_bool_has_truth_value) {
     EXPECT_TRUE(MyBool{true});
     EXPECT_FALSE(MyBool{false});
}

TEST(common, typed_bool_has_negation) {
    EXPECT_TRUE(!MyBool{false});
}

TEST(common, typed_bool_has_equality) {
    EXPECT_THAT(MyBool{false}, Eq(MyBool{false}));
}

TEST(common, typed_bool_has_inequality) {
    EXPECT_THAT(MyBool{false}, Ne(MyBool{true}));
}

TEST(common, DISABLED_typed_bool_cannot_be_cast_across_types) {
    struct MyBool2Tag{};
    using MyBool2 = TypedBool<MyBool2Tag>;
    //MyBool mb{MyBool2{true}}; // TODO: assert that this line is invalid, then enable test
}

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
