#pragma once

#include <color.hpp>
#include <crtp_functionality.hpp>
#include <vec3.hpp>


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