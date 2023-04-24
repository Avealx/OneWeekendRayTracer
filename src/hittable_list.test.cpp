#include <hittable_list.hpp>

#include <hit.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


struct mock_hittable : hittable_I {
    mock_hittable(double distance = 0.0) : distance{distance} {}

    hit_record hit(ray const & r, double t_min, double t_max) const override {
        return distance < t_max ? hit_record{point3{0.0}, vec3{0.0}, distance, FaceSide::front}
                                : hit_record::miss();
    }

    double distance;
};


TEST(hittable_list, can_be_default_constructed) {
    hittable_list{};
}

TEST(hittable_list, can_be_constructed_with_element) {
    EXPECT_THAT(hittable_list{std::make_shared<mock_hittable>()}.objects.size(), Eq(1)) ;
}

TEST(hittable_list, allows_adding_objects) {
    hittable_list hl{};
    auto size_before_add = hl.objects.size();


    hl.add(std::make_shared<mock_hittable>());
    auto size_after_add = hl.objects.size();

    EXPECT_THAT(size_before_add, Eq(0));
    EXPECT_THAT(size_after_add, Eq(1));
}

TEST(hittable_list, allows_clearing) {
    hittable_list hl{std::make_shared<mock_hittable>()};
    auto size_before_clear = hl.objects.size();

    hl.clear();
    auto size_after_clear = hl.objects.size();

    EXPECT_THAT(size_before_clear, Eq(1));
    EXPECT_THAT(size_after_clear, Eq(0));
}

TEST(hittable_list, returns_closest_hit) {
    hittable_list hl{std::make_shared<mock_hittable>(10.0)};
    hl.add(std::make_shared<mock_hittable>(5.0));
    hl.add(std::make_shared<mock_hittable>(7.5));

    double t_min = 0.0, t_max = 20.0;
    auto record = hl.hit(ray{point3{0.0}, vec3{0.0}}, t_min, t_max);

    EXPECT_THAT(record.t, Eq(5.0));
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

