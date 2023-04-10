#include <ray.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(ray, can_be_constructed) {
    ray r{};
    EXPECT_TRUE(true);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
