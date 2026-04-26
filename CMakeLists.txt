cmake_minimum_required(VERSION 3.20)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    external/glad/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    external/glad/include
)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
)
