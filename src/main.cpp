#include <color.hpp>

#include <iostream>

int main() {
    // Image
    int const image_width = 256;
    int const image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' <<  std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color{double{image_width} - 1, double{image_height} - 1, 0.25};
            write_color(std::cout, pixel_color, false);
        }
    }
    std:: cerr << "\nDone.\n";
}