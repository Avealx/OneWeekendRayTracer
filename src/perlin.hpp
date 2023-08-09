#pragma once

#include <vec3.hpp>

#include <array>

// taken from http://eastfarthing.com/blog/2015-04-21-noise/
class Perlin {
public:
    Perlin() {
        for (auto & grad : grads_)
            grad = random_unit_vector();

        for (std::size_t ii = 0; ii < size_; ++ii) {
            std::size_t jj = random_int(0, ii);
            if (jj < ii)
                perm_[ii] = perm_[jj];
            perm_[jj] = ii;
        }

    }

    double operator()(vec3 const & p) const {
        static double min = std::numeric_limits<double>::max();
        static double max = 0.0;
        double result = 0.0;
        int cell_x = std::floor(p.x);
        int cell_y = std::floor(p.y);
        int cell_z = std::floor(p.z);
        int ii = 0;
        for (int grid_z = cell_z; grid_z <= cell_z + 1; ++grid_z)
            for (int grid_y = cell_y; grid_y <= cell_y + 1; ++grid_y)
                for (int grid_x = cell_x; grid_x <= cell_x + 1; ++grid_x) {
                    auto hash = perm_[(perm_[(perm_[grid_x & mask_] + grid_y) & mask_] + grid_z) & mask_];
                    result += surflet(p - vec3{static_cast<double>(grid_x),
                                               static_cast<double>(grid_y),
                                               static_cast<double>(grid_z)},
                                      grads_[hash]);
                }
        return result;
    }

    double turbulence(vec3 const & p, double const depth = 7.0) const {
        auto accum = 0.0;
        auto q = p;
        auto weight = 1.0;
        for (std::size_t ii = 0; ii < depth; ++ii, weight *= 0.5, q *= 2.0)
            accum += weight * (*this)(q);

        return std::abs(accum);
    }

private:
    // functions
    static double interpolant(double const r) {
        auto t = std::abs(r);
        return t >= 1.0 ? 0.0 : 1.0 - (3.0 - 2.0 * t) * t * t;
    }

    static double surflet(vec3 const &p, vec3 const &grad) {
        return interpolant(p.x) * interpolant(p.y) * interpolant(p.z) * dot(p, grad);
    }

    // members
    inline static constexpr std::size_t size_{1 << 9};
    inline static constexpr std::size_t const mask_{size_ - 1};
    std::array<vec3, size_> grads_;
    std::array<std::size_t, size_> perm_;
};
