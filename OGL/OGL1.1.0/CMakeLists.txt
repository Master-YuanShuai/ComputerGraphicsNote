cmake_minimum_required(VERSION 3.21)
project(Firstobj)

set(CMAKE_CXX_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(Firstobj main.cpp src/glad.c)

target_link_libraries(Firstobj ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)