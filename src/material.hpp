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
        constexpr double eta = 1.0; // assume that the other material is air
        double const refraction_ratio = hit_rec.side == FaceSide::front ? (1.0 / etaT_) : etaT_;
        auto const scatter_direction = refract(unit_vector(ray_in.d), hit_rec.normal, refraction_ratio);

        ScatterInfo result;
        result.scattered_ray = ray{hit_rec.p, scatter_direction};
        result.attenuation = color{1.0, 1.0, 1.0};
        return result;
    }

private:
    double etaT_;
};