#pragma once

#include <common.hpp>
#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

using AabbBounds = TypedInterval<vec3, struct AabbBoundsTag>;

class Aabb {
public:
    explicit Aabb(AabbBounds const & bounds) : bounds_{bounds} {}

    vec3 min() const { return bounds_.min; }
    vec3 max() const { return bounds_.max; }

    bool hit(ray const & r, TimeInterval times) const;

private:
    AabbBounds const bounds_;
};

bool Aabb::hit(ray const & r, TimeInterval times) const {
    for (std::size_t dim = 0; dim < 3; ++dim) {
        auto const inverse_direction = 1.0 / r.d[dim];
        auto t0 = (bounds_.min[dim] - r.o[dim]) * inverse_direction;
        auto t1 = (bounds_.max[dim] - r.o[dim]) * inverse_direction;
        if (inverse_direction < 0.0)
            std::swap(t0, t1);
        times.min = t0 > times.min ? t0 : times.min;
        times.max = t1 < times.max ? t1 : times.max;
        if (times.max <= times.min)
            return false;
    }
    return true;
}