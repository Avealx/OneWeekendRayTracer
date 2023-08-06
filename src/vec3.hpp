#pragma once

#include <common.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>


template <class Base>
struct inject_trivial_begin_end {
    auto  begin()       { return reinterpret_cast<typename Base::      iterator>(this); }
    auto  begin() const { return reinterpret_cast<typename Base::const_iterator>(this); }
    auto cbegin() const { return reinterpret_cast<typename Base::const_iterator>(this); }

    auto  end()       { return reinterpret_cast<typename Base::      iterator>(static_cast<Base       *>(this) + 1); }
    auto  end() const { return reinterpret_cast<typename Base::const_iterator>(static_cast<Base const *>(this) + 1); }
    auto cend() const { return reinterpret_cast<typename Base::const_iterator>(static_cast<Base const *>(this) + 1); }
};


// requires Base implementing concept `iterable` (begin())
template<class Base>
struct inject_trivial_indexing {
    auto& operator[](std::size_t index)       { return *(static_cast<Base       *>(this)->begin() + index); }
    auto  operator[](std::size_t index) const { return *(static_cast<Base const *>(this)->begin() + index); }
};


struct vec3 : inject_trivial_begin_end<vec3>
            , inject_trivial_indexing<vec3> {
    // types
    using       iterator = double       *;
    using const_iterator = double const *;

    // static members
    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    // data members
    double x, y, z;

    // function member
    vec3() = default;
    explicit constexpr vec3(double d): x{d}, y{d}, z{d} {}
    constexpr vec3(double x, double y, double z): x{x}, y{y}, z{z} {}

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

    bool near_zero() const {
        double constexpr eps = 1e-8;
        return std::fabs(x) < eps && std::fabs(y) < eps && std::fabs(z) < eps;
    }
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

// TODO: how to make this a static member while still using the dot function?
inline vec3 random_in_unit_disk() {
    while (true)
        if (vec3 const v{random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0};
            v.length_squared() < 1.0)
            return v;
}

inline vec3 random_in_unit_sphere() {
    auto result = vec3::random(-1.0, 1.0);
    while (dot(result, result) > 1.0)
        result = vec3::random(-1.0, 1.0);
    return result;
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(vec3 const & normal) {
    vec3 const in_unit_sphere = random_in_unit_sphere();
    return dot(normal, in_unit_sphere) > 0.0 ?  in_unit_sphere
                                             : -in_unit_sphere;
}

inline vec3 reflect(vec3 const & v, vec3 const & normal) {
    return v - 2 * dot(v, normal) * normal;
}

inline vec3 refract(vec3 const & in, vec3 const & normal, double const etaI_over_etaT) {
    // see https://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
    auto const cos_thetaI = std::fmin(dot(-in, normal), 1.0);
    vec3 const t_parallel = etaI_over_etaT * (in + cos_thetaI * normal);
    vec3 const t_perp = -sqrt(std::fabs(1.0 - t_parallel.length_squared())) * normal;
    return t_parallel + t_perp;
}

inline vec3 cross(vec3 const & u, vec3 const & v) {
   return {u.y * v.z - u.z * v.y,
           u.z * v.x - u.x * v.z,
           u.x * v.y - u.y * v.x};
}

inline vec3 pow(vec3 const v, double exponent) {
    return {std::pow(v.x, exponent), std::pow(v.y, exponent), std::pow(v.z, exponent)};
}

inline std::ostream& operator<<(std::ostream& out, vec3 const & v) { return out << '(' << v.x << ", "
                                                                                << v.y << ", "
                                                                                << v.z << ')'; }

inline vec3 min_components(vec3 const u, vec3 const v) {
    return vec3{std::min(u.x, v.x),
                std::min(u.y, v.y),
                std::min(u.z, v.z)};
}

inline vec3 max_components(vec3 const u, vec3 const v) {
    return vec3{std::max(u.x, v.x),
                std::max(u.y, v.y),
                std::max(u.z, v.z)};
}

using point3 = vec3;
