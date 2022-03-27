function(Core_ConfigureProjectBenchmark)
  include(FetchContent)
  set(BENCHMARK_ENABLE_TESTING off)
  FetchContent_Declare(
    GoogleBenchmark
    GIT_REPOSITORY   https://github.com/google/benchmark.git
    GIT_TAG          v1.6.1
  )
  FetchContent_MakeAvailable(GoogleBenchmark)

  set(BenchmarkTargetName ${PROJECT_NAME}Benchmark)

  add_executable(${BenchmarkTargetName} benchmark/sort_benchmark.cpp)
  target_link_libraries(${BenchmarkTargetName} PRIVATE
    benchmark::benchmark
    SortAlgorithmsLibrary
  )
endfunction()
