#pragma once
#include <aabb.hpp>
#include <hit.hpp>
#include <material.hpp>
#include <ray.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>

struct Sphere : hittable_I {
    vec3 c;
    double r;
    std::shared_ptr<material_I> material_ptr;

    Sphere(vec3 const& c, double r, std::shared_ptr<material_I> material_ptr = nullptr /*TODO: make proper default material*/)
    : c{c}, r{r}, material_ptr{material_ptr} {}

    // hittable_I
    hit_record hit(Ray const & r, double t_min, double t_max) const override;
    Aabb bounding_box(TimeInterval times) const override;
};

inline hit_record Sphere::hit(Ray const & rr, double t_min, double t_max) const {
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
    vec3 const outward_normal = (result.p - c) / r;
    result.set_face_normal(rr, outward_normal);
    result.material_ptr = material_ptr;
    return result;
}

inline Aabb Sphere::bounding_box(TimeInterval times) const {
    return Aabb{AabbBounds{c - vec3{r}, c + vec3{r}}};
}