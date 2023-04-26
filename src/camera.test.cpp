#include <camera.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;


TEST(camera, can_be_created) {
    camera{};
}

TEST(camera, ray_to_center_of_image_is_correct) {
    EXPECT_THAT(camera{}.get_ray(0.5, 0.5), Eq(ray{{0.0,0.0,0.0}, {0.0, 0.0, -1.0}}));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

