cmake_minimum_required(VERSION 3.22)

project(3DEngine CXX)
set(CMAKE_CXX_STANDARD 20)

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

find_package(Vulkan)

include_directories(${INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/resources)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)