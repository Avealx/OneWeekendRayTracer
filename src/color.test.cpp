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

    write_color(ss, c, 1, ns_color::WritePretty{false});

    EXPECT_THAT(ss.str(), Eq("0 127 255"));
}

TEST(color, can_be_written_to_stream_normalized_by_num_samples) {
    color const c{0.0, 1.0, 2.0};
    int const samples_per_pixel = 2;
    std::stringstream ss{};

    write_color(ss, c, samples_per_pixel, ns_color::WritePretty{false});

    EXPECT_THAT(ss.str(), Eq("0 127 255"));
}

TEST(color, can_be_written_to_stream_gamma_corrected) {
    color const c{0.0, 0.25, 1.0};
    std::stringstream ss{};

    write_color(ss,
                c,
                1,
                ns_color::WritePretty{false},
                ns_color::GammaCorrection{true});

    EXPECT_THAT(ss.str(), Eq("0 127 255"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}