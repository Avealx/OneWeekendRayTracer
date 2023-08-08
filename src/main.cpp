#include <bvh.hpp>
#include <camera.hpp>
#include <color.hpp>
#include <common.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>
#include <moving_sphere.hpp>
#include <perlin.hpp>
#include <ray.hpp>
#include <sphere.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>


auto const aspect_ratio = 3.0 / 2.0;


hittable_list random_scene() {
    hittable_list world;

    // auto ground_material = std::make_shared<lambertian>(color{0.5, 0.5, 0.5, });
    auto ground_texture = std::make_shared<CheckerTexture>(color{0.0, 0.25, 0.25}, color{0.15, 0.05, 0.05});
    auto ground_material = std::make_shared<lambertian>(ground_texture);
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

hittable_list two_spheres() {
    hittable_list world;

    auto checker_texture = std::make_shared<CheckerTexture>(color{0.0, 0.25, 0.25}, color{0.9, 0.9, 0.9});
    world.add(std::make_shared<Sphere>(point3(0.0, -10.0, 0.0), 10, std::make_shared<lambertian>(checker_texture)));
    world.add(std::make_shared<Sphere>(point3(0.0,  10.0, 0.0), 10, std::make_shared<lambertian>(checker_texture)));

    return world;
}

hittable_list two_perlin_spheres() {
    hittable_list world;

    auto perlin_texture = std::make_shared<NoiseTexture>(4.0 /*frequency scale*/);
    world.add(std::make_shared<Sphere>(point3{0.0, -1000.0, 0.0}, 1000.0, std::make_shared<lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(point3{0.0, 2.0, 0.0}, 2.0, std::make_shared<lambertian>(perlin_texture)));

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

enum class SceneID {
    random_spheres,
    two_spheres,
    two_perlin_spheres,
};

struct Scene {
    hittable_list world;
    Camera camera;
};

Scene select_scene(SceneID const id)
{
    point3 const lookfrom{13.0, 2.0, 3.0};
    point3 const lookat{0.0, 0.0, 0.0};
    vec3 const vertical_up{0.0, 1.0, 0.0};
    auto const vertical_fov_degree = FieldOfView{20.0};
    auto const focus_distance = FocusDistance{10.0};
    double const time0 = 0.0;
    double const time1 = 1.0;

    Aperture aperture{0.0};
    hittable_list world;

    switch (id) {
    case SceneID::random_spheres:
        aperture = Aperture{0.1};
        world = random_scene();
        break;
    case SceneID::two_spheres:
        world = two_spheres();
        break;
    case SceneID::two_perlin_spheres:
        world = two_perlin_spheres();
        break;
    }

    return {world,
            Camera{lookfrom,
                   lookat,
                   vertical_up,
                   vertical_fov_degree,
                   AspectRatio{aspect_ratio},
                   aperture,
                   focus_distance,
                   time0,
                   time1}};
}

int main() {
    // Image
    int const image_width = 600;
    int const image_height = static_cast<int>(image_width / aspect_ratio);
    int const samples_per_pixel = 50;
    int const max_depth = 5;

    // World and camera
    auto const scene = select_scene(SceneID::two_perlin_spheres);
    // auto const scene = select_scene(SceneID::two_spheres);
    auto const world = BvhNode(scene.world, TimeInterval{0.0, 1.0});
    auto const camera = scene.camera;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    if (true) {
        for (int j = image_height - 1; j >= 0; --j)
        {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color{0.0, 0.0, 0.0};
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto w = (i + random_double()) / (image_width - 1);
                    auto h = (j + random_double()) / (image_height - 1);
                    Ray r = camera.get_ray(w, h);
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
    }
    else {
        Perlin noise{};
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::lowest();
        std::vector<double> values;

        for (int j = image_height - 1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' <<  std::flush;
            for (int i = 0; i < image_width; ++i) {
                double const value = noise(vec3{static_cast<double>(i) * 10 / image_width,
                                                static_cast<double>(j) * 10 / image_height,
                                                1.0});
                values.push_back(value);
                min = std::min(min, value);
                max = std::max(max, value);
            }
        }

        for (auto v : values) {
            color const pixel_color{(v - min) / (max - min)};
            write_color(std::cout,
                        pixel_color,
                        1,
                        ns_color::WritePretty{false},
                        ns_color::GammaCorrection{true});
            std::cout << " ";
        }

    }


    std:: cerr << "\nDone.\n";
}
