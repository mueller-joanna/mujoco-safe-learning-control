cmake_minimum_required(VERSION 3.28)
project(
  safe_learning
  VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# for unix platform, define install directories.
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../../../cmake")
include(Dependencies)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(sl_interfaces REQUIRED)

message("CMAKE_CURRENT_LIST_DIR" ${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
set(CONTROLLERS ${CMAKE_CURRENT_LIST_DIR}/src/controllers)
set(MODELS ${CMAKE_CURRENT_LIST_DIR}/src/models)
set(VISUALIZATION ${CMAKE_CURRENT_LIST_DIR}/src/visualization)
set(LIBS 
  ${MODELS}/cart-pole.cpp
  ${CONTROLLERS}/lqr.cpp
  ${CONTROLLERS}/rl.cpp
  ${VISUALIZATION}/mujoco_sim.cpp
)
add_executable(base ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp ${LIBS})

target_compile_features(base PUBLIC c_std_99 cxx_std_20)

target_link_libraries(base 
  drake::drake 
  #tensorflow 
  Eigen3::Eigen 
  glfw
  mujoco::mujoco
  mujoco::platform_ui_adapter
) # 
ament_target_dependencies(base rclcpp std_msgs example_interfaces sl_interfaces)

install(TARGETS base
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
