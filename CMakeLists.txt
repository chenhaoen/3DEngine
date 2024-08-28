cmake_minimum_required(VERSION 3.28)

project(3DEngine CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples)