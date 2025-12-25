cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(chmath VERSION 0.3.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_subdirectory(util)

set(mods
  src/chmath.cppm
  src/plot.cppm
  src/table.cppm
)

add_library(chmath STATIC)
target_sources(chmath PUBLIC
  FILE_SET cxx_module_files
  TYPE CXX_MODULES
  FILES ${mods}
)
target_compile_options(chmath PUBLIC -Wall -Wextra -Wpedantic)

target_include_directories(chmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/util/include)
target_link_libraries(chmath PUBLIC chutil)

add_subdirectory(tests)
