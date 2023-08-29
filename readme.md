# Ray Tracing in One Weekend
My take on the fantastic series by Shirley, Black, and Hollasch:

* [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) (implemented)
*  [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) (implemented)
* [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html) (ongoing, of course)

![Final screenshot of book 2](/data/output/TheNextWeek/image_08_final_scene.png)
Final rendering of book 2. A beautiful scene showcasing different materials (glass, metal and diffuse), multiple instances, textures, motion blur and Perlin-noise.

# Install

Using conan 2.0.2:

`conan install . --output-folder=build --build=missing`
`cd build`
`cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`
`cmake --build .`

# Notes
Although an reference implementation is given by the authors, I used TDD to get some exercise and to have a safety net when refactoring and deviating from the reference.

## Parallelism
Ray tracing is inherently [embarrassingly parallel](https://en.wikipedia.org/wiki/Embarrassingly_parallel). Yet, when parallelizing the algorithm, the performance was nowhere near the expectation. Profiling showed that most of the time was spend in the random number generator. Making the random number generator thread-local added another factor of 2.

## TODO
- [x] parallelize
- [x] inject vector functionality using policies
- [ ] Try alternative BVHs
- [ ] slim down cmake dependencies (currently everything depends on everything)
- [ ] move implementations to source files to speed up the compilation (check the effect on performance)
- [ ] implement proper volume rendering ([scratchapixel seems to have a nice write-up of it](https://www.scratchapixel.com/lessons/3d-basic-rendering/volume-rendering-for-developers/intro-volume-rendering.html))
