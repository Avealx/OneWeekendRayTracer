#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

#include <array>

class vec3 {
public:
    using       iterator = double       *;
    using const_iterator = double const *;

    double x, y, z;

    vec3() = default;
    explicit constexpr vec3(double d): x{d}, y{d}, z{d} {}
    constexpr vec3(double x, double y, double z): x{x}, y{y}, z{z} {}

    double& operator[](std::size_t index)       { return *(&x + index); }
    double  operator[](std::size_t index) const { return *(&x + index); }

          iterator  begin()       { return &x; }
    const_iterator  begin() const { return &x; }
    const_iterator cbegin() const { return &x; }

          iterator  end()       { return &x + 3; }
    const_iterator  end() const { return &x + 3; }
    const_iterator cend() const { return &x + 3; }

    vec3  operator-() const { return vec3{-x, -y, -z}; }

    vec3& operator+=(vec3 const & v) {
        for (std::size_t i = 0; i < 3; ++i)
            (*this)[i] += v[i];
        return *this;
    }

    vec3& operator*=(double const t) {
        for (auto & c : *this)
            c *= t;
        return *this;
    }

    vec3& operator/=(double d) { return *this *= 1.0 / d; }

    double length()         const { return std::sqrt(x*x + y*y + z*z); }
    double length_squared() const { return           x*x + y*y + z*z ; }
};

bool operator==(vec3 u, vec3 v) { return u[0] == v[0] &&
                                         u[1] == v[1] &&
                                         u[2] == v[2]; }

bool operator!=(vec3 u, vec3 v) { return !(u == v); }

inline vec3 operator+(vec3 const & u, vec3 const & v) { return vec3{u.x + v.x, u.y + v.y, u.z + v.z}; }
inline vec3 operator-(vec3 const & u, vec3 const & v) { return vec3{u.x - v.x, u.y - v.y, u.z - v.z}; }

inline vec3 operator*(vec3 const & u, vec3 const & v) { return vec3{u.x * v.x, u.y * v.y, u.z * v.z}; }

inline vec3 operator*(double const s, vec3 const & u) { return vec3{s*u.x, s*u.y, s*u.z}; }
inline vec3 operator*(vec3 const & u, double const s) { return s * u; }

inline vec3 operator/(vec3 const & u, double const s) { return 1.0 / s * u ;}

inline vec3 unit_vector(vec3 const & v) { return v / v.length(); }

inline double dot(vec3 const & u, vec3 const & v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline vec3 cross(vec3 const & u, vec3 const & v) {
   return {u.y * v.z - u.z * v.y,
           u.z * v.x - u.x * v.z,
           u.x * v.y - u.y * v.x};
}

inline std::ostream& operator<<(std::ostream& out, vec3 const & v) { return out << '(' << v.x << ", "
                                                                                << v.y << ", "
                                                                                << v.z << ')'; }

using point3 = vec3;