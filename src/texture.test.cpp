#include <texture.hpp>


#include <gtest/gtest.h>
#include <gmock/gmock.h>


using testing::Eq;
using testing::Ne;
using testing::Test;


struct ATextureCoordinate : Test {
 TextureCoordinates2d const tc = {0.2, 0.1};
};

TEST_F(ATextureCoordinate, can_be_compared_for_equality) {
 auto const same{tc};
 EXPECT_THAT(tc, Eq(same));
}

TEST_F(ATextureCoordinate, is_not_unnecessarily_bloated) {
 EXPECT_THAT(sizeof(tc), Eq(2 * sizeof(double)));
}

TEST_F(ATextureCoordinate, can_be_compared_for_inequality) {
 TextureCoordinates2d const different{};
 EXPECT_THAT(tc, Ne(different));
}

TEST(SolidColor, returns_correct_value) {
 color const value{0.0, 0.4, 0.2};
 EXPECT_THAT(SolidColor{value}.value({}, {}), Eq(value));
}


int main(int argc, char **argv)
{
 ::testing::InitGoogleTest(&argc, argv);
 return RUN_ALL_TESTS();
}
