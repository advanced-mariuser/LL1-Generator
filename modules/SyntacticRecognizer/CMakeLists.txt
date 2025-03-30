add_library(SyntacticRecognizer
        SyntacticRecognizer/SyntacticRecognizer.cpp
        SyntacticRecognizer/SyntacticRecognizer.h
        CSVParser/CSVParser.cpp
        CSVParser/CSVParser.h
)
target_include_directories(SyntacticRecognizer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/SyntacticRecognizer
        ${CMAKE_CURRENT_SOURCE_DIR}/CSVParser
        ${PROJECT_ROOT}/modules
        ${PROJECT_ROOT}/modules/Lexer/Token
)