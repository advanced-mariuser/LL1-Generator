FILE(GLOB_RECURSE SRC "*.cpp" "*.h")
add_library(grammar_optimizer ${SRC})
target_include_directories(grammar_optimizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})