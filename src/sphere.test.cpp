#include <sphere.hpp>

#include <hit.hpp>
#include <ray.hpp>
#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


TEST(sphere, has_center) { Sphere{vec3{}, 0.0}.c; }

TEST(sphere, has_radius) { Sphere{vec3{}, 0.0}.r; }

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
    Ray    const r{origin, direction_miss};
};

struct a_sphere_and_tangential_ray : a_sphere {
    point3 const origin_tangential{1.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -1.0};
    Ray    const r{origin_tangential, direction};
};

struct a_sphere_and_ray : a_sphere {
    point3 const origin{0.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -1.0};
    Ray    const r{origin, direction};
};

struct a_sphere_and_unnormalized_ray : a_sphere {
    point3 const origin{0.0, 0.0, 5.0};
    vec3   const direction{0.0, 0.0, -2.0};
    Ray    const r{origin, direction};
};

TEST_F(a_sphere, has_correct_bounding_box) {
    EXPECT_THAT(s.bounding_box(TimeInterval{}).min(), Eq(s.c - vec3{s.r}));
    EXPECT_THAT(s.bounding_box(TimeInterval{}).max(), Eq(s.c + vec3{s.r}));
}

TEST_F(a_sphere, has_correct_uv_coordinates) {
    EXPECT_THAT(s.get_uv(point3{1.0, 0.0, 0.0}), Eq(TextureCoordinates2d{0.5,  0.5}));
    EXPECT_THAT(s.get_uv(point3{0.0, 1.0, 0.0}), Eq(TextureCoordinates2d{0.5,  1.0}));
    EXPECT_THAT(s.get_uv(point3{0.0, 0.0, 1.0}), Eq(TextureCoordinates2d{0.25, 0.5}));
    EXPECT_THAT(s.get_uv(point3{-1.0, 0.0, 0.0}), Eq(TextureCoordinates2d{0.0,  0.5}));
    EXPECT_THAT(s.get_uv(point3{0.0, -1.0, 0.0}), Eq(TextureCoordinates2d{0.5,  0.0}));
    EXPECT_THAT(s.get_uv(point3{0.0, 0.0, -1.0}), Eq(TextureCoordinates2d{0.75, 0.5}));
}

TEST_F(a_sphere_and_missing_ray, hit_misses) {
    EXPECT_THAT(s.hit(r, 0.0, 10.0), Eq(HitRecord::miss()));
}

TEST_F(a_sphere_and_tangential_ray, hit_sphere_hits) {
    HitRecord const expected_hit_record{point3{1.0, 0.0, 0.0}, vec3{-1.0, 0.0, 0.0}, nullptr, 5.0, {} /*tex crd*/, FaceSide::back};
    EXPECT_THAT(s.hit(r, 0.0, 10.0), Eq(expected_hit_record));
}

TEST_F(a_sphere_and_ray, hit_sphere_time_is_correct_for_normalized_ray) {
    EXPECT_THAT(s.hit(r, 0.0, 10.0).t, Eq(4.0));
}

TEST_F(a_sphere_and_unnormalized_ray, hit_time_is_correct_for_unnormalized_ray) {
    EXPECT_THAT(s.hit(r, 0.0, 10.0).t, Eq(2.0));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
