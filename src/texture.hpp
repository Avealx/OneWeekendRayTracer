#pragma once

#include <color.hpp>
#include <vec3.hpp>


struct TextureCoordinates2d {
    double u{-1.0}, v{-1.0};
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