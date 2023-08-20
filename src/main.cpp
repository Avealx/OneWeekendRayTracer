#include <bvh.hpp>
#include <camera.hpp>
#include <color.hpp>
#include <common.hpp>
#include <counter.hpp>
#include <hit.hpp>
#include <hittable_list.hpp>
#include <hittables.hpp>
#include <moving_sphere.hpp>
#include <perlin.hpp>
#include <quad.hpp>
#include <ray.hpp>
#include <sphere.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>
#include <thread>


auto const aspect_ratio = 3.0 / 2.0;


HittableList random_scene() {
    HittableList world;

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


using BackgroundFunction = std::function<color(Ray const &)>;

color default_background(Ray const & ray) {
    vec3 const unit_direction = unit_vector(ray.d);
    auto t = 0.5 * (unit_direction.y + 1.0);
    return (1.0 - t) * color{1.0, 1.0, 1.0} + t * color{0.5, 0.7, 1.0};
}

color black_background(Ray const &){
    return color{0.0};
}

color ray_color(Ray const & r,
                HittableI const & world,
                int depth,
                BackgroundFunction const & background_color) {
    if (depth <= 0)
        return color{0.0, 0.0, 0.0};

    HitRecord rec = world.hit(r, 1e-3, infinity);
    if (rec) {
        auto const scatter_info = rec.material_ptr->scatter(r, rec);
        if (scatter_info)
            return scatter_info.emitted + scatter_info.attenuation * ray_color(scatter_info.scattered_ray,
                                                                               world,
                                                                               depth - 1,
                                                                               background_color);
        return scatter_info.emitted;
    }

    // background
    return background_color(r);
}

enum class SceneID {
    random_spheres,
    two_spheres,
    two_perlin_spheres,
    planet,
    quads,
    simple_light,
    cornell_box,
    cornell_smoke,
};

struct Scene {
    HittableList world;
    BackgroundFunction background_color;
    Camera camera;
};

Scene select_scene(SceneID const id)
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

std::mutex CERR_MUTEX;

void render_rows(Counter& rows,
                 int const image_height,
                 int const image_width,
                 int const samples_per_pixel,
                 int const max_depth,
                 Scene const scene,
                 std::vector<std::vector<color>>& result_image) {
        auto const world = BvhNode(scene.world, TimeInterval{0.0, 1.0});
        auto const camera = scene.camera;

        while(auto next_row = rows.next())
        {
            int j = image_height - static_cast<int>(next_row.value()) - 1;

            {
                std::lock_guard lock{CERR_MUTEX};
                std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            }

            for (int i = 0; i < image_width; ++i)
            {
                color pixel_color{0.0, 0.0, 0.0};
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto w = (i + random_double()) / (image_width - 1);
                    auto h = (j + random_double()) / (image_height - 1);
                    Ray r = camera.get_ray(w, h);
                    pixel_color += ray_color(r, world, max_depth, scene.background_color);
                }

                result_image[next_row.value()][i] = pixel_color;
            }
        }
}


int main() {
    // Image
    int const image_width = 1200;
    int const image_height = static_cast<int>(image_width / aspect_ratio);
    int const samples_per_pixel = 50;
    int const max_depth = 50;

    auto const scene = select_scene(SceneID::cornell_smoke);

    std::vector<std::vector<color>> result(image_height, std::vector<color>(image_width));

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    if (true) {
        Counter rows{image_height};
        auto const number_of_threads = std::max(2u, std::thread::hardware_concurrency());
        std::vector<std::thread> threads(number_of_threads - 1);
        for (auto & thread : threads)
            thread = std::thread{render_rows,
                                 std::ref(rows),
                                 image_height,
                                 image_width,
                                 samples_per_pixel,
                                 max_depth,
                                 scene,
                                 std::ref(result)};
        render_rows(rows,
                    image_height,
                    image_width,
                    samples_per_pixel,
                    max_depth,
                    scene,
                    std::ref(result));

        for (auto & thread : threads)
            thread.join();

        for (auto const & row : result)
            for (auto const & pixel : row) {
                write_color(std::cout,
                            pixel,
                            samples_per_pixel,
                            ns_color::WritePretty{false},
                            ns_color::GammaCorrection{true});
                std::cout << " ";
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
