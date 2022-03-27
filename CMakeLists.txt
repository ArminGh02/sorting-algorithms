cmake_minimum_required(VERSION 3.11)
project(SortingAlgorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED on)

option(OPTIONS_ENABLE_TESTING "Enable Tests Using Catch2" on)
if (OPTIONS_ENABLE_TESTING)
  include(cmake/ConfigureProjectTests.cmake)
  Core_ConfigureProjectTests()
endif()

option(OPTIONS_ENABLE_BENCHMARKING "Enable Benchmarks Using Google Benchmarks" on)
if (OPTIONS_ENABLE_BENCHMARKING)
  include(cmake/ConfigureProjectBenchmark.cmake)
  Core_ConfigureProjectBenchmark()
endif()

add_library(${PROJECT_NAME} INTERFACE include/sorting_algorithms/sort.hpp)
add_library(SortAlgorithmsLibrary ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
