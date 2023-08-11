#include <quad.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>


using testing::Eq;
using testing::Gt;
using testing::Test;


struct DummyMaterial : MaterialI {
    ScatterInfo scatter(Ray const & ray_in, HitRecord const & hit_rec) const override {
        return ScatterInfo::miss();
    }
};

TEST(Quad, can_be_constructed) {
    vec3 const corner{1.0, 1.0, 1.0};
    vec3 const u{1.0, 0.0, 0.0};
    vec3 const v{0.0, 2.0, 0.0};
    Quad const quad{corner, u, v, std::make_shared<DummyMaterial>()};
}

struct AnAxisAlignedQuad : Test {
    vec3 const corner{1.0, 1.0, 1.0};
    vec3 const u{1.0, 0.0, 0.0};
    vec3 const v{0.0, 2.0, 0.0};
    Quad const quad{corner, u, v, std::make_shared<DummyMaterial>()};
};

TEST_F(AnAxisAlignedQuad, has_nonzero_aabb_thicknes) {
    Aabb const aabb = quad.bounding_box(TimeInterval{});
    vec3 const lengths = aabb.max() - aabb.min();
    EXPECT_THAT(lengths.x, Gt(0.0));
    EXPECT_THAT(lengths.y, Gt(0.0));
    EXPECT_THAT(lengths.z, Gt(0.0));
}

TEST_F(AnAxisAlignedQuad, can_be_hit) {
    vec3 const direction = cross(u, v);
    point3 const origin = corner + 0.5 * (u + v) - direction;
    Ray const ray{origin, direction};
    auto const hit_record = quad.hit(ray, 0.0, infinity);
    EXPECT_TRUE(hit_record);
}

TEST_F(AnAxisAlignedQuad, can_be_missed) {
    vec3 const direction = cross(u, v);
    point3 const origin = corner - u - v - direction;
    Ray const ray{origin, direction};
    auto const hit_record = quad.hit(ray, 0.0, infinity);
    EXPECT_FALSE(hit_record);
}

TEST_F(AnAxisAlignedQuad, returns_correct_hit_location) {
    vec3 const direction = cross(u, v);
    point3 const origin = corner + 0.25 * u + 0.5 * v - direction;
    Ray const ray{origin, direction};
    auto const hit_record = quad.hit(ray, 0.0, infinity);
    EXPECT_THAT(hit_record.uv, Eq(TextureCoordinates2d{0.25, 0.5}));
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}