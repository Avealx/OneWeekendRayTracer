 #include <texture.hpp>


 #include <gtest/gtest.h>
 #include <gmock/gmock.h>


 using testing::Eq;


 TEST(SolidColor, can_be_default_constructed) {
     SolidColor{};
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
