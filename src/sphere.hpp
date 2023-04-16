#pragma once
#include <ray.hpp>
#include <vec3.hpp>

struct Sphere {
    vec3 c;
    double r;
};

double hit_sphere(Sphere const & s, ray const & r) {
    // hit distance(s) t along ray r(t) = r.o + t*r.d:
    // (r(t) - s.o)^2 = s.r^2
    // => t^2*<r.d,r.d> + 2t<r.d,r.o-s.c> + <r.o-s.c,r.o-s.c> - s.r^2 = 0
    //
    // quadratic formular: ax^2 + bx + c = 0
    // --> x == frac{-b +- sqrt(b^2 - 4ac)}{2a}
    //
    // => b^2 - 4ac > 0 for two hits to exist (else tangential hit or miss)
    vec3 oc = r.o - s.c;
    auto a = dot(r.d, r.d);
    auto b = 2.0 * dot(oc, r.d);
    auto c = dot(oc, oc) - s.r * s.r;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return -1.0;
    return (-b - sqrt(discriminant)) / (2.0 * a);
}