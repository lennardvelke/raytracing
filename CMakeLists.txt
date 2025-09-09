cmake_minimum_required(VERSION 3.2)

project(Main)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

find_package(OpenGL REQUIRED)

include_directories(GLOB HEADER_FILE 
include/
shader/
)


configure_file(./shader/fragment_shader.glsl ${CMAKE_BINARY_DIR}/shader/fragment_shader.glsl COPYONLY)
configure_file(./shader/basic.vert ${CMAKE_BINARY_DIR}/shader/basic.vert COPYONLY)
configure_file(./shader/compute_shader.glsl ${CMAKE_BINARY_DIR}/shader/compute_shader.glsl COPYONLY)

file(GLOB_RECURSE SRC_FILES src/*/**.cpp)

add_executable(Main main.cpp ./src/ray.cpp src/camera.cpp ./src/vec3.cpp ./src/renderer.cpp src/glad.c)



add_subdirectory(dependencies/glad)


target_include_directories(Main PUBLIC ${HEADER_FILE} dependencies)

target_link_libraries(Main glfw glad dl OpenGL::GL )



