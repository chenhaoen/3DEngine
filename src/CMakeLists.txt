project(Base)

find_package(assimp)

set(BASE_INCLUDE_DIR ${INCLUDE_DIR}/Base)
set(BASE_SRC_DIR ${SRC_DIR}/Base)
set(NODES_INCLUDE_DIR ${INCLUDE_DIR}/Nodes)
set(NODES_SRC_DIR ${SRC_DIR}/Nodes)
set(VK_INCLUDE_DIR ${INCLUDE_DIR}/Vk)
set(VK_SRC_DIR ${SRC_DIR}/Vk)

file(GLOB BASE_SOURCES "${BASE_SRC_DIR}/*.cpp")
file(GLOB BASE_HEADERS "${BASE_INCLUDE_DIR}/*.h")
file(GLOB NODES_SOURCES "${NODES_SRC_DIR}/*.cpp")
file(GLOB NODES_HEADERS "${NODES_INCLUDE_DIR}/*.h")
file(GLOB VK_SOURCES "${VK_SRC_DIR}/*.cpp")
file(GLOB VK_HEADERS "${VK_INCLUDE_DIR}/*.h")
file(GLOB IMGUI_HEADERS "${3DEngine_SOURCE_DIR}/3rd/imgui/*.h")
file(GLOB IMGUI_SOURCES "${3DEngine_SOURCE_DIR}/3rd/imgui/*.cpp")

set(ALL_HEADERS ${BASE_HEADERS} ${NODES_HEADERS} ${IMGUI_HEADERS} ${VK_HEADERS})
set(ALL_SOURCES ${BASE_SOURCES} ${NODES_SOURCES} ${IMGUI_SOURCES} ${VK_SOURCES})

add_library(${PROJECT_NAME} SHARED 
${ALL_HEADERS}
${ALL_SOURCES}
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_vulkan.h
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_vulkan.cpp
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.h
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp
)

add_compile_definitions(BASE_EXPORT)
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)


target_include_directories(${PROJECT_NAME} PRIVATE 
${3DEngine_SOURCE_DIR}/3rd/imgui
${3DEngine_SOURCE_DIR}/3rd/stb)

target_link_libraries(${PROJECT_NAME} 
Vulkan::Vulkan
glfw 
assimp::assimp)