cmake_minimum_required(VERSION 3.28)
message(STATUS "Using ${CMAKE_GENERATOR}")

project(
  safe_learning
  VERSION 0.0.1
)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_C_COMPILER "/usr/bin/clang-14")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-14")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  #add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# for unix platform, define install directories.
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(Dependencies)
  
set(VENDORS_DIR ${CMAKE_CURRENT_LIST_DIR}/vendors)
#add_subdirectory(${VENDORS_DIR}/reflect)
include_directories(${VENDORS_DIR}/reflect)


include_directories(${CMAKE_CURRENT_LIST_DIR}/vendors/)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
set(CONTROLLERS src/controllers)
set(MODELS src/models)
set(LIBS ${MODELS}/cart-pole.cpp ${CONTROLLERS}/lqr.cpp ${CONTROLLERS}/rl.cpp)
add_executable(safe_learning src/main.cpp ${LIBS})

target_compile_features(safe_learning PUBLIC c_std_99 cxx_std_20) 

install(TARGETS safe_learning
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

#target_sources(safe_learning PRIVATE 
#  FILE_SET all_my_modules TYPE CXX_MODULES FILES
#  ${VENDORS_DIR}/reflect/reflect.cppm
#)

target_link_libraries(safe_learning PRIVATE 
  drake::drake 
  #tensorflow 
  Eigen3::Eigen 
  glfw
  mujoco::mujoco
  mujoco::platform_ui_adapter
  CURL::libcurl
) # 

include(Testing)
