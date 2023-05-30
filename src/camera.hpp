#pragma once

#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

class camera {
public:
    camera(
        point3 const lookfrom = point3{0.0, 0.0, 0.0},
        point3 const lookat = point3{0.0, 0.0, -1.0},
        point3 const vertical_up = vec3{0.0, 1.0, 0.0},
        double const vertical_fov_degree = 90.0,
        double const aspect_ratio = 16.0 / 9.0
    ) {
        auto const vertical_fov_rad = degree_to_radians(vertical_fov_degree);
        auto const focal_length = 1.0;
        auto const h = focal_length * std::tan(vertical_fov_rad / 2.0);
        auto const viewport_height = 2.0 * h;
        auto const viewport_width = aspect_ratio * viewport_height;

        auto const w = unit_vector(lookfrom - lookat);
        auto const u = unit_vector(cross(vertical_up, w));
        auto const v = cross(w, u);

        origin_ = lookfrom;
        horizontal_ = viewport_width * u;
        vertical_ = viewport_height * v;
        lower_left_corner_ = origin_ - 0.5*horizontal_ - 0.5*vertical_ - w;
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