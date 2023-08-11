#pragma once
#include <aabb.hpp>
#include <hit.hpp>
#include <material.hpp>
#include <ray.hpp>
#include <texture.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>

struct Sphere : HittableI {
    vec3 c;
    double r;
    std::shared_ptr<MaterialI> material_ptr;

    Sphere(vec3 const& c, double r, std::shared_ptr<MaterialI> material_ptr = nullptr /*TODO: make proper default material*/)
    : c{c}, r{r}, material_ptr{material_ptr} {}

    // HittableI
    HitRecord hit(Ray const & r, double t_min, double t_max) const override;
    Aabb bounding_box(TimeInterval times) const override;

    TextureCoordinates2d get_uv(point3 const p) const;
};

inline HitRecord Sphere::hit(Ray const & ray, double t_min, double t_max) const {
    // hit distance(s) t along ray r(t) = r.o + t*r.d:
    // (r(t) - s.o)^2 = s.r^2
    // => t^2*<r.d,r.d> + 2t<r.d,r.o-s.c> + <r.o-s.c,r.o-s.c> - s.r^2 = 0
    //
    // quadratic formular: ax^2 + bx + c = 0
    // --> x = frac{-b +- sqrt(b^2 - 4ac)}{2a}
    // substitute b = 2h
    // --> x = frac{-h +- sqrt(h^2 - ac)}{a}
    vec3 oc = ray.o - c;
    auto a = ray.d.length_squared();
    auto h = dot(oc, ray.d);
    auto C = oc.length_squared() - r * r;

    auto discriminant = h*h - a*C;
    if (discriminant < 0)
        return HitRecord::miss();

    // find nearest hit point
    auto sqrtd = sqrt(discriminant);
    auto root = (-h - sqrtd) / a;
    if(root < t_min || t_max < root) {
        root = -h + sqrtd / a;
        if(root < t_min || t_max < root)
            return HitRecord::miss();
    }

    HitRecord result;
    result.t = root;
    result.p = ray.at(result.t);
    vec3 const outward_normal = (result.p - c) / r;
    result.set_face_normal(ray, outward_normal);
    result.uv = get_uv(outward_normal);
    result.material_ptr = material_ptr;
    return result;
}

inline Aabb Sphere::bounding_box(TimeInterval times) const {
    return Aabb{AabbBounds{c - vec3{r}, c + vec3{r}}};
}

// p is a point on the sphere of ratius 1 centered at the origin.
inline TextureCoordinates2d Sphere::get_uv(point3 const p) const {
    auto const theta = std::acos(-p.y);
    auto const phi = std::atan2(-p.z, p.x) + pi;
    return {phi / (2.0 * pi), theta / pi};
}