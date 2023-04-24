#pragma once

#include <hit.hpp>

#include <memory>
#include <vector>

struct hittable_list : hittable_I {
    hittable_list() = default;
        hittable_list(std::shared_ptr<hittable_I> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<hittable_I> object) { objects.push_back(object); }
    // hittable_I
    hit_record hit(ray const & r, double t_min, double t_max) const override {
        auto closest_record = hit_record::miss();
        closest_record.t = t_max;
        //auto closest_so_far = t_max;

        for (auto const & object : objects)
            if (auto hr = object->hit(r, t_min, closest_record.t))
                closest_record = hr;

        return closest_record;
    }

    // data
    std::vector<std::shared_ptr<hittable_I>> objects;
};