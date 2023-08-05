#include <ray.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>

using ::testing::Eq;
using ::testing::Ne;


struct a_ray: ::testing::Test {
    Ray const default_constructed_ray{};
    vec3 const origin{1, 2, 3};
    point3 const direction{-1, -2, -3};
    double const time = 4.2;
    Ray const the_ray{origin, direction, time};
};

TEST_F(a_ray, default_constructor_sets_origin_to_zero) {
    EXPECT_THAT(default_constructed_ray.o, Eq(vec3{0, 0, 0}));
}

TEST_F(a_ray, default_constructor_sets_direction_to_zero) {
    EXPECT_THAT(default_constructed_ray.d, Eq(vec3{0, 0, 0}));
}

TEST_F(a_ray, can_be_constructed_using_origin_and_direction) {
    EXPECT_THAT(the_ray.d, Eq(direction));
    EXPECT_THAT(the_ray.o, Eq(origin));
}

TEST_F(a_ray, has_equality) {
    EXPECT_THAT(the_ray, Eq(Ray{the_ray.o, the_ray.d}));
}

TEST_F(a_ray, has_inequality) {
    EXPECT_THAT(the_ray, Ne(Ray{the_ray.o, the_ray.d + vec3{0.1}}));
    EXPECT_THAT(the_ray, Ne(Ray{the_ray.o + vec3{0.1}, the_ray.d}));
}

TEST_F(a_ray, can_be_printed) {
    std::stringstream ss;
    ss << the_ray;
    EXPECT_THAT(ss.str(), Eq("r((1, 2, 3)->(-1, -2, -3))"));
}

TEST_F(a_ray, has_time_default_constructed_to_zero) {
    EXPECT_THAT(default_constructed_ray.t, Eq(0.0));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
