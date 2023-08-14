#pragma once

#include <hit.hpp>
#include <memory>

class Translated : public HittableI {
public:
    Translated(std::shared_ptr<HittableI> hittable, vec3 const & translation)
        : hittable_{hittable}
        , translation_{translation}
        , aabb_{hittable->bounding_box(TimeInterval{}) + translation}
    {
    }

    // HittableI
    HitRecord hit(Ray const & r, double t_min, double t_max) const override;
    Aabb bounding_box(TimeInterval times) const override{ return aabb_; };

private:
    std::shared_ptr<HittableI> hittable_;
    vec3 translation_;
    Aabb aabb_;
};

HitRecord Translated::hit(Ray const & ray, double t_min, double t_max) const {
    Ray const translated_ray{ray.o - translation_, ray.d, ray.t};
    auto hit_record = hittable_->hit(translated_ray, t_min, t_max);
    hit_record.p += translation_;
    return hit_record;
}