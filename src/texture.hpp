#pragma once

#include <color.hpp>
#include <crtp_functionality.hpp>
#include <perlin.hpp>
#include <vec3.hpp>

#include <memory>

struct TextureCoordinates2d : inject_pointer_as_iterator<double>
                            , inject_trivial_begin_end<TextureCoordinates2d>
                            , inject_container_equal<TextureCoordinates2d> {
    double u{-1.0}, v{-1.0};

    TextureCoordinates2d() = default;
    TextureCoordinates2d(double const u, double const v) : u{u}, v{v} {}
};

struct TextureI {
    virtual color value(TextureCoordinates2d const uv, point3 const & p) const = 0;
};


class SolidColor : public TextureI {
public:
    SolidColor() = default;
    SolidColor(color const c) : value_{c} {}

    // TextureI
    color value(TextureCoordinates2d const uv, point3 const & p) const override {
        return value_;
    }

private:
    color value_;
};

struct CheckerTexture : TextureI {
    CheckerTexture() = default;
    CheckerTexture(std::shared_ptr<TextureI> even, std::shared_ptr<TextureI> odd) : even{even}, odd{odd} {}
    CheckerTexture(color const color0, color const color1) : even{std::make_shared<SolidColor>(color0)}
                                                           , odd{std::make_shared<SolidColor>(color1)} {}

    // TextureI
    color value(TextureCoordinates2d const uv, point3 const & p) const override {
        auto sines = std::sin(10.0 * p.x) * std::sin(10.0 * p.y) * std::sin(10.0 * p.z);
        if (sines < 0)
            return odd->value(uv, p);
        return even->value(uv, p);
    }

    std::shared_ptr<TextureI> even;
    std::shared_ptr<TextureI> odd;
};

struct NoiseTexture : TextureI {

    NoiseTexture(double const scale = 1.0) : scale{scale} {}

    // TextureI
    color value(TextureCoordinates2d const uv, point3 const & p) const override {
        // return color(1.0, 1.0, 1.0) * std::abs(noise(scale * p));
        return color(1.0, 1.0, 1.0) * (1.0 + noise(scale * p)) * 0.5;
    }

    Perlin noise;
    double const scale;
};