#include <iostream>
#include <gtest/gtest.h>

using namespace std;
using namespace testing;

TEST(FooTest, some_test)
{
    EXPECT_TRUE(true);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
