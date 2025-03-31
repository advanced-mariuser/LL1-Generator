#ifndef TOKENSCONNECTOR_H
#define TOKENSCONNECTOR_H

#include "Token.h"
#include <unordered_map>
#include <algorithm>

class TokensConnector
{
private:
    static std::string ToLower(const std::string& str)
    {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        return lowerStr;
    }

public:
    static Token ConvertTokenData(Token token)
    {
        if (token.GetType() == TokenType::IDENTIFIER)
        {
            std::string lowerData = ToLower(token.GetData());
            if (lowerData == "true")
            {
                token.SetData("t");
                return token;
            } else if (lowerData == "false")
            {
                token.SetData("f");
                return token;
            }
        }

        if (token.GetType() == TokenType::LOGICAL)
        {
            std::string lowerData = ToLower(token.GetData());
            if (lowerData == "<=")
            {
                token.SetData("l");
                return token;
            } else if (lowerData == ">=")
            {
                token.SetData("b");
                return token;
            } else if (lowerData == "!=")
            {
                token.SetData("q");
                return token;
            }

        }

        static const std::unordered_map<TokenType, std::string> typeToPrefix = {
                {TokenType::MOD,        "m"},
                {TokenType::DIV,        "d"},
                {TokenType::AND,        "A"},
                {TokenType::OR,         "O"},
                {TokenType::IDENTIFIER, "i"},
                {TokenType::STRING,     "s"},
                {TokenType::NUMBER,     "N"},
                {TokenType::NOT,        "n"},
                {TokenType::SEPARATOR,  "#"},
                {TokenType::ELSE_BLOCK, "J"},
                {TokenType::IF_STATEMENT, "H"},
                {TokenType::THEN, "Z"},
        };

        auto it = typeToPrefix.find(token.GetType());
        if (it != typeToPrefix.end())
        {
            token.SetData(it->second);
            return token;
        }

        return token;
    }

    static std::vector<Token> ConvertTokens(const std::vector<Token> tokens)
    {
        std::vector<Token> converted;
        converted.reserve(tokens.size());

        for (const auto& token: tokens)
        {
            converted.push_back(ConvertTokenData(token));
        }

        return converted;
    }
};

#endif //TOKENSCONNECTOR_H
