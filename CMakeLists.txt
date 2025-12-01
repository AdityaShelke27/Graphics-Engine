cmake_minimum_required(VERSION 3.10)
project(graphics-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(external/glad/include)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
    src/*.h
)

add_executable(Graphics-Engine ${SOURCES})

# Give shader directory to C++ code
target_compile_definitions(Graphics-Engine PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders"
)

#set_property(TARGET Graphics-Engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Portable shader copying (optional)
add_custom_command(
    TARGET Graphics-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Graphics-Engine>/shaders
)

target_link_libraries(Graphics-Engine
    glfw
    glm::glm
    opengl32
)