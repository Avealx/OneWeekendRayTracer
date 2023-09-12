#pragma once
#include <camera.hpp>
#include <color.hpp>
#include <vec3.hpp>

#include <hittable_list.hpp>
#include <hittables.hpp>
#include <material.hpp>
#include <moving_sphere.hpp>
#include <quad.hpp>
#include <ray.hpp>
#include <scenes.hpp>
#include <sphere.hpp>

enum class SceneID {
    random_spheres,
    two_spheres,
    two_perlin_spheres,
    planet,
    quads,
    simple_light,
    cornell_box,
    cornell_smoke,
    final_scene,
};

using BackgroundFunction = std::function<color(Ray const &)>;

struct Scene {
    HittableList world;
    BackgroundFunction background_color;
    Camera camera;
};

color default_background(Ray const & ray) {
    vec3 const unit_direction = unit_vector(ray.d);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
}

color black_background(Ray const &){
    return color{0.0};
}

HittableList random_scene() {
    HittableList world;

    auto ground_texture = std::make_shared<CheckerTexture>(color{0.0, 0.25, 0.25}, color{0.15, 0.05, 0.05});
    auto ground_material = std::make_shared<lambertian>(ground_texture);
    world.add(std::make_shared<Sphere>(point3{0.0, -1000.0, 0.0}, 1000.0, ground_material));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            auto const choose_mat = random_double();
            point3 const center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3{4, 0.2, 0.0}).length() <= 0.9)
                continue;

            std::shared_ptr<MaterialI> sphere_material;
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

HittableList two_spheres() {
    HittableList world;

    auto checker_texture = std::make_shared<CheckerTexture>(color{0.0, 0.25, 0.25}, color{0.9, 0.9, 0.9});
    world.add(std::make_shared<Sphere>(point3(0.0, -10.0, 0.0), 10, std::make_shared<lambertian>(checker_texture)));
    world.add(std::make_shared<Sphere>(point3(0.0,  10.0, 0.0), 10, std::make_shared<lambertian>(checker_texture)));

    return world;
}

HittableList two_perlin_spheres() {
    HittableList world;

    auto perlin_texture = std::make_shared<NoiseTexture>(4.0 /*frequency scale*/);
    world.add(std::make_shared<Sphere>(point3{0.0, -1000.0, 0.0}, 1000.0, std::make_shared<lambertian>(perlin_texture)));
    world.add(std::make_shared<Sphere>(point3{0.0, 2.0, 0.0}, 2.0, std::make_shared<lambertian>(perlin_texture)));

    return world;
}

HittableList planet() {
    HittableList world;

    auto earth_texture = std::make_shared<ImageTexture>(std::string{DATA_PATH "/earthmap.jpg"});
    world.add(std::make_shared<Sphere>(point3{0.0, 0.0, 0.0}, 2.0, std::make_shared<lambertian>(earth_texture)));

    return world;
}

HittableList quads() {
    HittableList world;

    auto left_red     = std::make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<lambertian>(color(0.2, 0.8, 0.8));

    world.add(std::make_shared<Quad>(point3{-3.0, -2.0, 1.0}, vec3{0.0, 0.0,  4.0}, vec3{0.0, 4.0,  0.0}, left_red));
    world.add(std::make_shared<Quad>(point3{-2.0, -2.0, 0.0}, vec3{4.0, 0.0,  0.0}, vec3{0.0, 4.0,  0.0}, back_green));
    world.add(std::make_shared<Quad>(point3{ 3.0, -2.0, 1.0}, vec3{0.0, 0.0,  4.0}, vec3{0.0, 4.0,  0.0}, right_blue));
    world.add(std::make_shared<Quad>(point3{-2.0,  3.0, 1.0}, vec3{4.0, 0.0,  0.0}, vec3{0.0, 0.0,  4.0}, upper_orange));
    world.add(std::make_shared<Quad>(point3{-2.0, -3.0, 5.0}, vec3{4.0, 0.0,  0.0}, vec3{0.0, 0.0, -4.0}, lower_teal));

    return world;
}

HittableList simple_light() {
    HittableList world = two_perlin_spheres();

    auto diffuse_light = std::make_shared<DiffuseLight>(color(4.0, 4.0, 4.0));
    world.add(std::make_shared<Sphere>(point3(0.0, 7.0, 0.0), 2.0, diffuse_light));
    world.add(std::make_shared<Quad>(point3{3.0, 1.0, -2.0}, vec3{2.0, 0.0, 0.0}, vec3{0.0, 2.0, 0.0}, diffuse_light));

    return world;
}

