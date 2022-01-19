cmake_minimum_required(VERSION 3.21)
project(OGLOBJ)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(OGLOBJ main.cpp src/glad.c shader/Shader.hpp)

target_link_libraries(OGLOBJ ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)