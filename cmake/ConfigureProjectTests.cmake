function(Core_ConfigureProjectTests)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.8
  )
  FetchContent_MakeAvailable(Catch2)

  add_executable(${PROJECT_NAME}Tests test/sort_test.cpp)
  target_link_libraries(${PROJECT_NAME}Tests PRIVATE Catch2::Catch2)

  include(CTest)
  include(${Catch2_SOURCE_DIR}/contrib/Catch.cmake)
  catch_discover_tests(${PROJECT_NAME}Tests)
endfunction()
