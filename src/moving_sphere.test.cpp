#include <moving_sphere.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::Test;


struct AMovingSphere : Test {
    vec3   const center0 = {0.0, 0.0, 0.0};
    vec3   const center1 = {1.0, 2.0, 3.0};
    double const time0 = 0.0;
    double const time1 = 1.0;
    double const radius = 0.5;
    MovingSphere const ms{center0, center1, time0, time1, radius};
};

TEST_F(AMovingSphere, has_moving_center) {
    EXPECT_THAT(ms.center(0.5), Eq(0.5 * (center0 + center1)));
}

TEST_F(AMovingSphere, has_correct_bounding_box) {
    auto const aabb = ms.bounding_box(TimeInterval{time0, time1});
    EXPECT_THAT(aabb.min(), Eq(center0 - vec3{radius}));
    EXPECT_THAT(aabb.max(), Eq(center1 + vec3{radius}));
}

TEST(MovingSphere, is_hit_correctly) {
    vec3   const c0 = {0.0, 0.0, 0.0};    vec3   const c1 = {10.0, 0.0, 0.0};
    double const t0 = 0.0;                double const t1 = 1.0;
    MovingSphere const ms{c0, c1, t0, t1, 0.005};
    ray const r{point3(5.0, 0.0, 1.0), vec3{0.0, 0.0, -1.0}, 0.5 * (t0 + t1)};
    EXPECT_TRUE(ms.hit(r, 0.0, 10.0));
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


