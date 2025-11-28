cmake_minimum_required(VERSION 3.10)
project(graphics-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(external/glad/include)

add_executable(Graphics-Engine 
    src/main.cpp
    src/glad.c
)

target_link_libraries(Graphics-Engine
    glfw
    glm::glm
    opengl32
)