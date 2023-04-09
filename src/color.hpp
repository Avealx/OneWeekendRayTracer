#pragma once

#include <vec3.hpp>

#include <iostream>


using color  = vec3;

// write translated color to stream (translated from [0,1] to [0, 255])
std::ostream& write_color(std::ostream& out, color const pixel_color, bool pretty = true) {

    return out << (pretty ? "(" : "")
               << static_cast<int>(255.999 * pixel_color.x)
               << (pretty ? ", " : " ")
               << static_cast<int>(255.999 * pixel_color.y)
               << (pretty ? ", " : " ")
               << static_cast<int>(255.999 * pixel_color.z)
               << (pretty ? ")" : "");
}