#include <common.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::DoubleEq;

TEST(common, degree_to_radians) {
    EXPECT_THAT(degree_to_radians(180), DoubleEq(pi));
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


