cmake_minimum_required(VERSION 3.30)
project(chmath_tests_table LANGUAGES CXX)

add_executable(table table.cpp)
target_include_directories(table PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(table PRIVATE chmath)
