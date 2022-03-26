cmake_minimum_required(VERSION 3.11)
project(SortingAlgorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include(cmake/ConfigureProjectTests.cmake)
Core_ConfigureProjectTests()

include(cmake/ConfigureProjectBenchmark.cmake)
Core_ConfigureProjectBenchmark()

add_library(${PROJECT_NAME} INTERFACE include/sorting_algorithms/sort.hpp)
add_library(SortAlgorithmsLibrary ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
