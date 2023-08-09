#include <texture.hpp>


#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

using testing::DoubleEq;
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

struct AnImageTexture : Test {
    ImageTexture const texture{std::string{DATA_PATH "/test_image_RGB8_10x10_R33_G64_B128_.png"}};
};

// TODO: make proper matcher
void expect_double_equal(color const & u, color const & v) {
    EXPECT_THAT(u.x, DoubleEq(v.x));
    EXPECT_THAT(u.y, DoubleEq(v.y));
    EXPECT_THAT(u.z, DoubleEq(v.z));
}

TEST_F(AnImageTexture, retrieves_the_correct_color) {
    color const expected_color{33.0 / 255.0, 64.0 / 255.0, 128.0 / 255.0};
    expect_double_equal(texture.value(TextureCoordinates2d{0.5, 0.5}, point3{}),
                        expected_color);
}

int main(int argc, char **argv)
{
 ::testing::InitGoogleTest(&argc, argv);
 return RUN_ALL_TESTS();
}
