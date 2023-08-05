#include <bvh.hpp>

#include <hittable_list.hpp>
#include <sphere.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>




using testing::Eq;
using testing::Test;


TEST(BvhNode, can_be_default_constructed) {
    BvhNode const bvh{};
}

TEST(BvhNode, can_be_constructed_from_hittable_list) {
    BvhNode const bvh{hittable_list{std::make_shared<Sphere>(vec3{}, 1.0)},
                      TimeInterval{}};
}

TEST(BvhNode, can_be_constructed_from_hittables) {
    BvhNode const bvh{std::vector<std::shared_ptr<hittable_I>>{
                          std::make_shared<Sphere>(vec3{}, 1.0),
                          std::make_shared<Sphere>(vec3{}, 1.0)},
                      TimeInterval{}};
}

struct ABvhNode : Test {
public:
    Sphere const sphere{vec3{2.0}, 1.0};
    BvhNode const bvh{hittable_list{std::make_shared<Sphere>(sphere)}, TimeInterval{}};
};

TEST_F(ABvhNode, can_be_missed) {
    Ray const ray{point3{0.0}, -vec3{sphere.c}};
    EXPECT_FALSE(bvh.hit(ray, 0.0, infinity));
}

TEST_F(ABvhNode, can_be_hit) {
    Ray const ray{point3{0.0}, vec3{sphere.c}};
    EXPECT_TRUE(bvh.hit(ray, 0.0, infinity));
}

struct ManyBvhNodes : Test {
public:
    Sphere const sphere1{vec3{1.0, 1.0, -10.0}, 0.1};
    Sphere const sphere2{vec3{2.0, -1.0, -12.0}, 0.1};
    Sphere const sphere3{vec3{3.0, 1.2, -11.0}, 0.1};
    Sphere const sphere4{vec3{4.0, -1.1, -11.0}, 0.1};
    BvhNode const bvh{hittable_list{{std::make_shared<Sphere>(sphere1),
                                     std::make_shared<Sphere>(sphere2),
                                     std::make_shared<Sphere>(sphere3),
                                     std::make_shared<Sphere>(sphere4)}},
                     TimeInterval{}};
};

TEST_F(ManyBvhNodes, can_be_hit1) {
    point3 const origin{0.0};
    Ray const ray1{origin, sphere1.c - origin};
    EXPECT_TRUE(bvh.hit(ray1, 0.0, infinity));
}

TEST_F(ManyBvhNodes, can_be_hit2) {
    point3 const origin{0.0};
    Ray const ray2{origin, sphere2.c - origin};
    EXPECT_TRUE(bvh.hit(ray2, 0.0, infinity));
}

TEST_F(ManyBvhNodes, can_be_hit3) {
    point3 const origin{0.0};
    Ray const ray3{origin, sphere3.c - origin};
    EXPECT_TRUE(bvh.hit(ray3, 0.0, infinity));
}

TEST_F(ManyBvhNodes, can_be_hit4) {
    point3 const origin{0.0};
    Ray const ray4{origin, sphere4.c - origin};
    EXPECT_TRUE(bvh.hit(ray4, 0.0, infinity));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