HittableList cornell_box_base() {
    HittableList world;

    auto red   = std::make_shared<lambertian>(color{0.65, 0.05, 0.05});
    auto white = std::make_shared<lambertian>(color{0.73, 0.73, 0.73});
    auto green = std::make_shared<lambertian>(color{0.12, 0.45, 0.15});
    auto light = std::make_shared<DiffuseLight>(color{15.0, 15.0, 15.0});

    world.add(std::make_shared<Quad>(point3(555.0,   0.0,   0.0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(std::make_shared<Quad>(point3(  0.0,   0.0,   0.0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(std::make_shared<Quad>(point3(343.0, 553.0, 332.0), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(std::make_shared<Quad>(point3(  0.0,   0.0,   0.0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(std::make_shared<Quad>(point3(  0.0, 555.0,   0.0), vec3(555,0,0), vec3(0,0,555), white));  // top
    world.add(std::make_shared<Quad>(point3(  0.0,   0.0, 555.0), vec3(555,0,0), vec3(0,555,0), white));  // back

    return world;
}

HittableList cornell_box() {
    HittableList world = cornell_box_base();

    auto white = std::make_shared<lambertian>(color{0.73, 0.73, 0.73});

    std::shared_ptr<HittableI> box1 = box(point3{0.0, 0.0, 0.0}, point3(165.0, 330.0, 165.0), white);
    box1 = std::make_shared<RotatedY>(box1, 15);
    box1 = std::make_shared<Translated>(box1, vec3{265.0, 0.0, 295.0});
    world.add(box1);

    std::shared_ptr<HittableI> box2 = box(point3{0.0, 0.0, 0.0}, point3(165.0, 165.0, 165.0), white);
    box2 = std::make_shared<RotatedY>(box2, -18);
    box2 = std::make_shared<Translated>(box2, vec3{130.0, 0.0, 65.0});
    world.add(box2);

    return world;
}




HittableList cornell_smoke() {

    HittableList world = cornell_box_base();

    auto white = std::make_shared<lambertian>(color{0.73, 0.73, 0.73});

    std::shared_ptr<HittableI> box1 = box(point3{0.0, 0.0, 0.0}, point3(165.0, 330.0, 165.0), white);
    box1 = std::make_shared<RotatedY>(box1, 15);
    box1 = std::make_shared<Translated>(box1, vec3{265.0, 0.0, 295.0});
    world.add(std::make_shared<ConstantMedium>(box1, 0.01, color{0.0, 0.0, 0.0}));

    std::shared_ptr<HittableI> box2 = box(point3{0.0, 0.0, 0.0}, point3(165.0, 165.0, 165.0), white);
    box2 = std::make_shared<RotatedY>(box2, -18);
    box2 = std::make_shared<Translated>(box2, vec3{130.0, 0.0, 65.0});
    world.add(std::make_shared<ConstantMedium>(box2, 0.01, color{1.0, 1.0, 1.0}));

    return world;
}

HittableList final_scene() {
    HittableList boxes1;
    // floor
    auto const ground = std::make_shared<lambertian>(color{0.48, 0.83, 0.53});
    int const boxes_per_side = 20;
    for (int ii = 0; ii < boxes_per_side; ++ii)
        for (int jj = 0; jj < boxes_per_side; ++jj) {
            auto const w = 100.0;
            auto const x0 = -1000.0 + ii * w;
            auto const z0 = -1000.0 + jj * w;
            auto const y0 = 0.0;
            auto const x1 = x0 + w;
            auto const y1 = random_double(1.0, 101.0);
            auto const z1 = z0 + w;
            boxes1.add(box(point3{x0, y0, z0}, point3{x1, y1, z1}, ground));
        }

    HittableList world;
    world.add(std::make_shared<BvhNode>(boxes1, TimeInterval{}));

    // light
    auto const light = std::make_shared<DiffuseLight>(color(7.0, 7.0, 7.0));
    world.add(std::make_shared<Quad>(point3{123.0, 554.0, 147.0}, vec3{300.0, 0.0, 0.0}, vec3{0.0, 0.0, 265.0}, light));

    // moving sphere
    auto const center1 = point3{400.0, 400.0, 200.0};
    auto const center2 = center1 + vec3{30.0, 0.0, 0.0};
    auto const sphere_material = std::make_shared<lambertian>(color{0.7, 0.3, 0.1});
    world.add(std::make_shared<MovingSphere>(center1, center2, 0.0, 1.0, 50.0, sphere_material));

    // glass sphere
    world.add(std::make_shared<Sphere>(point3{260.0, 150.0, 45.0}, 50.0, std::make_shared<dielectric>(1.5)));
    // metal sphere
    world.add(std::make_shared<Sphere>(point3{0.0, 150.0, 145.0}, 50.0, std::make_shared<metal>(color{0.8, 0.8, 0.0}, 1.0)));

    // volume
    auto const boundary1 = std::make_shared<Sphere>(point3{360.0, 150.0, 145.0}, 70.0, std::make_shared<dielectric>(1.5));
    world.add(boundary1);
    world.add(std::make_shared<ConstantMedium>(boundary1, 0.2, color(0.2, 0.4, 0.9)));
    // enveloping volume
    // auto const boundary2 = std::make_shared<Sphere>(point3{0.0, 0.0, 0.0}, 5000.0, std::make_shared<dielectric>(1.5));
    // world.add(std::make_shared<ConstantMedium>(boundary2, 0.0001, color{1.0, 1.0, 1.0}));

    // globe
    auto const earth_material = std::make_shared<lambertian>(std::make_shared<ImageTexture>(std::string{DATA_PATH "/earthmap.jpg"}));
    world.add(std::make_shared<Sphere>(point3{400.0, 200.0, 400.0}, 100.0, earth_material));
    // perlin noise sphere
    auto const pertext = std::make_shared<NoiseTexture>(0.2);
    world.add(std::make_shared<Sphere>(point3{220.0, 280.0 ,300.0}, 80.0, std::make_shared<lambertian>(pertext)));

    // spheres in a box
    HittableList boxes2;
    auto const white = std::make_shared<lambertian>(color{0.73, 0.73, 0.73});
    int const ns = 1000;
    for (int j = 0; j < ns; j++)
        boxes2.add(std::make_shared<Sphere>(point3::random(0.0,165.0), 10.0, white));
    world.add(std::make_shared<Translated>(
        make_shared<RotatedY>(
            std::make_shared<BvhNode>(boxes2, TimeInterval{}), 15.0),
            vec3{-100.0, 270.0, 395.0}));

    return world;
}

Scene create_scene(SceneID const id, AspectRatio const aspect_ratio)
{
    vec3 const vertical_up{0.0, 1.0, 0.0};
    auto const focus_distance = FocusDistance{10.0};
    double const time0 = 0.0;
    double const time1 = 1.0;
    BackgroundFunction background_color = default_background;

    point3 lookfrom{13.0, 2.0, 3.0};
    point3 lookat{0.0, 0.0, 0.0};
    auto vertical_fov_degree = FieldOfView{20.0};
    Aperture aperture{0.0};
    HittableList world;

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
    case SceneID::planet:
        world = planet();
        break;
    case SceneID::quads:
        world = quads();
        vertical_fov_degree = FieldOfView{80.0};
        lookfrom = point3{0.0, 0.0, 9.0};
        break;
    case SceneID::simple_light:
        world = simple_light();
        lookfrom = point3{26.0, 3.0, 6.0};
        lookat = point3{0.0, 2.0, 0.0};
        background_color = black_background;
        break;
    case SceneID::cornell_box:
        world = cornell_box();
        vertical_fov_degree = FieldOfView{40.0};
        lookfrom = point3{278.0, 278.0, -800};
        lookat = point3{278.0, 278.0, 0.0};
        background_color = black_background;
        break;
    case SceneID::cornell_smoke:
        world = cornell_smoke();
        vertical_fov_degree = FieldOfView{40.0};
        lookfrom = point3{278.0, 278.0, -800};
        lookat = point3{278.0, 278.0, 0.0};
        background_color = black_background;
        break;
    case SceneID::final_scene:
        world = final_scene();
        vertical_fov_degree = FieldOfView{40.0};
        lookfrom = point3{478.0, 278.0, -600};
        lookat = point3{278.0, 278.0, 0.0};
        background_color = black_background;
        break;
    }

    return {world,
            background_color,
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