include(FetchContent)

# From https://discourse.cmake.org/t/how-to-turn-off-warning-flags-for-project-added-by-fetchcontent-declare/2461
# TODO add if debug
if( CMAKE_BUILD_TYPE STREQUAL "Debug")
message("#### Don't display warnings as errors when building dependencies ####")
set_property(
   DIRECTORY
   APPEND
   PROPERTY COMPILE_OPTIONS -Wno-error=format-truncation
)
endif()

#set(Local_LIBS
#    /usr/local/lib
#)
#add_library(TensorFlow OBJECT ${Local_LIBS}/libtensorflow.so)

set(MUJOCO_BUILD_EXAMPLES OFF)
set(MUJOCO_BUILD_SIMULATE ON)
set(MUJOCO_BUILD_TESTS OFF)
set(MUJOCO_TEST_PYTHON_UTIL OFF)

message("#### Building mujoco ####")
FetchContent_Declare(
  mujoco
  GIT_REPOSITORY https://github.com/google-deepmind/mujoco.git
  GIT_TAG        3.3.7
  UPDATE_DISCONNECTED TRUE
  EXCLUDE_FROM_ALL TRUE 
)

FetchContent_GetProperties(mujoco)
FetchContent_MakeAvailable(mujoco)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

message("#### Building glfw3 ####")
message("glfw3 version: `${MUJOCO_DEP_VERSION_glfw3}`")
FetchContent_Declare(
  glfw
  USE_SYSTEM_PACKAGE ON
  #MJPC_USE_SYSTEM_GLFW
  #glfw
  #SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/glfw3-src/
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG ${MUJOCO_DEP_VERSION_glfw3}
  #TARGETS
  #glfw
  #EXCLUDE_FROM_ALL
)

FetchContent_GetProperties(glfw)

find_package(Eigen3 REQUIRED NO_MODULE)

find_package(drake CONFIG REQUIRED PATHS /opt/drake)

message("#### Building curl ####")
FetchContent_Declare(
  curl
  URL https://curl.se/download/curl-8.16.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
  OVERRIDE_FIND_PACKAGE
)

#FetchContent_GetProperties(curl)
FetchContent_MakeAvailable(curl)
find_package(curl)
