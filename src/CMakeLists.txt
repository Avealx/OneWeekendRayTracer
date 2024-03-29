add_executable(main main.cpp)
target_include_directories(main
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${EXTERNAL_PATH})
target_compile_definitions(main
    PRIVATE
    DATA_PATH="${DATA_PATH}")

function(setup_test target_filepath)
    # Simplify adding a new test.
    # E.g. target_filepath = "src/myfile.test.cpp"
    #      -> target_name = test name = test command = "myfile_test"
    #      -> if present, add "myfile.cpp" to the target sources
    set(test_libraries
        GTest::gtest
        GTest::gmock)

    get_filename_component(filename ${target_filepath} NAME)
    string(REGEX REPLACE "\\.cpp$" "" filename_no_ext ${filename})
    string(REGEX REPLACE "\\." "_" target_name ${filename_no_ext})

    add_executable(${target_name} ${target_filepath})
    target_link_libraries(${target_name} ${test_libraries})
    target_include_directories(${target_name}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${EXTERNAL_PATH})
    target_compile_definitions(${target_name}
        PRIVATE
        DATA_PATH="${DATA_PATH}")
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

setup_test(aabb.test.cpp)
setup_test(bvh.test.cpp)
setup_test(crtp_functionality.test.cpp)
setup_test(main.test.cpp)
setup_test(material.test.cpp)
setup_test(camera.test.cpp)
setup_test(color.test.cpp)
setup_test(common.test.cpp)
setup_test(counter.test.cpp)
setup_test(hit.test.cpp)
setup_test(hittable_list.test.cpp)
setup_test(hittables.test.cpp)
setup_test(interval.test.cpp)
setup_test(moving_sphere.test.cpp)
setup_test(perlin.test.cpp)
setup_test(quad.test.cpp)
setup_test(ray.test.cpp)
setup_test(sphere.test.cpp)
setup_test(texture.test.cpp)
setup_test(vec3.test.cpp)