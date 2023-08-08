#pragma once

#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

#include <iostream>

using FieldOfView = StrongType<double, struct FieldOfViewTag>;
using AspectRatio = StrongType<double, struct AspectRatioTag>;
using Aperture = StrongType<double, struct ApertureTag>;
using FocusDistance = StrongType<double, struct FocusDistanceTag>;

class Camera {
public:
    Camera(
        point3 const lookfrom = point3{0.0, 0.0, 0.0},
        point3 const lookat = point3{0.0, 0.0, -1.0},
        point3 const vertical_up = vec3{0.0, 1.0, 0.0},
        FieldOfView const vertical_fov_degree = FieldOfView{90.0},
        AspectRatio const aspect_ratio = AspectRatio{16.0 / 9.0},
        Aperture const aperture = Aperture{0.0},
        FocusDistance const focus_dist = FocusDistance{1.0},
        double const time0 = 0.0,
        double const time1 = 1.0
    )
        : origin_{lookfrom}
        , lens_radius_{aperture.value() / 2.0}
        , time0_{time0}
        , time1_{time1}
     {
        auto const vertical_fov_rad = degree_to_radians(vertical_fov_degree.value());
        auto const focal_length = 1.0;
        auto const h = focal_length * std::tan(vertical_fov_rad / 2.0);
        auto const viewport_height = 2.0 * h;
        auto const viewport_width = aspect_ratio.value() * viewport_height;

        w_ = unit_vector(lookfrom - lookat);
        u_ = unit_vector(cross(vertical_up, w_));
        v_ = cross(w_, u_);

        horizontal_ = focus_dist.value() * viewport_width * u_;
        vertical_ = focus_dist.value() * viewport_height * v_;
        lower_left_corner_ = origin_ - 0.5*horizontal_ - 0.5*vertical_ - focus_dist.value() * w_;
    }

    Ray get_ray(double h, double v) const {
        vec3 const rd = lens_radius_ * random_in_unit_disk();
        vec3 const offset = u_ * rd.x + v_ * rd.y;
        vec3 const origin_in_lens = origin_ + offset;
        return Ray{origin_in_lens,
                   lower_left_corner_ + h*horizontal_+ v*vertical_ - origin_in_lens,
                   random_double(time0_, time1_)};
    }

protected:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 u_, v_, w_;
    double lens_radius_;
    double time0_, time1_;
};