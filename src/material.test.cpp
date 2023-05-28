#include <material.hpp>

#include <color.hpp>
#include <ray.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <typeinfo>


using ::testing::DoubleEq;
using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::Lt;
using ::testing::Test;


TEST(scatter_info, has_attenuation) {
    EXPECT_THAT(typeid(ScatterInfo{}.attenuation).name(), Eq(typeid(color).name()));
}

TEST(scatter_info, has_scattered_ray) {
    EXPECT_THAT(typeid(ScatterInfo{}.scattered_ray).name(), Eq(typeid(ray).name()));
}

TEST(scatter_info, can_be_cast_to_bool) {
    EXPECT_FALSE(ScatterInfo::miss());
    EXPECT_TRUE(ScatterInfo{});
}

struct mock_material : material_I {
    static color mock_color() { return {1.0, 0.1, 0.01}; }
    static ray   mock_ray() { return ray{point3{1.0, 2.0, 3.0}, vec3{3.0, 2.0, 1.0}}; }

    ScatterInfo scatter(ray const & ray_in, hit_record const & hit_rec) const override {
        return {mock_color(), mock_ray()};
    }
};

TEST(material, has_scatter_returning_ScatterInfo) {
    std::unique_ptr<material_I> material_ptr = std::make_unique<mock_material>();
    auto scatter_info = material_ptr->scatter(ray{point3{0.0, 0.0, 0.0}, vec3{1.0, 1.0, 1.0}}, hit_record{});
    EXPECT_THAT(scatter_info.attenuation,   Eq(mock_material::mock_color()));
    EXPECT_THAT(scatter_info.scattered_ray, Eq(mock_material::mock_ray()));
}

//---------------------------------------------------------------------materials
struct a_lambertian_material : Test {
    color const albedo{0.9, 0.7, 0.0};
    lambertian const material{albedo};

    hit_record const a_hit_record{point3{2.0}, vec3{0.0, 0.0, 1.0}, nullptr, 0.0, FaceSide::front};
    ray const a_ray{point3{0.0}, vec3{1.0}};
};

TEST_F(a_lambertian_material, scatters_starting_at_the_hit_point) {
    auto scatter_info = material.scatter(a_ray, a_hit_record);
    EXPECT_THAT(dot(scatter_info.scattered_ray.o, a_hit_record.p), Ge(0.0));
}

TEST_F(a_lambertian_material, scatters_towards_the_normal) {
    auto scatter_info = material.scatter(a_ray, a_hit_record);
    EXPECT_THAT(dot(scatter_info.scattered_ray.d, a_hit_record.normal), Ge(0.0));
}

TEST_F(a_lambertian_material, has_attenuation_according_to_albedo) {
    auto scatter_info = material.scatter(a_ray, a_hit_record);
    EXPECT_THAT(scatter_info.attenuation, Eq(albedo));
}

struct a_metal_material : Test {
    color const  albedo{0.8, 0.85, 0.9};
    metal const material{albedo};

    hit_record const a_hit_record{point3{2.0}, vec3{0.0, 0.0, 1.0}, nullptr, 0.0, FaceSide::front};
    ray const a_ray{point3{0.0}, vec3{1.0}};
};

TEST_F(a_metal_material, scatters_via_reflection) {
    auto scatter_info = material.scatter(a_ray, a_hit_record);
    EXPECT_THAT(scatter_info.scattered_ray.d, Eq(unit_vector(reflect(a_ray.d, a_hit_record.normal))));
}

struct a_fuzzy_metal_material : Test {
    color const  albedo{0.8, 0.85, 0.9};
    double const fuzz{0.1};
    metal const material{albedo, fuzz};

    hit_record const a_hit_record{point3{2.0}, vec3{0.0, 0.0, 1.0}, nullptr, 0.0, FaceSide::front};
    ray const a_ray{point3{0.0}, vec3{1.0}};
};

TEST_F(a_fuzzy_metal_material, scatters_fuzzy) {
    auto const scatter_info = material.scatter(a_ray, a_hit_record);

    auto const perfect_reflection = unit_vector(reflect(a_ray.d, a_hit_record.normal));
    auto const deviation = scatter_info.scattered_ray.d - perfect_reflection;
    EXPECT_THAT(deviation.length(), Gt(0.0));
    EXPECT_THAT(deviation.length(), Lt(fuzz));
}

struct a_dielectric_material : Test {
    color const  albedo{0.8, 0.85, 0.9};
    double const etaI{1.0}; // hard coded in dielectric
    double const etaT{1.3}; // index of refraction
    dielectric const material{etaT};

    hit_record const a_hit_record{point3{2.0}, vec3{0.0, 0.0, 1.0}, nullptr, 0.0, FaceSide::front};
    ray const a_ray{point3{0.0}, unit_vector(vec3{0.0, 1.5, -1.0})};
};

TEST_F(a_dielectric_material, refracts_ray) {
    auto const scatter_info = material.scatter(a_ray, a_hit_record);

    double const sin_ThetaI = a_ray.d.y;
    auto const & refracted_ray = scatter_info.scattered_ray;
    double const sin_ThetaT = refracted_ray.d.y;
    EXPECT_THAT(etaI * sin_ThetaI, Eq(etaT * sin_ThetaT));
}

double to_radians(double degree) { return degree / 180.0 * pi; }

#include <iostream>

TEST_F(a_dielectric_material, has_total_internal_reflection_when_necessary) {
    // To test for  total internal reflection, we need a eta ratio > 1.0, which implies
    // that the eta in the nominator ('inside') has to be larger then the eta in the
    // demonimator ('outside').
    // Most materials have an eta larger then air, so total reflection happens usually
    // inside the material. Hence, total i n t e r n a l reflection.
    double const sin_ThetaCritical = etaI / etaT; // we hit from the backside
    double const delta_theta = 0.01;

    // refraction when smaller then critical angle
    {
        double const sin_theta_refracted = sin_ThetaCritical - delta_theta;
        double const cos_theta_refracted = std::sqrt(1 - sin_theta_refracted * sin_theta_refracted);
        ray const ray_to_be_refracted{point3{0.0}, vec3{0.0, sin_theta_refracted, cos_theta_refracted}};

        auto const scatter_info = material.scatter(ray_to_be_refracted, a_hit_record);

        double const sin_ThetaI = ray_to_be_refracted.d.y;
        auto const &refracted_ray = scatter_info.scattered_ray;
        double const sin_ThetaT = refracted_ray.d.y;
        EXPECT_THAT(etaI * sin_ThetaI, DoubleEq(etaT * sin_ThetaT));
    }

    // reflection when greater then critical angle
    {
        double const sin_theta_refracted = sin_ThetaCritical + delta_theta;
        double const cos_theta_refracted = std::sqrt(1 - sin_theta_refracted * sin_theta_refracted);
        ray const ray_to_be_reflected{point3{0.0}, vec3{0.0, sin_theta_refracted, cos_theta_refracted}};

        hit_record const a_hit_record{point3{2.0}, vec3{0.0, 0.0, 1.0}, nullptr, 0.0, FaceSide::back};
        auto const scatter_info = material.scatter(ray_to_be_reflected, a_hit_record);

        auto const &reflected_ray = scatter_info.scattered_ray;
        EXPECT_THAT(reflected_ray.d.y, DoubleEq(ray_to_be_reflected.d.y));
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

