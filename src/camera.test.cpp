#include <camera.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::AllOf;
using testing::DoubleEq;
using testing::Eq;
using testing::Ge;
using testing::Lt;
using testing::Ne;


TEST(camera, can_be_created) {
    camera{};
}

// A camera derivative that allows to access the internals for testing purposes.
struct TestableCamera : public camera {
    TestableCamera(
        point3 const lookfrom = vec3{0.0, 0.0, 0.0},
        point3 const lookat = vec3{0.0, 0.0, -1.0},
        point3 const vertical_up = vec3{0.0, 1.0, 0.0},
        FieldOfView const vertical_fov_degree = FieldOfView{90.0},
        AspectRatio const aspect_ratio = AspectRatio{16.0 / 9.0},
        Aperture const aperture = Aperture{0.0},
        FocusDistance const focus_dist = FocusDistance{1.0},
        double const time0 = 0.5,
        double const time1 = 1.0
    ) : camera{lookfrom,
               lookat,
               vertical_up,
               vertical_fov_degree,
               aspect_ratio,
               aperture,
               focus_dist,
               time0,
               time1} {}

    TestableCamera(
        FieldOfView const vertical_fov_degree,
        AspectRatio const aspect_ratio
    ) : camera{ point3{0.0, 0.0, 0.0}, point3{0.0, 0.0, -1.0}, vec3{0.0, 1.0, 0.0}, vertical_fov_degree, aspect_ratio} {}

    using camera::lower_left_corner_;
    using camera::time0_;
    using camera::time1_;
};

// TODO: make proper matcher
void expect_double_equal(vec3 const & u, vec3 const & v) {
    EXPECT_THAT(u.x, DoubleEq(v.x));
    EXPECT_THAT(u.y, DoubleEq(v.y));
    EXPECT_THAT(u.z, DoubleEq(v.z));
}

// TODO: after adding the next test, I cannot give only these two fields to the ctor.
//       think about how to do this nicely and then continue at linsting 64
TEST(camera, can_be_created_using_vertical_field_of_view_and_aspect_ratio) {
    FieldOfView const vertical_fov_deg{90.0};
    AspectRatio const aspect_ratio{2.0};
    TestableCamera cam{vertical_fov_deg, aspect_ratio};
    expect_double_equal(cam.lower_left_corner_, vec3{-2.0, -1.0, -1.0});
}

TEST(camera, can_be_positioned) {
    point3 const lookfrom{0.0, 1.0, 0.0};
    point3 const lookat{0.0, 0.0, 0.0};
    vec3 const vertical_up{0.0, 0.0, -1.0};
    auto const vertical_fov_deg = FieldOfView{90.0};
    auto const aspect_ratio = AspectRatio{2.0};
    TestableCamera cam{lookfrom, lookat, vertical_up, vertical_fov_deg, aspect_ratio};
    expect_double_equal(cam.lower_left_corner_, vec3{-2.0, 0.0, 1.0});
}

TEST(camera, ray_to_center_of_image_is_correct) {
    EXPECT_THAT(camera{}.get_ray(0.5, 0.5), Eq(Ray{{0.0,0.0,0.0}, {0.0, 0.0, -1.0}}));
}

TEST(camera, ray_to_center_of_image_has_random_origin_according_to_aperture) {
    point3 const lookfrom = vec3{0.0, 0.0, 0.0};
    point3 const lookat = vec3{0.0, 0.0, -1.0};
    point3 const vertical_up = vec3{0.0, 1.0, 0.0};
    FieldOfView const vertical_fov_degree{90.0};
    auto const aspect_ratio = AspectRatio{16.0 / 9.0};
    auto const aperture = Aperture{2.0};
    auto const focus_dist = FocusDistance{10.0};
    camera const cam{lookfrom,
                     lookat,
                     vertical_up,
                     vertical_fov_degree,
                     aspect_ratio,
                     aperture,
                     focus_dist};

    Ray const r1 = cam.get_ray(0.5, 0.5);
    Ray const r2 = cam.get_ray(0.5, 0.5);

    EXPECT_THAT(r1, Ne(r2));
    EXPECT_THAT((r1.o - r2.o).length(), Lt(aperture.value()));
}

TEST(camera, emits_ray_with_time_inside_cameras_time_bounds) {
    TestableCamera const cam{};
    auto const ray = cam.get_ray(0.5, 0.5);
    EXPECT_THAT(ray.time(), AllOf(Ge(cam.time0_), Lt(cam.time1_)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

