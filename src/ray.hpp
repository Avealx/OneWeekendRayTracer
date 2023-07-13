#pragma once

#include <vec3.hpp>

#include <iostream>

struct ray {
    point3 o; // origin
    vec3 d;   // direction
    double t; // time

    ray() = default;
    ray(point3 const & origin, vec3 const & direction, double time = 0.0)
    : o{origin}, d{direction}, t{time} {}

    point3 origin() const { return o; }
    vec3 direction() const { return d; }
    double time() const { return t; }

    point3 at(double t) const { return o + t * d; }
};

bool operator==(ray const & r1, ray const & r2) {
    return r1.origin() == r2.origin() && r1.direction() == r2.direction();
}

bool operator!=(ray const & r1, ray const & r2) {
    return !(r1 == r2);
}

std::ostream& operator<<(std::ostream& out, ray const & r) {
    return out << "r(" << r.o << "->" << r.d <<')';
}