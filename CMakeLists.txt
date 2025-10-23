cmake_minimum_required(VERSION 3.28)
project(safe_learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# for unix platform, define install directories.
include(GNUInstallDirs)

set(Local_LIBS
    /usr/local/lib
)
add_library(TensorFlow OBJECT ${Local_LIBS}/libtensorflow.so)

find_package(Eigen3 REQUIRED NO_MODULE)

set(MUJOCO_FOLDER /usr/lib/x86_64-linux-gnu)
add_library(mujoco OBJECT ${MUJOCO_FOLDER}/libmujoco.so)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
set(MODELS src/models)
set(LIBS ${MODELS}/cart-pole.cpp)
add_executable(safe_learning src/main.cpp ${LIBS})

target_compile_features(safe_learning PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++1
install(TARGETS safe_learning
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

target_link_libraries(safe_learning PRIVATE tensorflow Eigen3::Eigen ${MUJOCO_FOLDER}/libmujoco.so) # 

## Testing
# from https://github.com/aminya/project_options/issues/146
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
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
