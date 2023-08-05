# pragma once

#include <aabb.hpp>
#include <hit.hpp>
#include <material.hpp>
#include <ray.hpp>
#include <vec3.hpp>

struct MovingSphere : hittable_I {
    vec3 c0, c1;   // centers at time t0, t1
    double r;      // radius
    double t0, t1; // times
    std::shared_ptr<material_I> material_ptr;

    MovingSphere(vec3 const& c0,
                 vec3 const& c1,
                 double const time0,
                 double const time1,
                 double r,
                 std::shared_ptr<material_I> material_ptr = nullptr /*TODO: make proper default material*/)
    : c0{c0}, c1{c1}, t0{time0}, t1{time1}, r{r}, material_ptr{material_ptr} {}

    vec3 center(double const time) const;

    // hittable_I
    hit_record hit(Ray const & r, double t_min, double t_max) const override;
    Aabb bounding_box(TimeInterval times) const override;
};

inline vec3 MovingSphere::center(double const time) const {
    return c0 + (time - t0) / (t1 - t0) * (c1 - c0);
}

inline hit_record MovingSphere::hit(Ray const & rr, double t_min, double t_max) const {
    // TODO: unify with Sphere.hit()
    vec3 oc = rr.o - center(rr.t);
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
    vec3 const outward_normal = (result.p - center(rr.t)) / r;
    result.set_face_normal(rr, outward_normal);
    result.material_ptr = material_ptr;
    return result;
}

inline Aabb MovingSphere::bounding_box(TimeInterval times) const {
    return surrounding_box(Aabb{AabbBounds{center(times.min) - vec3{r},
                                           center(times.min) + vec3{r}}},
                           Aabb{AabbBounds{center(times.max) - vec3{r},
                                           center(times.max) + vec3{r}}});
}