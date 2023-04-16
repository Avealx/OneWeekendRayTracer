#include <sphere.hpp>

#include <ray.hpp>
#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


TEST(sphere, has_center) { Sphere{}.c; }

TEST(sphere, has_radius) { Sphere{}.r; }

TEST(sphere, has_aggregate_constructor) {
    Sphere s{{1, 2, 3}, 4};
    EXPECT_THAT(s.c, Eq(vec3{1, 2, 3}));
    EXPECT_THAT(s.r, Eq(4));
}

TEST(sphere, hit_sphere_misses) {
    Sphere s{vec3{0, 0, 0}, 1};
    point3 origin{0, 0, 4};
    vec3 direction{1, 0, 0};
    ray r{origin, direction};
    EXPECT_FALSE(hit_sphere(s, r));
}

TEST(sphere, hit_sphere_hits) {
    Sphere s{vec3{0, 0, 0}, 1};
    point3 origin{0, 0, 1};
    vec3 direction{0, 0, -2};
    ray r{origin, direction};
    EXPECT_TRUE(hit_sphere(s, r));
}

TEST(sphere, hit_sphere_misses_tangential) {
    Sphere s{vec3{0, 0, 0}, 1};
    point3 origin{1, 0, 4};
    vec3 direction{0, 0, -2};
    ray r{origin, direction};
    EXPECT_FALSE(hit_sphere(s, r));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}