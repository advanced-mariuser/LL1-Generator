#pragma once

#include "TokenType.h"
#include "string"

class Token
{
public:
    Token(TokenType type, std::string  data, int row, int pos)
            : m_type(type),
              m_data(std::move(data)),
              m_row(row),
              m_pos(pos)
    {
    }

    [[nodiscard]] const TokenType& GetType() const
    {
        return m_type;
    }

    [[nodiscard]] const std::string& GetData() const
    {
        return m_data;
    }

    [[nodiscard]] int GetRow() const
    {
        return m_row;
    }

    [[nodiscard]] int GetPos() const
    {
        return m_pos;
    }

private:
    TokenType m_type;
    std::string m_data;
    int m_row;
    int m_pos;
};

inline static std::vector<char> SEPARATORS = {
        '(', ')', '[', ']', '{', '}', ';', ','
};