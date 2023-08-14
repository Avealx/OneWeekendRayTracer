#pragma once

#include <aabb.hpp>
#include <hit.hpp>

#include <memory>
#include <vector>

struct HittableList : HittableI {
    HittableList() = default;
    HittableList(std::shared_ptr<HittableI> object) { add(object); }
    HittableList(std::vector<std::shared_ptr<HittableI>> const & hittables) : objects{hittables} {}

    void clear() { objects.clear(); }
    void add(std::shared_ptr<HittableI> object) { objects.push_back(object); }

    // HittableI
    HitRecord hit(Ray const & r, double t_min, double t_max) const override {
        auto closest_record = HitRecord::miss();
        closest_record.t = t_max;
        //auto closest_so_far = t_max;

        for (auto const & object : objects)
            if (auto hr = object->hit(r, t_min, closest_record.t))
                closest_record = hr;

        return closest_record;
    }

    Aabb bounding_box(TimeInterval times) const override;

    // data
    std::vector<std::shared_ptr<HittableI>> objects;
};

inline Aabb HittableList::bounding_box(TimeInterval times) const {
    if (objects.empty())
        return Aabb{AabbBounds{vec3{1.0}, vec3{-1.0}}};  // invalid

    Aabb result = objects.front()->bounding_box(times);
    for (auto const object : objects)
        result = surrounding_box(result, object->bounding_box(times));
    return result;
}