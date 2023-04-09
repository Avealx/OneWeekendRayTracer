add_executable(main main.cpp)

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
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

setup_test(main.test.cpp)
setup_test(color.test.cpp)
setup_test(vec3.test.cpp)