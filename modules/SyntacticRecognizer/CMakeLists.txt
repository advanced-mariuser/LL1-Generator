add_library(SyntacticRecognizer
        SyntacticRecognizer/SyntacticRecognizer.cpp
        SyntacticRecognizer/SyntacticRecognizer.h
        CSVParser/CSVParser.cpp
        CSVParser/CSVParser.h
        TokensConnector/TokensConnector.h
)
target_include_directories(SyntacticRecognizer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/SyntacticRecognizer
        ${CMAKE_CURRENT_SOURCE_DIR}/CSVParser
        ${CMAKE_CURRENT_SOURCE_DIR}/TokensConnector
        ${PROJECT_ROOT}/modules/Lexer/Token
        ${PROJECT_ROOT}/modules/Common
)