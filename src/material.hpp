#pragma once

#include <color.hpp>
#include <hit.hpp>
#include <ray.hpp>

class hit_record;

struct ScatterInfo {
    color attenuation;
    ray scattered_ray;
};

struct material_I {
    virtual ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const = 0;
};