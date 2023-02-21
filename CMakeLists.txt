cmake_minimum_required(VERSION 3.0.0)
project(little_raytrace VERSION 0.0.1)

file(GLOB SOURCES src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(little_raytrace ${SOURCES})

target_include_directories(little_raytrace PRIVATE hdr)