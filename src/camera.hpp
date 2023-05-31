#pragma once

#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

#include <iostream>

class camera {
public:
    camera(
        point3 const lookfrom = point3{0.0, 0.0, 0.0},
        point3 const lookat = point3{0.0, 0.0, -1.0},
        point3 const vertical_up = vec3{0.0, 1.0, 0.0},
        double const vertical_fov_degree = 90.0,
        double const aspect_ratio = 16.0 / 9.0,
        double const aperture = 0.0,
        double const focus_dist = 1.0
    )
        : origin_{lookfrom}
        , lens_radius_{aperture / 2.0}
     {
        auto const vertical_fov_rad = degree_to_radians(vertical_fov_degree);
        auto const focal_length = 1.0;
        auto const h = focal_length * std::tan(vertical_fov_rad / 2.0);
        auto const viewport_height = 2.0 * h;
        auto const viewport_width = aspect_ratio * viewport_height;

        w_ = unit_vector(lookfrom - lookat);
        u_ = unit_vector(cross(vertical_up, w_));
        v_ = cross(w_, u_);

        horizontal_ = focus_dist * viewport_width * u_;
        vertical_ = focus_dist * viewport_height * v_;
        lower_left_corner_ = origin_ - 0.5*horizontal_ - 0.5*vertical_ - focus_dist * w_;
    }

    ray get_ray(double h, double v) const {
        vec3 const rd = lens_radius_ * random_in_unit_disk();
        vec3 const offset = u_ * rd.x + v_ * rd.y;
        vec3 const origin_in_lens = origin_ + offset;
        return ray{origin_in_lens,
                   lower_left_corner_ + h*horizontal_+ v*vertical_ - origin_in_lens};
    }

protected:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 u_, v_, w_;
    double lens_radius_;
};