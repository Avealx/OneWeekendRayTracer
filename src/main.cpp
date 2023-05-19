#include <camera.hpp>
#include <color.hpp>
#include <common.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>
#include <ray.hpp>
#include <sphere.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>


color ray_color(ray const & r, hittable_I const & world, int depth) {
    if (depth <= 0)
        return color{0.0, 0.0, 0.0};

    hit_record rec = world.hit(r, 1e-3, infinity);
    if (rec) {
        auto const scatter_info = rec.material_ptr->scatter(r, rec);
        if (scatter_info)
            return scatter_info.attenuation * ray_color(scatter_info.scattered_ray,
                                                        world,
                                                        depth - 1);
        return color(0.0, 0.0, 0.0);
    }

    // background
    vec3 const unit_direction = unit_vector(r.d);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
}

int main() {
    // Image
    auto const aspect_ratio = 16.0 / 9.0;
    int const image_width = 600;
    int const image_height = static_cast<int>(image_width / aspect_ratio);
    int const samples_per_pixel = 100;
    int const max_depth = 50;

    // World
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color{0.4, 0.5, 0.6});
    auto material_center = std::make_shared<lambertian>(color{0.9, 0.9, 0.9});
    auto material_left =   std::make_shared<metal>(color{0.8, 0.8, 0.75});
    auto material_right =   std::make_shared<metal>(color{0.8, 0.6, 0.2}, 0.3);

    world.add(std::make_shared<Sphere>(point3{0.0, -100.5, -1.0}, 100.0, material_ground));
    world.add(std::make_shared<Sphere>(point3{ 0.0, 0.0, -1.0}, 0.5, material_center));
    world.add(std::make_shared<Sphere>(point3{-1.0, 0.0, -1.0}, 0.5, material_left));
    world.add(std::make_shared<Sphere>(point3{ 1.0, 0.0, -1.0}, 0.5, material_right));

    // Camera
    camera const cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' <<  std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto w = (i + random_double()) / (image_width - 1);
                auto h = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(w, h);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout,
                        pixel_color,
                        samples_per_pixel,
                        ns_color::WritePretty{false},
                        ns_color::GammaCorrection{true});
            std::cout << " ";
        }
    }
    std:: cerr << "\nDone.\n";
}
