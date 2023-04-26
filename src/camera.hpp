#pragma once

#include <ray.hpp>
#include <vec3.hpp>

class camera {
public:
    camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * aspect_ratio;
        auto focal_length = 1.0;

        origin_ = point3{0.0, 0.0, 0.0};
        horizontal_ = vec3{viewport_width, 0.0, 0.0};
        vertical_ = vec3{0.0, viewport_height, 0.0};
        lower_left_corner_ = origin_ - 0.5*horizontal_ - 0.5*vertical_ - vec3{0.0, 0.0, focal_length};
    }

    ray get_ray(double h, double v) const {
        return ray{origin_, lower_left_corner_ + h*horizontal_+ v*vertical_ - origin_};
    }

private:
    point3 origin_;
    point3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
};