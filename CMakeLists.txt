cmake_minimum_required(VERSION 3.10)
project(graphw CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GRAPHW_BUILD_EXAMPLES "Set to ON to build examples" ON)

add_library(graphw INTERFACE)
target_include_directories(graphw INTERFACE graphw)
target_link_libraries(graphw INTERFACE SDL2 SDL2main SDL2_image)

# Test
find_package(Catch2)
if(Catch2_FOUND)
  add_executable(test tests/main.cpp tests/graphw.cpp)
  target_link_libraries(test graphw Catch2::Catch2)
endif()

# Benchmark
add_executable(benchmark tests/benchmark.cpp)
target_link_libraries(benchmark graphw)

# Examples
if(GRAPHW_BUILD_EXAMPLES)
  set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
  set(EXAMPLE_BASENAMES animate arc_diagram circular_layout
                        force_directed_layout random_layout spiral_layout)
  foreach(name ${EXAMPLE_BASENAMES})
    add_executable(${name} ${EXAMPLES_DIR}/${name}.cpp)
    target_link_libraries(${name} graphw)
  endforeach()
endif()
