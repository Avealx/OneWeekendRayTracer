#include <hit.hpp>

#include <vec3.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cmath>
#include <sstream>


using ::testing::Eq;
using ::testing::Ne;


TEST(hit_record, has_aggregate_constructor) { hit_record{point3{}, vec3{}, 0.0}; }

TEST(hit_record, can_be_printed) {
    std::stringstream ss{};
    ss << hit_record{point3{1.0, 2.0, 3.0}, vec3{1.0, 2.0, 3.0}, 5.0, FaceSide::front};
    EXPECT_THAT(ss.str(), Eq("(p=(1, 2, 3), n=(1, 2, 3), t=5, front)"));
}

TEST(hit_record, has_can_be_cast_to_bool_true) {
    auto const hr = hit_record{point3{}, vec3{}, 0.0};
    EXPECT_TRUE(hr);
}

TEST(hit_record, has_can_be_cast_to_bool_false) {
    EXPECT_FALSE(hit_record::miss());
}

TEST(hit_record, equality_true_miss_mis) {
    auto miss = hit_record::miss();
    EXPECT_THAT(hit_record::miss(), Eq(hit_record::miss()));
    miss.t = 10;
    EXPECT_THAT(hit_record::miss(), Eq(hit_record::miss()));
}

TEST(hit_record, equality_false_hit_miss) {
    hit_record const hr = hit_record{point3{0.0}, vec3{0.0}, 0.0};
    EXPECT_THAT(hr, Ne(hit_record::miss()));
}

TEST(hit_record, equality_true_hit_hit) {
    hit_record const hr = hit_record{point3{0.0}, vec3{0.0}, 0.0};
    EXPECT_THAT(hr, Eq(hr));
}

TEST(hit_record, equality_false_hit_hit) {
    hit_record const hr = hit_record{point3{0.0}, vec3{0.0}, 0.0};
    hit_record const hr1 = hit_record{point3{1.0}, vec3{0.0}, 0.0};
    hit_record const hr2 = hit_record{point3{0.0}, vec3{1.0}, 0.0};
    hit_record const hr3 = hit_record{point3{0.0}, vec3{0.0}, 1.0};

    EXPECT_THAT(hr, Ne(hr1));
    EXPECT_THAT(hr, Ne(hr2));
    EXPECT_THAT(hr, Ne(hr3));
}

TEST(hit_record, has_correct_side_when_ray_is_outside) {
    hit_record hr = hit_record{point3{0.0}, vec3{0.0, 0.0, -1.0}, 0.0, FaceSide::front};
    vec3 const outward_normal = vec3{0.0, 0.0, 1.0};
    hr.set_face_normal(ray{point3{0.0, 0.0, 1.0}, outward_normal}, outward_normal);
    EXPECT_THAT(hr.normal, Eq(-outward_normal));
}

TEST(hit_record, has_correct_side_when_ray_is_inside) {
    hit_record hr = hit_record{point3{0.0}, vec3{0.0, 0.0, -1.0}, 0.0, FaceSide::front};
    vec3 const outward_normal = vec3{0.0, 0.0, 1.0};
    hr.set_face_normal(ray{point3{0.0, 0.0, 1.0}, -outward_normal}, outward_normal);
    EXPECT_THAT(hr.normal, Eq(outward_normal));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
