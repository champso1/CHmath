cmake_minimum_required(VERSION 3.30)
project(chmath_tests_plotting LANGUAGES CXX)

add_executable(plotting plotting.cpp)
target_include_directories(plotting PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(plotting PUBLIC chmath)
