cmake_minimum_required(VERSION 3.8.5)


set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --std=c++1z -Wall -Wextra -Wpedantic -Wno-unknown-pragmas" )

set ( PROJECT_LINK_LIBS flow glad glfw)

add_executable(fluid main.cpp)
target_link_libraries(fluid ${PROJECT_LINK_LIBS} )
target_include_directories(fluid PUBLIC include)

add_custom_target(build-time-copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/marching_squares/
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Flow/Shaders/*.glsl
            ${CMAKE_CURRENT_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/Flow/Shaders/marching_squares/*.glsl
            ${CMAKE_CURRENT_BINARY_DIR}/marching_squares/)
