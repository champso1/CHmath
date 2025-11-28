cmake_minimum_required(VERSION 3.30)
project(chmath VERSION 0.0.2 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(mods
  include/chmath/chmath.cppm
  include/chmath/plot.cppm
  include/chmath/table.cppm
)

add_library(chmath STATIC)
target_sources(chmath PUBLIC
  FILE_SET cxx_module_files
  TYPE CXX_MODULES
  FILES ${mods}
)
target_compile_options(chmath PUBLIC -Wall -Wextra -Wpedantic)

add_subdirectory(tests)
