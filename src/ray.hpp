#pragma once

#include <vec3.hpp>

struct ray {
    point3 o; // origin
    vec3 d;   // direction

    ray() = default;
    ray(point3 const & origin, vec3 const & direction)
    : o{origin}, d{direction} {}

    point3 origin() const { return o; }
    vec3 direction() const { return d; }

    point3 at(double t) const { return o + t * d; }
};