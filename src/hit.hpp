#pragma once

#include <aabb.hpp>
#include <ray.hpp>
#include <texture.hpp>

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>


class material_I;

enum class FaceSide {front, back, miss /*indicates a miss hit_record*/};

std::string toString(FaceSide const side) {
    switch(side) {
        case FaceSide::front: return "front";
        case FaceSide::back:  return "back";
        case FaceSide::miss:  return "miss";
    }
    return "INVALID SIDE";
}

struct hit_record {
    point3 p;
    vec3 normal;
    std::shared_ptr<material_I> material_ptr;
    double t;
    TextureCoordinates2d uv;
    FaceSide side;

    explicit operator bool() const { return side != FaceSide::miss; }

    static hit_record miss() {
        return hit_record{{}, {}, nullptr, std::numeric_limits<double>::quiet_NaN(), {}, FaceSide::miss};
    }

    void set_face_normal(Ray const & r, vec3 const & outward_normal) {
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
    virtual hit_record hit(Ray const & r, double t_min, double t_max) const = 0;
    virtual Aabb bounding_box(TimeInterval times) const = 0;
};