add_library(Lexer
        Lexer/Lexer.cpp
        Lexer/Lexer.h
        Token/Token.h
        Token/TokenType.h
        Token/TokenUtils.h
)
target_include_directories(Lexer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Lexer
        ${CMAKE_CURRENT_SOURCE_DIR}/Token
)