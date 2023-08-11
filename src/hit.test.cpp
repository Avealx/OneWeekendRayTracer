#include <hit.hpp>

#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>
#include <sstream>


using ::testing::Eq;
using ::testing::Ne;


TEST(HitRecord, has_aggregate_constructor) { HitRecord{point3{}, vec3{}, nullptr, 0.0}; }

TEST(HitRecord, can_be_printed) {
    std::stringstream ss{};
    ss << HitRecord{point3{1.0, 2.0, 3.0}, vec3{1.0, 2.0, 3.0}, nullptr, 5.0, {} /*tex crd*/, FaceSide::front};
    EXPECT_THAT(ss.str(), Eq("(p=(1, 2, 3), n=(1, 2, 3), t=5, front)"));
}

TEST(HitRecord, has_can_be_cast_to_bool_true) {
    auto const hr = HitRecord{point3{}, vec3{}, nullptr, 0.0};
    EXPECT_TRUE(hr);
}

TEST(HitRecord, has_can_be_cast_to_bool_false) {
    EXPECT_FALSE(HitRecord::miss());
}

TEST(HitRecord, equality_true_miss_mis) {
    auto miss = HitRecord::miss();
    EXPECT_THAT(HitRecord::miss(), Eq(HitRecord::miss()));
    miss.t = 10;
    EXPECT_THAT(HitRecord::miss(), Eq(HitRecord::miss()));
}

TEST(HitRecord, equality_false_hit_miss) {
    HitRecord const hr = HitRecord{point3{0.0}, vec3{0.0}, nullptr, 0.0};
    EXPECT_THAT(hr, Ne(HitRecord::miss()));
}

TEST(HitRecord, equality_true_hit_hit) {
    HitRecord const hr = HitRecord{point3{0.0}, vec3{0.0}, nullptr, 0.0};
    EXPECT_THAT(hr, Eq(hr));
}

TEST(HitRecord, equality_false_hit_hit) {
    HitRecord const hr = HitRecord{point3{0.0}, vec3{0.0}, nullptr, 0.0};
    HitRecord const hr1 = HitRecord{point3{1.0}, vec3{0.0}, nullptr, 0.0};
    HitRecord const hr2 = HitRecord{point3{0.0}, vec3{1.0}, nullptr, 0.0};
    HitRecord const hr3 = HitRecord{point3{0.0}, vec3{0.0}, nullptr, 1.0};

    EXPECT_THAT(hr, Ne(hr1));
    EXPECT_THAT(hr, Ne(hr2));
    EXPECT_THAT(hr, Ne(hr3));
}

TEST(HitRecord, has_correct_side_when_ray_is_outside) {
    HitRecord hr = HitRecord{point3{0.0}, vec3{0.0, 0.0, -1.0}, nullptr, 0.0, {} /*tex crd*/, FaceSide::front};
    vec3 const outward_normal = vec3{0.0, 0.0, 1.0};
    hr.set_face_normal(Ray{point3{0.0, 0.0, 1.0}, outward_normal}, outward_normal);
    EXPECT_THAT(hr.normal, Eq(-outward_normal));
}

TEST(HitRecord, has_correct_side_when_ray_is_inside) {
    HitRecord hr = HitRecord{point3{0.0}, vec3{0.0, 0.0, -1.0}, nullptr, 0.0, {} /*tex crd*/, FaceSide::front};
    vec3 const outward_normal = vec3{0.0, 0.0, 1.0};
    hr.set_face_normal(Ray{point3{0.0, 0.0, 1.0}, -outward_normal}, outward_normal);
    EXPECT_THAT(hr.normal, Eq(outward_normal));
}


//TEST(HitRecord, has_material) {
//    HitRecord{}.material;
//}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
