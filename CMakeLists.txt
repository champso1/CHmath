cmake_minimum_required(VERSION 3.30)
project(chmath VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# grab all the source files
file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/src/* ${CMAKE_SOURCE_DIR}/include/*)

# we build a static library.
add_library(chmath STATIC ${sources})
target_include_directories(chmath PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(chmath PUBLIC -Wall -Wextra -Wpedantic)

add_subdirectory(tests)
