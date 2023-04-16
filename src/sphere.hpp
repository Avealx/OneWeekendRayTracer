#pragma once
#include <hit.hpp>
#include <ray.hpp>
#include <vec3.hpp>

#include <iostream>

struct Sphere : hittable_I {
    vec3 c;
    double r;

    constexpr Sphere(vec3 const& c, double r) : c{c}, r{r} {}

    hit_record hit(ray const & r, double t_min, double t_max) const override;
};

inline hit_record Sphere::hit(ray const & rr, double t_min, double t_max) const {
    // hit distance(s) t along ray r(t) = r.o + t*r.d:
    // (r(t) - s.o)^2 = s.r^2
    // => t^2*<r.d,r.d> + 2t<r.d,r.o-s.c> + <r.o-s.c,r.o-s.c> - s.r^2 = 0
    //
    // quadratic formular: ax^2 + bx + c = 0
    // --> x = frac{-b +- sqrt(b^2 - 4ac)}{2a}
    // substitute b = 2h
    // --> x = frac{-h +- sqrt(h^2 - ac)}{a}
    vec3 oc = rr.o - c;
    auto a = rr.d.length_squared();
    auto h = dot(oc, rr.d);
    auto C = oc.length_squared() - r * r;

    auto discriminant = h*h - a*C;
    if (discriminant < 0)
        return hit_record::miss();

    // find nearest hit point
    auto sqrtd = sqrt(discriminant);
    auto root = (-h - sqrtd) / a;
    if(root < t_min || t_max < root) {
        root = -h + sqrtd / a;
        if(root < t_min || t_max < root)
            return hit_record::miss();
    }

    hit_record result;
    result.t = root;
    result.p = rr.at(result.t);
    result.normal = (result.p - c) / r;
    return result;
}

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