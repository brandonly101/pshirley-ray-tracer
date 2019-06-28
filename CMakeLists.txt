cmake_minimum_required (VERSION 3.14.5)
project (RayTracer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES src/*.cpp)

# add_executable(RayTracer main.cpp)
add_executable(RayTracer ${SOURCES})

# target_include_directories(test PRIVATE ${YOUR_DIRECTORY})
target_include_directories(RayTracer
    PUBLIC
        include
)
