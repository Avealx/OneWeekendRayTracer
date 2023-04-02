cmake_minimum_required(VERSION 3.22)
project(weekend_raytracer CXX)

set(CMAKE_CXXSTANDARD 20)

find_package(GTest)

add_executable(main_test src/main.test.cpp)

target_link_libraries(main_test gtest::gtest)
