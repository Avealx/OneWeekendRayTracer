#include <color.hpp>
#include <common.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>
#include <ray.hpp>
#include <sphere.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>


color ray_color(ray const & r, hittable_I const & world) {
    hit_record rec = world.hit(r, 0, infinity);
    if (rec)
        return 0.5 * (rec.normal + color{1.0, 1.0, 1.0});

    vec3 const unit_direction = unit_vector(r.d);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
}

int main() {
    // Image
    auto const aspect_ratio = 16.0 / 19.0;
    int const image_width = 600;
    int const image_height = static_cast<int>(aspect_ratio * image_width);

    // World
    hittable_list world;
    world.add(std::make_shared<Sphere>(point3{0.0, 0.0, -1.0}, 0.5));
    world.add(std::make_shared<Sphere>(point3{0.0, -100.5, -1.0}, 100.0));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height / aspect_ratio;
    auto focal_length = 1.0;

    auto origin = point3{0, 0, 0};
    auto horizontal = vec3{viewport_width, 0, 0};
    auto vertical = vec3{0, viewport_height, 0};
    auto lower_left_corner = origin - 0.5*horizontal - 0.5*vertical - vec3{0, 0, focal_length};

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' <<  std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto h = static_cast<double>(i) / (image_width  - 1);
            auto v = static_cast<double>(j) / (image_height - 1);
            ray const r{origin, lower_left_corner + h*horizontal + v*vertical - origin};
            color const pixel_color = ray_color(r, world);

            bool const pretty = false;
            write_color(std::cout, pixel_color, pretty);
            std::cout << " ";
        }
    }
    std:: cerr << "\nDone.\n";
}