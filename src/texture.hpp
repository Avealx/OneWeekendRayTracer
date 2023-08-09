#pragma once

#include <color.hpp>
#include <crtp_functionality.hpp>
#include <perlin.hpp>
#include <vec3.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
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
        // return color(1.0, 1.0, 1.0) * (1.0 + noise(p)) * 0.5;
        // return color(1.0, 1.0, 1.0) * (1.0 + noise(scale * p)) * 0.5;
        // return color(1.0, 1.0, 1.0) * noise.turbulence(scale * p);
        // return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + std::sin(scale * p.z + 10.0 * std::abs(noise(p))));
        return color(1.0, 1.0, 1.0) * 0.5 * (1.0 + std::sin(scale * p.z + 10.0 * noise.turbulence(p)));
    }

    Perlin noise;
    double const scale;
};


class ImageTexture : public TextureI {
public:
    explicit ImageTexture(std::string const & file_name) {
        int components_per_pixel = bytes_per_pixel_;

        data_ = stbi_load(file_name.data(), &width_, &height_, &components_per_pixel, components_per_pixel);
        if (!data_)
            throw std::runtime_error("Could not load texture image: " + file_name + "\n");

        bytes_per_scanline_ = bytes_per_pixel_ * width_;
    }

    ~ImageTexture() {
        delete data_;
    }

    // TextureI
    color value(TextureCoordinates2d const uv, point3 const & p) const override {
        int const i = std::clamp(uv.u, 0.0, 1.0) * (width_ - 1);
        int const j = flip(std::clamp(uv.v, 0.0, 1.0)) * (height_ - 1);

        auto pixel_it = data_ + j * bytes_per_scanline_ + i * bytes_per_pixel_;
        return color_scale * color(pixel_it[0], pixel_it[1], pixel_it[2]);
    }

private:
    double flip(double const d) const { return 1.0 - d; }

    inline static constexpr int bytes_per_pixel_{3};
    inline static constexpr double color_scale{1.0 / 255.0};

    unsigned char * data_{nullptr};
    int width_, height_;
    int bytes_per_scanline_;
};