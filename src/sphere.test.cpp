#include <sphere.hpp>

#include <ray.hpp>
#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


TEST(sphere, has_center) { Sphere{}.c; }

TEST(sphere, has_radius) { Sphere{}.r; }

TEST(sphere, has_aggregate_constructor) {
    Sphere s{{1.0, 2.0, 3.0}, 4.0};
    EXPECT_THAT(s.c, Eq(vec3{1.0, 2.0, 3.0}));
    EXPECT_THAT(s.r, Eq(4.0));
}


struct a_sphere : testing::Test {
    Sphere const s{vec3{0.0, 0.0, 0.0}, 1.0};
};

struct a_sphere_and_missing_ray : a_sphere {
    point3 const origin{0.0, 0.0, 5.0};
    vec3   const direction_miss{1.0, 0.0, 0.0};
    ray    const r{origin, direction_miss};
};

struct a_sphere_and_tangential_ray : a_sphere {
    point3 const origin_tangential{1.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -1.0};
    ray    const r{origin_tangential, direction};
};

struct a_sphere_and_ray : a_sphere {
    point3 const origin{0.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -1.0};
    ray    const r{origin, direction};
};

struct a_sphere_and_unnormalized_ray : a_sphere {
    point3 const origin{0.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -2.0};
    ray    const r{origin, direction};
};


TEST_F(a_sphere_and_missing_ray, hit_sphere_misses) {
    EXPECT_THAT(hit_sphere(s, r), Eq(-1.0));
}

TEST_F(a_sphere_and_tangential_ray, hit_sphere_hits) {
    EXPECT_THAT(hit_sphere(s, r), Eq(5.0));
}

TEST_F(a_sphere_and_ray, hit_sphere_time_is_correct_for_normalized_ray) {
    EXPECT_THAT(hit_sphere(s, r), Eq(4.0));
}

TEST_F(a_sphere_and_unnormalized_ray, hit_time_is_correct_for_unnormalized_ray) {
    EXPECT_THAT(hit_sphere(s, r), Eq(2.0));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
