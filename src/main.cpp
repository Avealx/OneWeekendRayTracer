#include <camera.hpp>
#include <color.hpp>
#include <common.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>
#include <moving_sphere.hpp>
#include <ray.hpp>
#include <sphere.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>


hittable_list random_scene() {
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color{0.5, 0.5, 0.5, });
    world.add(std::make_shared<Sphere>(point3{0.0, -1000.0, 0.0}, 1000.0, ground_material));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto const choose_mat = random_double();
            point3 const center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3{4, 0.2, 0.0}).length() <= 0.9)
                continue;

            std::shared_ptr<material_I> sphere_material;
            if (choose_mat < 0.8) {
                // diffuse
                auto const albedo = color::random() * color::random();
                sphere_material = std::make_shared<lambertian>(albedo);
                point3 const center1 = center + vec3(0.0, random_double(0.0, 0.5), 0.0);
                double const time0 = 0.0;
                double const time1 = 1.0;
                world.add(std::make_shared<MovingSphere>(center, center1, time0, time1, 0.2, sphere_material));
            } else if (choose_mat < 0.95) {
                // metal
                auto const albedo = color::random(0.5, 1.0);
                auto const fuzz = random_double(0.0, 0.5);
                sphere_material = std::make_shared<metal>(albedo, fuzz);
                world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
            } else {
                // glass
                sphere_material = std::make_shared<dielectric>(1.5);
                world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
            }
        }
    }

    auto const material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<Sphere>(point3{0.0, 1.0, 0.0}, 1.0, material1));

    auto const material2 = std::make_shared<lambertian>(color{0.4, 0.2, 0.1});
    world.add(std::make_shared<Sphere>(point3{-4.0, 1.0, 0.0}, 1.0, material2));

    auto const material3 = std::make_shared<metal>(color{0.7, 0.6, 0.5}, 0.0);
    world.add(std::make_shared<Sphere>(point3{4.0, 1.0, 0.0}, 1.0, material3));

    return world;
}


color ray_color(Ray const & r, hittable_I const & world, int depth) {
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
    auto const aspect_ratio = 3.0 / 2.0;
    int const image_width = 1200;
    int const image_height = static_cast<int>(image_width / aspect_ratio);
    int const samples_per_pixel = 500;
    int const max_depth = 50;

    // World
    auto const world = random_scene();

    // Camera
    point3 const lookfrom{13.0, 2.0, 3.0};
    point3 const lookat{0.0, 0.0, 0.0};
    vec3 const vertical_up{0.0, 1.0, 0.0};
    auto const vertical_fov_degree = FieldOfView{20.0};
    auto const aperture = Aperture{0.1};
    auto const focus_distance = FocusDistance{10.0};
    double time0 = 0.0;
    double time1 = 1.0;
    camera const cam{lookfrom,
                     lookat,
                     vertical_up,
                     vertical_fov_degree,
                     AspectRatio{aspect_ratio},
                     aperture,
                     focus_distance,
                     time0,
                     time1};

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' <<  std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color{0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto w = (i + random_double()) / (image_width - 1);
                auto h = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(w, h);
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
