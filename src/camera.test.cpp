#include <camera.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::DoubleEq;


TEST(camera, can_be_created) {
    camera{};
}

struct TestableCamera : public camera {
    TestableCamera( double const vertical_fov_degree, double const aspect_ratio
    ) : camera{vertical_fov_degree, aspect_ratio} {}

    using camera::lower_left_corner_;
};

// TODO: make proper matcher
void expect_double_equal(vec3 const & u, vec3 const & v) {
    EXPECT_THAT(u.x, DoubleEq(v.x));
    EXPECT_THAT(u.y, DoubleEq(v.y));
    EXPECT_THAT(u.z, DoubleEq(v.z));
}

TEST(camera, can_be_created_using_vertical_field_of_view_and_aspect_ratio) {
    double const vertical_fov_deg = 90;
    double const aspect_ratio = 2.0;
    TestableCamera cam{vertical_fov_deg, aspect_ratio};
    expect_double_equal(cam.lower_left_corner_, vec3{-2.0, -1.0, -1.0});
}

Test

TEST(camera, ray_to_center_of_image_is_correct) {
    EXPECT_THAT(camera{}.get_ray(0.5, 0.5), Eq(ray{{0.0,0.0,0.0}, {0.0, 0.0, -1.0}}));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

