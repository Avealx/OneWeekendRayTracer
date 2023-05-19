#pragma once

#include <color.hpp>
#include <hit.hpp>
#include <ray.hpp>


struct ScatterInfo {
    color attenuation;
    ray scattered_ray;
};

struct material_I {
    virtual ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const = 0;
};

//--------------------------------------------------------------------lambertian
class lambertian : material_I {
public:
    explicit constexpr lambertian(color const & albedo) : albedo_{albedo} {}

    // material_i
    ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const override {
        auto scatter_direction = hit_rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = hit_rec.normal;

        ScatterInfo result;
        result.scattered_ray = ray{hit_rec.p, scatter_direction};
        result.attenuation = albedo_;
        return result;
    }

private:
    color albedo_;
};

//-------------------------------------------------------------------------metal
class metal : material_I {
public:
    explicit constexpr metal(color const & albedo) : albedo_{albedo} {}

    // material_i
    ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const override {
        auto const scatter_direction = reflect(unit_vector(ray_in.d), hit_rec.normal);

        ScatterInfo result;
        result.scattered_ray = ray{hit_rec.p, scatter_direction};
        result.attenuation = albedo_;
        return result;
    }

private:
    color albedo_;
};