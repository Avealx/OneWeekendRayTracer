#pragma once


#include <aabb.hpp>
#include <hit.hpp>
#include <material.hpp>
#include <vec3.hpp>

class Quad : public HittableI {
public:
    Quad(vec3 const & corner, vec3 const & u, vec3 const & v, std::shared_ptr<MaterialI> material)
        : corner_{corner}, u_{u}, v_{v}, material_{material}
        , normal_{unit_vector(cross(u, v))}
        , d_{dot(corner_, normal_)}
        , w_{cross(u, v) / (dot(cross(u, v), cross(u, v)))}
        , aabb_{Aabb{AabbBounds{min_components(corner, corner + u + v),
                                max_components(corner, corner + u + v)}}.pad()} {}

    // HittableI
    HitRecord hit(Ray const & ray, double t_min, double t_max) const override {
        auto const denom = dot(ray.d, normal_);  // project direction onto normal

        if (std::abs(denom) < 1e-8)  // direction is parallel to plane
            return HitRecord::miss();

        auto const t = (d_ - dot(normal_, ray.o)) / denom;
        if (t < t_min || t_max < t)  // hit outside of ray interval
            return HitRecord::miss();

        auto const intersection = ray.at(t);
        vec3 const p = intersection - corner_;
        auto const alpha = dot (w_, cross(p, v_));
        auto const beta = dot (w_, cross(u_, p));
        if (alpha < 0.0 || 1.0 < alpha || beta < 0.0 || 1.0 < beta)
            return HitRecord::miss();

        HitRecord result;
        result.p = intersection;
        result.set_face_normal(ray, normal_);
        result.t = t;
        result.material_ptr = material_;
        result.uv = TextureCoordinates2d{alpha, beta};
        return result;
    };
    Aabb bounding_box(TimeInterval times) const override { return aabb_; }

private:
    vec3 const corner_;
    vec3 const u_;
    vec3 const v_;
    vec3 const normal_;
    double const d_;
    vec3 const w_;  // entity cached to speed up hit computations
    std::shared_ptr<MaterialI> const material_;
    Aabb const aabb_;
};