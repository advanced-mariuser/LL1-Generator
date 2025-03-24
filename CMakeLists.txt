cmake_minimum_required(VERSION 3.8)
project(ll1-generator)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

file(GLOB_RECURSE MODULES_FILES modules/*.cpp modules/*.h)

add_executable(ll1-generator main.cpp ${MODULES_FILES})
target_include_directories(ll1-generator PRIVATE modules)
target_compile_definitions(ll1-generator PRIVATE PROJECT_ROOT="${PROJECT_ROOT}")