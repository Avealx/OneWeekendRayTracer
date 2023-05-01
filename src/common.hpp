#pragma once

#include <limits>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

template<class Tag>
class TypedBool {
public:
    constexpr explicit TypedBool(bool value) : value_{value} {}

    constexpr explicit operator bool() const {
        return value_;
    }

    constexpr TypedBool& operator=(TypedBool const other) {
        value_ = other.value_;
        return *this;
    }

    constexpr TypedBool operator!() {
        return TypedBool{!value_};
    }

private:
    bool value_;
};

template <class Tag>
inline bool operator==(TypedBool<Tag> const lhs, TypedBool<Tag> const rhs) {
    return bool{lhs} == bool{rhs};
}

template <class Tag>
inline bool operator!=(TypedBool<Tag> const lhs, TypedBool<Tag> const rhs) {
    return !(lhs == rhs);
}

inline double degree_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}