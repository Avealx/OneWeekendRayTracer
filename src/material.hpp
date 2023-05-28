#pragma once

#include <color.hpp>
#include <hit.hpp>
#include <ray.hpp>


struct ScatterInfo {
    color attenuation;
    ray scattered_ray;

    static ScatterInfo const & miss() {
        static ScatterInfo the_miss{color{std::numeric_limits<double>::lowest()}, ray{}};
        return the_miss;
    }

    explicit operator bool() const { return this != &miss(); }
};

struct material_I {
    virtual ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const = 0;
};

//--------------------------------------------------------------------lambertian
class lambertian : public material_I {
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
class metal : public material_I {
public:
    explicit constexpr metal(color const & albedo, double fuzz = 0.0)
    : albedo_{albedo}
    , fuzz_{0.0 <= fuzz && fuzz <= 1.0 ? fuzz : 1.0 } {}

    // material_i
    ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const override {
        auto const scatter_direction = unit_vector(  reflect(ray_in.d, hit_rec.normal)
                                                   + fuzz_ * random_in_unit_sphere());

        ScatterInfo result;
        result.scattered_ray = ray{hit_rec.p, scatter_direction};
        result.attenuation = albedo_;
        return result;
    }

private:
    color albedo_;
    double fuzz_;
};

//--------------------------------------------------------------------dielectric
class dielectric : public material_I {
public:
    explicit constexpr dielectric(double index_of_refraction) : etaT_{index_of_refraction} {}

    // material_i
    ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const override {
        constexpr double etaI = 1.0; // assume that the other material is air
        double const refraction_ratio = hit_rec.side == FaceSide::front ? (etaI / etaT_) : etaT_ / etaI;

        vec3 const unit_direction = unit_vector(ray_in.d);
        double cos_theta = std::fmin(dot(-unit_direction, hit_rec.normal), 1.0);
        double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

        bool const choose_reflect = refraction_ratio * sin_theta > 1.0
                                    || reflectance(cos_theta, refraction_ratio) > random_double();
        vec3 const scatter_direction = choose_reflect ? reflect(unit_direction, hit_rec.normal)
                                                      : refract(unit_direction, hit_rec.normal, refraction_ratio);

        ScatterInfo result;
        result.scattered_ray = ray{hit_rec.p, scatter_direction};
        result.attenuation = color{1.0, 1.0, 1.0};
        return result;
    }

private:
    double etaT_;

    static double reflectance(double const cos, double const ref_index) {
        auto const r0 = (1 - ref_index) / (1 + ref_index);
        auto const r0_squared = r0 * r0;
        return r0_squared + (1 - r0_squared) * std::pow((1.0 - cos), 5.0);
    }
};