cmake_minimum_required(VERSION 3.8)
project(ll1-generator)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")

enable_testing()

option(ENABLE_SyntacticRecognizer "Enable SyntacticRecognizer" ON)
option(ENABLE_Lexer "Enable Lexer" ON)
option(ENABLE_TableGenerator "Enable TableGenerator" ON)
option(ENABLE_GrammarOptimizer "Enable GrammarOptimizer" ON)

add_executable(ll1-generator main.cpp)

configure_file(
        ${CMAKE_SOURCE_DIR}/modules/ll1_table.csv
        ${CMAKE_BINARY_DIR}/modules/ll1_table.csv
        COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/input.txt
        ${CMAKE_BINARY_DIR}/input.txt
        COPYONLY
)

target_include_directories(ll1-generator PRIVATE
        ${PROJECT_ROOT}
        ${PROJECT_ROOT}/modules
        ${PROJECT_ROOT}/modules/SyntacticRecognizer/SyntacticRecognizer
        ${PROJECT_ROOT}/modules/SyntacticRecognizer/CSVParser
        ${PROJECT_ROOT}/modules/Lexer/Lexer
        ${PROJECT_ROOT}/modules/Lexer/Token
)

function(add_module_if_enabled module_name)
    if (${ENABLE_${module_name}})
        message(STATUS "Enabling module: ${module_name}")
        add_subdirectory(modules/${module_name})
        target_link_libraries(ll1-generator PRIVATE ${module_name})
    else ()
        message(STATUS "Disabling module: ${module_name}")
    endif ()
endfunction()

add_module_if_enabled(SyntacticRecognizer)
add_module_if_enabled(Lexer)
add_module_if_enabled(TableGenerator)
add_module_if_enabled(GrammarOptimizer)

add_subdirectory(tests)

add_dependencies(ll1-generator run_tests)
