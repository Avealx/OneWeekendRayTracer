cmake_minimum_required(VERSION 3.22)
project(weekend_raytracer CXX)

set(CMAKE_CXXSTANDARD 20)

include(CTest)

find_package(GTest)

add_subdirectory("src")