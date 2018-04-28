cmake_minimum_required(VERSION 3.8.5)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set ( PROJECT_LINK_LIBS glad glfw)
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wpedantic -Wno-unknown-pragmas" )
 
file(GLOB_RECURSE SOURCES "src/*.cpp" )

add_library(flow STATIC ${SOURCES})
target_link_libraries(flow ${PROJECT_LINK_LIBS} )
target_include_directories(flow PUBLIC include )
