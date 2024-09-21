project(triangle)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Base)

execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/shaders/shader.vert -o ${CMAKE_CURRENT_LIST_DIR}/shaders/vert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/shaders/shader.frag -o ${CMAKE_CURRENT_LIST_DIR}/shaders/frag.spv)

configure_file(${CMAKE_CURRENT_LIST_DIR}/shaders/vert.spv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/vert.spv COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/shaders/frag.spv ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/frag.spv COPYONLY)