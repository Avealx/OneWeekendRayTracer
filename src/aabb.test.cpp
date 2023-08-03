#include <aabb.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::Test;


TEST(Aabb, can_be_created) {
    Aabb const aabb{AabbBounds{vec3{0.0, 0.0, 0.0}, vec3{1.0, 1.0, 1.0}}};
}

struct AnAabb : Test {
    vec3 const min{-1.0, -2.0, -3.0};
    vec3 const max{1.0, 2.0, 3.0};
    Aabb const aabb{AabbBounds{min, max}};
};

TEST_F(AnAabb, allows_retrieving_min) {
    EXPECT_THAT(aabb.min(), Eq(min));
}

TEST_F(AnAabb, allows_retrieving_max) {
    EXPECT_THAT(aabb.max(), Eq(max));
}

TEST_F(AnAabb, can_be_hit_from_x) {
    // assume that aabb.max has positive components, fingers crossed that this wont change
    vec3 const origin{0.5 * (aabb.min() + aabb.max()) + aabb.max().x * vec3{1.0, 0.0, 0.0}};
    vec3 const direction{-1.0, 0.0, 0.0};
    ray const r{origin, direction};

    EXPECT_TRUE(aabb.hit(r, TimeInterval{0.0, 2.0 * aabb.max().x}));
}

TEST_F(AnAabb, can_be_hit_from_y) {
    // assume that aabb.max has positive components, fingers crossed that this wont change
    vec3 const origin{0.5 * (aabb.min() + aabb.max()) + aabb.max().y * vec3{0.0, 1.0, 0.0}};
    vec3 const direction{0.0, -1.0, 0.0};
    ray const r{origin, direction};

    EXPECT_TRUE(aabb.hit(r, TimeInterval{0.0, 2.0 * aabb.max().y}));
}

TEST_F(AnAabb, can_be_hit_from_z) {
    // assume that aabb.max has positive components, fingers crossed that this wont change
    vec3 const origin{0.5 * (aabb.min() + aabb.max()) + aabb.max().z * vec3{0.0, 0.0, 1.0}};
    vec3 const direction{0.0, 0.0, -1.0};
    ray const r{origin, direction};

    EXPECT_TRUE(aabb.hit(r, TimeInterval{0.0, 2.0 * aabb.max().z}));
}

TEST_F(AnAabb, can_be_missed) {
    vec3 const origin{2.0 * aabb.max()};
    vec3 const direction{1.0, 0.0, 0.0};
    ray const r{origin, direction};

    EXPECT_FALSE(aabb.hit(r, TimeInterval{0.0, 2.0 * aabb.max().z}));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
