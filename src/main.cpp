#include <bvh.hpp>
#include <color.hpp>
#include <counter.hpp>
#include <ray.hpp>
#include <scenes.hpp>
#include <vec3.hpp>

#include <iostream>
#include <memory>
#include <thread>


auto const aspect_ratio = AspectRatio{3.0 / 2.0};


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
    int const image_width = 300;
    std::size_t const image_height = static_cast<int>(image_width / aspect_ratio.value());
    int const samples_per_pixel = 50;
    int const max_depth = 5;

    auto const scene = create_scene(SceneID::final_scene, aspect_ratio);

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
