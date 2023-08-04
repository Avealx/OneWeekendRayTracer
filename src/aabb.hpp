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

    explicit operator bool() const {
        return bounds_.min.x <= bounds_.max.x &&
               bounds_.min.y <= bounds_.max.y &&
               bounds_.min.z <= bounds_.max.z;
    }

private:
    AabbBounds bounds_;
};

inline bool Aabb::hit(ray const & r, TimeInterval times) const {
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

Aabb surrounding_box(Aabb const & aabb0, Aabb const & aabb1) {
    return Aabb{AabbBounds{
        min_components(aabb0.min(), aabb1.min()),
        max_components(aabb0.max(), aabb1.max())
    }};
}