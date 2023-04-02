cmake_minimum_required(VERSION 3.22)
project(weekend_raytracer CXX)

set(CMAKE_CXXSTANDARD 20)

include(CTest)

find_package(GTest)

add_executable(main src/main.cpp)

function(setup_test target_filepath)
    # Simplify adding a new test.
    # E.g. target_filepath = "src/myfile.test.cpp"
    #      -> target_name = test name = test command = "myfile_test"
    set(test_libraries
        GTest::gtest
        GTest::gmock)

    get_filename_component(filename ${target_filepath} NAME)
    string(REGEX REPLACE "\\.cpp$" "" filename_no_ext ${filename})
    string(REGEX REPLACE "\\." "_" target_name ${filename_no_ext})

    add_executable(${target_name} ${target_filepath})
    target_link_libraries(${target_name} ${test_libraries})
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

setup_test(src/main.test.cpp)
setup_test(src/vec3.test.cpp)