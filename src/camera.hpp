#pragma once

#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

class camera {
public:
    camera(
        double const vertical_fov_degree = 90.0,
        double const aspect_ratio = 16.0 / 9.0
    ) {
        auto const vertical_fov_rad = degree_to_radians(vertical_fov_degree);
        auto const focal_length = 1.0;
        auto const h = focal_length * std::tan(vertical_fov_rad / 2.0);
        auto const viewport_height = 2.0 * h;
        auto const viewport_width = aspect_ratio * viewport_height;

        origin_ = point3{0.0, 0.0, 0.0};
        horizontal_ = vec3{viewport_width, 0.0, 0.0};
        vertical_ = vec3{0.0, viewport_height, 0.0};
        lower_left_corner_ = origin_ - 0.5*horizontal_ - 0.5*vertical_ - vec3{0.0, 0.0, focal_length};
    }

    ray get_ray(double h, double v) const {
        return ray{origin_, lower_left_corner_ + h*horizontal_+ v*vertical_ - origin_};
    }

protected:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
};