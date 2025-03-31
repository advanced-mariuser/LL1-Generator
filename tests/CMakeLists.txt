include(AddCatch2)

file(GLOB_RECURSE TEST_FILES "*.cpp")

file(GLOB_RECURSE MODULES_FILES ../modules/*.cpp ../modules/*.h)

add_executable(
        unit_tests
        ${TEST_FILES}
        ${MODULES_FILES}
)
target_link_libraries(
        unit_tests PRIVATE Catch2::Catch2WithMain
)

target_include_directories(unit_tests PRIVATE
        ../modules
        ../modules/TableGenerator
        ../modules/TableGenerator/Data
        ../modules/TableGenerator/Generator
        ../modules/TableGenerator/Parser
        ../modules/TableGenerator/Processor
        ../modules/TableGenerator/Utils
        ../modules/SyntacticRecognizer
        ../modules/Lexer
)

target_link_libraries(unit_tests PRIVATE
        TableGenerator
        SyntacticRecognizer
        Lexer
)

include(Catch)
catch_discover_tests(unit_tests)

add_custom_target(run_tests
        COMMAND unit_tests
        DEPENDS unit_tests
)