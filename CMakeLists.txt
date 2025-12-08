cmake_minimum_required(VERSION 3.10)
project(graphics-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(
    external/glad/include
    external/glm
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
    src/*.h
)

add_executable(Graphics-Engine ${SOURCES} "src/image_loader.cpp" "src/Light.h")

# Give shader directory to C++ code
target_compile_definitions(Graphics-Engine PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders"
)

#set_property(TARGET Graphics-Engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Convert shader files to headers
file(GLOB SHADERS "${CMAKE_SOURCE_DIR}/shaders/*")

#foreach(SHADER ${SHADERS})
#    get_filename_component(NAME ${SHADER} NAME)
#    set(OUT "${CMAKE_BINARY_DIR}/generated/${NAME}.h")
#
#    add_custom_command(
#        OUTPUT ${OUT}
#        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated
#        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} ${OUT}
#        DEPENDS ${SHADER}
#    )
#
#    list(APPEND GENERATED_SHADERS ${OUT})
#endforeach()

target_include_directories(Graphics-Engine PRIVATE
    ${CMAKE_BINARY_DIR}/generated
)
# Portable shader copying
add_custom_command(
    TARGET Graphics-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Graphics-Engine>/shaders
)

# Portable texture copying
add_custom_command(
    TARGET Graphics-Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:Graphics-Engine>/textures
)

# Add the generated headers to your build
target_sources(Graphics-Engine PRIVATE ${GENERATED_SHADERS})

target_link_libraries(Graphics-Engine
    glfw
    glm::glm
    opengl32
)