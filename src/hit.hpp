#pragma once

#include <ray.hpp>

#include <iostream>
#include <cmath>
#include <limits>


struct hit_record {
    point3 p;
    vec3 normal;
    double t;

    explicit operator bool() const { return ! std::isnan(t); }

    static constexpr hit_record miss() {
        return hit_record{{}, {}, std::numeric_limits<double>::quiet_NaN()};
    }
};

std::ostream& operator<<(std::ostream& out, hit_record const & hr) {
    return out << '(' <<   "p=" << hr.p
                      << ", n=" << hr.normal
                      << ", t=" << hr.t << ')' ;
}

bool operator==(hit_record const & lhs, hit_record const & rhs) {
    return (!bool{lhs} && !bool{rhs}) ||
           (lhs.p == rhs.p && lhs.normal == rhs.normal && lhs.t == rhs.t);
}

bool operator!=(hit_record const & lhs, hit_record const & rhs) {
    return !(lhs == rhs);
}

struct hittable_I {
    virtual hit_record hit(ray const & r, double t_min, double t_max) const = 0;
};