function(Core_ConfigureProjectTests)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.8
  )
  FetchContent_MakeAvailable(Catch2)

  set(TargetName ${PROJECT_NAME}Tests)

  add_executable(${TargetName} test/sort_test.cpp)
  target_link_libraries(${TargetName} PRIVATE
    Catch2::Catch2
    SortAlgorithmsLibrary
  )

  include(CTest)
  include(${Catch2_SOURCE_DIR}/contrib/Catch.cmake)
  catch_discover_tests(${TargetName})
endfunction()
