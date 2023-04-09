#include <color.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sstream>


using testing::Eq;


TEST(color, can_be_written_to_stream) {
    color const c{0.0, 0.5, 1.0};
    std::stringstream ss{};

    write_color(ss, c);

    EXPECT_THAT(ss.str(), Eq("(0, 127, 255)"));
}

TEST(color, can_be_written_to_stream_without_delimiters) {
    color const c{0.0, 0.5, 1.0};
    std::stringstream ss{};

    bool const pretty = false;
    write_color(ss, c, pretty);

    EXPECT_THAT(ss.str(), Eq("0 127 255"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}