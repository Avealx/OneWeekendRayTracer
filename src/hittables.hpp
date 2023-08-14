#pragma once

#include <hit.hpp>
#include <hittable_list.hpp>
#include <material.hpp>
#include <quad.hpp>
#include <vec3.hpp>

#include <algorithm>
#include <memory>
#include <ranges>


std::shared_ptr<HittableList> box(point3 const & corner1,
                                  point3 const & corner2,
                                  std::shared_ptr<MaterialI> material) {
    auto result = std::make_shared<HittableList>();

    auto const min = min_components(corner1, corner2);
    auto const max = max_components(corner1, corner2);

    auto const dx = vec3(max.x - min.x, 0, 0);
    auto const dy = vec3(0, max.y - min.y, 0);
    auto const dz = vec3(0, 0, max.z - min.z);

    result->add(std::make_shared<Quad>(point3(min.x, min.y, max.z),  dx,  dy, material)); // front
    result->add(std::make_shared<Quad>(point3(max.x, min.y, max.z), -dz,  dy, material)); // right
    result->add(std::make_shared<Quad>(point3(max.x, min.y, min.z), -dx,  dy, material)); // back
    result->add(std::make_shared<Quad>(point3(min.x, min.y, min.z),  dz,  dy, material)); // left
    result->add(std::make_shared<Quad>(point3(min.x, max.y, max.z),  dx, -dz, material)); // top
    result->add(std::make_shared<Quad>(point3(min.x, min.y, min.z),  dx,  dz, material)); // bottom

    return result;
}

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

class RotatedY : public HittableI {
public:
    RotatedY(std::shared_ptr<HittableI> hittable, double const angle_degree)
        : hittable_{hittable}
        , sin_theta_{std::sin(degree_to_radians(angle_degree))}
        , cos_theta_{std::cos(degree_to_radians(angle_degree))}
        , aabb_{compute_bounding_box()} {}

    // HittableI
    HitRecord hit(Ray const & r, double t_min, double t_max) const override;
    Aabb bounding_box(TimeInterval times) const override{ return aabb_; };
private:

    vec3 rotate_y(vec3 const & vec) const {
        return vec3{cos_theta_ * vec.x + sin_theta_ * vec.z,
                    vec.y,
                    -sin_theta_ * vec.x + cos_theta_ * vec.z};
    }

    vec3 rotate_inverse_y(vec3 const & vec) const {
        return vec3{cos_theta_ * vec.x - sin_theta_ * vec.z,
                    vec.y,
                    sin_theta_ * vec.x + cos_theta_ * vec.z};
    }

    Aabb compute_bounding_box() const {
        auto const original_corners = get_all_corners(hittable_->bounding_box(TimeInterval{}));
        vec3 min_corner{infinity}, max_corner{-infinity};
        for (auto const & rotated_corner : original_corners | std::views::transform([this](vec3 const & v){ return rotate_y(v); })) {
            min_corner = min_components(min_corner, rotated_corner);
            max_corner = max_components(max_corner, rotated_corner);
        }
        return Aabb{AabbBounds{min_corner, max_corner}};
    }


    std::shared_ptr<HittableI> hittable_;
    double sin_theta_;
    double cos_theta_;
    Aabb aabb_;
};

HitRecord RotatedY::hit(Ray const & ray, double t_min, double t_max) const {
    Ray const rotated_ray(rotate_inverse_y(ray.o), rotate_inverse_y(ray.d), ray.t);
    auto hit_record = hittable_->hit(rotated_ray, t_min, t_max);
    hit_record.p = rotate_y(hit_record.p);
    hit_record.normal = rotate_y(hit_record.normal);
    return hit_record;
}