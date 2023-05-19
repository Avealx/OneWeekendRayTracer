#include <material.hpp>

#include <color.hpp>
#include <ray.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <typeinfo>


using ::testing::Eq;
using ::testing::Ge;
using ::testing::Test;


TEST(scatter_info, has_attenuation) {
    EXPECT_THAT(typeid(ScatterInfo{}.attenuation).name(), Eq(typeid(color).name()));
}

TEST(scatter_info, has_scattered_ray) {
    EXPECT_THAT(typeid(ScatterInfo{}.scattered_ray).name(), Eq(typeid(ray).name()));
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

