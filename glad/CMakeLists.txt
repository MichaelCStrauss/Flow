cmake_minimum_required(VERSION 3.8.5)
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES "src/*.c")

add_library(glad STATIC ${SOURCES})
target_include_directories(glad PUBLIC include)