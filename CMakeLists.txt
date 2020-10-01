cmake_minimum_required(VERSION 3.10)
project(graphw CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(SDL2 SDL2main SDL2_image)

# Test
find_package(Catch2)
if(Catch2_FOUND)
    add_executable(test tests/test_main.cpp tests/test_graphw.cpp)
    target_link_libraries(test Catch2::Catch2)
endif()

# Benchmark
add_executable(benchmark tests/benchmark.cpp)

# Examples
add_executable(example_animate examples/animate.cpp)
add_executable(example_arc_diagram examples/arc_diagram.cpp)
add_executable(example_circular_layout examples/circular_layout.cpp)
add_executable(example_force_directed_layout examples/force_directed_layout.cpp)
add_executable(example_random_layout examples/random_layout.cpp)
add_executable(example_spiral_layout examples/spiral_layout.cpp)
