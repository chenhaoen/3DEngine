project(Base)

set(BASE_INCLUDE_DIR ${INCLUDE_DIR}/Base)
set(BASE_SRC_DIR ${SRC_DIR}/Base)


find_package(glfw3)

file(GLOB ALL_SOURCE "${BASE_SRC_DIR}/*.cpp")
file(GLOB ALL_HEADER "${BASE_INCLUDE_DIR}/*.h")

add_library(${PROJECT_NAME} SHARED 
${ALL_HEADER}
${ALL_SOURCE}
)

add_compile_definitions(BASE_EXPORT)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)