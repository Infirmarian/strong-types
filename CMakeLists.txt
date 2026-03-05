cmake_minimum_required(VERSION 3.16)

project(strong_types LANGUAGES CXX)

enable_testing()

foreach(STD IN ITEMS 11 14 17 20)
    set(TARGET "tests_cxx${STD}")

    add_executable(${TARGET})
    set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD ${STD})
    target_sources(${TARGET} PRIVATE test/strong_types_tests.cpp)
    target_include_directories(${TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(${TARGET} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )

    add_test(NAME ${TARGET} COMMAND ${TARGET})
endforeach()
