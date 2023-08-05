#pragma once

#include <vec3.hpp>

#include <iostream>

struct Ray {
    point3 o; // origin
    vec3 d;   // direction
    double t; // time

    Ray() = default;
    Ray(point3 const & origin, vec3 const & direction, double time = 0.0)
    : o{origin}, d{direction}, t{time} {}

    point3 origin() const { return o; }
    vec3 direction() const { return d; }
    double time() const { return t; }

    point3 at(double t) const { return o + t * d; }
};

bool operator==(Ray const & r1, Ray const & r2) {
    return r1.origin() == r2.origin() && r1.direction() == r2.direction();
}

bool operator!=(Ray const & r1, Ray const & r2) {
    return !(r1 == r2);
}

std::ostream& operator<<(std::ostream& out, Ray const & r) {
    return out << "r(" << r.o << "->" << r.d <<')';
}