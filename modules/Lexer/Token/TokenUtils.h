#pragma once

#include "Token.h"

inline static constexpr const long double MAX_SAFE_INTEGER = 9007199254740992.0L;

inline Token CreateToken(const std::string& data, int row, int pos)
{
    std::smatch matches;
    for (const auto& tokenType : tokenTypeToRegexMap)
    {
        if (std::regex_search(data, matches, tokenType.second))
        {
            if (tokenType.first == TokenType::NUMBER)
            {
                try
                {
                    if (data.length() > 16)
                    {
                        throw;
                    }

                    long double number = std::stold(data);
                    if (number > MAX_SAFE_INTEGER)
                    {
                        throw;
                    }
                }
                catch (const std::exception& e)
                {
                    return {TokenType::ERROR, data, row, pos};
                }
            }
            else if (tokenType.first == TokenType::IDENTIFIER && data.size() > 20)
            {
                return {TokenType::ERROR, data, row, pos};
            }
            return {tokenType.first, matches.str(0), row, pos};
        }
    }

    return {TokenType::ERROR, data, row, pos};
}

inline bool IsSeparator(char ch)
{
    return isspace(ch) || std::find(SEPARATORS.begin(), SEPARATORS.end(), ch) != SEPARATORS.end();
}

inline bool IsOperation(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '=' || ch == '/' || ch == '!' || ch == '<' || ch == '>';
}

inline bool IsQuote(int ch)
{
    return ch == '"';
}