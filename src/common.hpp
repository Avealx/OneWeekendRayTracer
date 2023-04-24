#pragma once

#include <limits>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

double degree_to_radians(double degrees) {
    return degrees * pi / 180.0;
}