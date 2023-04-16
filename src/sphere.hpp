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
    // --> x = frac{-b +- sqrt(b^2 - 4ac)}{2a}
    // substitute b = 2h
    // --> x = frac{-h +- sqrt(h^2 - ac)}{a}
    vec3 oc = r.o - s.c;
    auto a = r.d.length_squared();
    auto h = dot(oc, r.d);
    auto c = oc.length_squared() - s.r * s.r;
    auto discriminant = h*h - a*c;
    if (discriminant < 0)
        return -1.0;
    return (-h - sqrt(discriminant)) / a;
}