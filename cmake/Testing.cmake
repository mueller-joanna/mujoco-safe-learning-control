include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
find_package(GTest REQUIRED NO_MODULE)

enable_testing()

add_executable(
  main_test
  tests/main_test.cpp
)
target_link_libraries(
  main_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(main_test)