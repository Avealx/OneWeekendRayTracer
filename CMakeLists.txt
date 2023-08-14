cmake_minimum_required(VERSION 3.22)
project(weekend_raytracer CXX)

set(CMAKE_CXX_STANDARD 20)

include(CTest)

find_package(GTest)

set(EXTERNAL_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/external")

set(DATA_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/data")

add_subdirectory("src")
