#pragma once

#include <interval.hpp>
#include <ray.hpp>
#include <vec3.hpp>

#include <cmath>

using AabbBounds = TypedInterval<vec3, struct AabbBoundsTag>;

template <>
struct TypedIntervalTraits<vec3, struct AabbBoundsTag> {
    static vec3 length(vec3 const & difference) { return difference; }
};

class Aabb {
public:
    explicit Aabb(AabbBounds const & bounds) : bounds_{bounds} {}

    vec3 min() const { return bounds_.min; }
    vec3 max() const { return bounds_.max; }

    bool hit(Ray const & r, TimeInterval times) const;

    explicit operator bool() const {
        return bounds_.min.x <= bounds_.max.x &&
               bounds_.min.y <= bounds_.max.y &&
               bounds_.min.z <= bounds_.max.z;
    }

    Aabb pad() const { return ensure_width(0.0001); }

    Aabb ensure_width(double const delta) const {
        vec3 const widths = bounds_.length();
        AabbBounds const bounds = {
            vec3{widths.x < delta ? bounds_.min.x - 0.5 * delta : bounds_.min.x,
                 widths.y < delta ? bounds_.min.y - 0.5 * delta : bounds_.min.y,
                 widths.z < delta ? bounds_.min.z - 0.5 * delta : bounds_.min.z },
            vec3{widths.x < delta ? bounds_.max.x + 0.5 * delta : bounds_.max.x,
                 widths.y < delta ? bounds_.max.y + 0.5 * delta : bounds_.max.y,
                 widths.z < delta ? bounds_.max.z + 0.5 * delta : bounds_.max.z }};
        return Aabb{bounds};
    }

private:
    AabbBounds bounds_;
};

inline bool Aabb::hit(Ray const & r, TimeInterval times) const {
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