#pragma once

#include <ray.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <string>


enum class FaceSide {front, back};

std::string toString(FaceSide const side) {
    switch(side) {
        case FaceSide::front: return "front";
        case FaceSide::back: return "back";
    }
    return "INVALID SIDE";
}

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    FaceSide side;

    explicit operator bool() const { return ! std::isnan(t); }

    static constexpr hit_record miss() {
        return hit_record{{}, {}, std::numeric_limits<double>::quiet_NaN(), FaceSide::front};
    }

    void set_face_normal(ray const & r, vec3 const & outward_normal) {
        side = dot(r.d, outward_normal) < 0 ? FaceSide::front : FaceSide::back;
        normal = side == FaceSide::front ? outward_normal : -outward_normal;
    }
};

std::ostream& operator<<(std::ostream& out, hit_record const & hr) {
    return out << '(' <<   "p=" << hr.p
                      << ", n=" << hr.normal
                      << ", t=" << hr.t
                      << ", " << toString(hr.side)
                       << ')';
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