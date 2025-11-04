include(FetchContent)

message("#### Building testing dependencies ####")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)

#find_package(GTest REQUIRED NO_MODULE)

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
