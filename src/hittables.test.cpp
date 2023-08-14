#include <hittables.hpp>

#include <quad.hpp>
#include <sphere.hpp>  // unnecessary
#include <material.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Eq;
using testing::Test;

TEST(Translated, can_be_constructed) {
    Translated const t{std::make_shared<Sphere>(point3{}, 1.0), vec3(0.0)};
}

struct MockMaterial : MaterialI {
    ScatterInfo scatter(Ray const & ray_in, HitRecord const & hit_rec) const override {
        return ScatterInfo::miss();
    }
};

struct ATranslated : Test {
    vec3 const corner{-2.0, -0.5, 0.0};
    vec3 const u{4.0, 0.0, 0.0};
    vec3 const v{0.0, 1.0, 0.0};
    std::shared_ptr<HittableI> original_quad_ptr = std::make_shared<Quad>(corner, u, v, std::make_shared<MockMaterial>());
    vec3 const translation{1.0, 2.0, 1.0};
    Translated const translated{original_quad_ptr, translation};
};

TEST_F(ATranslated, has_correct_bounding_box) {
    auto const original_aabb = original_quad_ptr->bounding_box(TimeInterval{});
    auto const translated_aabb = translated.bounding_box(TimeInterval{});
    EXPECT_THAT(translated_aabb.min(), Eq(original_aabb.min() + translation));
    EXPECT_THAT(translated_aabb.max(), Eq(original_aabb.max() + translation));
}

TEST_F(ATranslated, can_be_hit) {
    point3 const origin = corner + 0.5 * (u + v) + translation + vec3{0.0, 0.0, 1.0};
    vec3 const direction{0.0 , 0.0, -1.0};
    Ray const ray{origin, direction};
    auto const hit_record = translated.hit(ray, 0.0, infinity);
    EXPECT_TRUE(hit_record);
    EXPECT_THAT(hit_record.p, Eq(point3{origin.x, origin.y, (corner+translation).z}));
}

TEST_F(ATranslated, can_be_missed) {
    point3 const origin = corner + 0.5 * (u + v) + vec3{0.0, 0.0, 1.0};
    vec3 const direction{0.0 , 0.0, -1.0};
    Ray const ray{origin, direction};
    EXPECT_FALSE(translated.hit(ray, 0.0, infinity));
}


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}