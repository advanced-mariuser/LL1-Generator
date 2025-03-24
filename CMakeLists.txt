cmake_minimum_required(VERSION 3.8)
project(ll1-generator)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

enable_testing()

option(ENABLE_MODULE1 "Enable MODULE1" ON)
option(ENABLE_module1 "Enable module1" OFF)
option(ENABLE_MODULE2 "Enable module2" ON)
option(ENABLE_MODULE3 "Enable module3" OFF)

add_executable(ll1-generator main.cpp)

function(add_module_if_enabled module_name)
    if (${ENABLE_${module_name}})
        message(STATUS "Enabling module: ${module_name}")
        add_subdirectory(modules/${module_name})
        target_link_libraries(ll1-generator PRIVATE ${module_name})
    else ()
        message(STATUS "Disabling module: ${module_name}")
    endif ()
endfunction()

add_module_if_enabled(MODULE1)
add_module_if_enabled(MODULE2)
add_module_if_enabled(MODULE3)

add_subdirectory(tests)

add_dependencies(ll1-generator run_tests)
