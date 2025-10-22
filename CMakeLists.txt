cmake_minimum_required(VERSION 3.8)
project(safe-learning)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# for unix platform, define install directories.
include(GNUInstallDirs)

set(Local_LIBS
    /usr/local/lib
)
add_library(TensorFlow OBJECT ${Local_LIBS}/libtensorflow.so)

add_executable(example src/example.cpp)

install(TARGETS example
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

target_link_libraries(example PRIVATE tensorflow) # 
