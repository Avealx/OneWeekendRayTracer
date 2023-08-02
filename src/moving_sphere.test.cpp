#include <moving_sphere.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


TEST(MovingSphere, has_moving_center) {
    vec3   const c0 = {0.0, 0.0, 0.0};    vec3   const c1 = {1.0, 2.0, 3.0};
    double const t0 = 0.0;                double const t1 = 1.0;
    MovingSphere const ms{c0, c1, t0, t1, 0.5};
    EXPECT_THAT(ms.center(0.5), Eq(0.5 * (c0 + c1)));
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


