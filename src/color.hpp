#pragma once

#include <common.hpp>
#include <vec3.hpp>

#include <iostream>


using color  = vec3;

namespace ns_color {
    struct WritePrettyTag{};
    using WritePretty = TypedBool<WritePrettyTag>;

    struct GammaCorrectionTag{};
    using GammaCorrection = TypedBool<GammaCorrectionTag>;
}

// write translated color to stream (translated from [0,1] to [0, 255])
std::ostream& write_color(std::ostream& out,
                          color const& pixel_color,
                          int const samples_per_pixel = 1,
                          ns_color::WritePretty const pretty = ns_color::WritePretty{true},
                          ns_color::GammaCorrection const gamma_correction = ns_color::GammaCorrection{false}) {
    auto const final_color = pow(pixel_color * 1.0 / samples_per_pixel, gamma_correction ? 0.5 : 1.0);
    return out << (pretty ? "(" : "")
               << static_cast<int>(255.999 * final_color.x)
               << (pretty ? ", " : " ")
               << static_cast<int>(255.999 * final_color.y)
               << (pretty ? ", " : " ")
               << static_cast<int>(255.999 * final_color.z)
               << (pretty ? ")" : "");
}