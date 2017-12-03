cmake_minimum_required(VERSION 3.8.5)
project (fluid)
set( CMAKE_BUILD_TYPE Release )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON ) 

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(Flow)
add_subdirectory(Fluid)
