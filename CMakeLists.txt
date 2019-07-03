cmake_minimum_required(VERSION 3.14.5)
project(RayTracer CXX)

if(NOT CMAKE_BUILD_TYPE)
#	set(CMAKE_BUILD_TYPE Release)
endif()

if(APPLE)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
)
