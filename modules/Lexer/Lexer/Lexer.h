#pragma once

#include "vector"
#include "../Token/TokenType.h"
#include "../Token/Token.h"

enum class LexerState
{
    DEFAULT,
    STRING,
    COMMENT,
    BLOCK_COMMENT,
};

class Lexer
{
public:
    void ScanInput(std::istream& input);
    std::vector<Token> GetTokenList();
    void PushToken(std::string& lexem, int row, int pos);
private:
    std::vector<Token> m_tokenList;
    LexerState m_state = LexerState::DEFAULT;
};