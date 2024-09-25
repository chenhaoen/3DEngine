project(Base)

set(BASE_INCLUDE_DIR ${INCLUDE_DIR}/Base)
set(BASE_SRC_DIR ${SRC_DIR}/Base)

find_package(glfw3)

file(GLOB ALL_SOURCES "${BASE_SRC_DIR}/*.cpp")
file(GLOB ALL_HEADERS "${BASE_INCLUDE_DIR}/*.h")
file(GLOB IMGUI_HEADERS "${3DEngine_SOURCE_DIR}/3rd/imgui/*.h")
file(GLOB IMGUI_SOURCES "${3DEngine_SOURCE_DIR}/3rd/imgui/*.cpp")

add_library(${PROJECT_NAME} SHARED 
${ALL_HEADERS}
${ALL_SOURCES}
${IMGUI_HEADERS}
${IMGUI_SOURCES}
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_vulkan.h
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_vulkan.cpp
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.h
${3DEngine_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp
)

add_compile_definitions(BASE_EXPORT)

target_include_directories(${PROJECT_NAME} PRIVATE 
${3DEngine_SOURCE_DIR}/3rd/imgui)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)