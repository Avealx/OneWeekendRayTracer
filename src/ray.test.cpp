#include <ray.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using ::testing::Eq;


struct a_ray: ::testing::Test {
    ray const default_constructed_ray{};
    vec3 const origin{1, 2, 3};
    point3 const direction{-1, -2, -3};
    ray const the_ray{origin, direction};
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


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
