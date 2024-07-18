cmake_minimum_required(VERSION 3.5)
project(owlstd)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build examples targets" OFF)
option(BUILD_TESTS "Build testing target" OFF)
option(ENABLE_CCACHE "Use ccache for compilation" ON)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE
  src/str.c
  src/str_hashmap.c
  src/vector.c
  src/queue.c
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif(BUILD_TESTS)

# if (BUILD_EXAMPLES)
#   add_subdirectory(examples)
# endif(BUILD_EXAMPLES)

if (ENABLE_CCACHE)
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_COMPILER_LAUNCHER ccache)
  else()
    warning(cchache not found)
  endif()
endif(ENABLE_CCACHE)
