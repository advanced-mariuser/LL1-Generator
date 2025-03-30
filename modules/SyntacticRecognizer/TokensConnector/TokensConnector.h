#ifndef TOKENSCONNECTOR_H
#define TOKENSCONNECTOR_H

#include "Token.h"
#include <unordered_map>

class TokensConnector
{
public:
    static Token ConvertTokenData(Token token)
    {
        static const std::unordered_map<TokenType, std::string> typeToPrefix = {
                {TokenType::MOD,    "m"},
                {TokenType::DIV,    "d"},
                {TokenType::AND,    "A"},
                {TokenType::OR,     "O"},
                {TokenType::IDENTIFIER,     "i"},
                {TokenType::STRING, "s"},
//                {TokenType::FALSE,  "f"},
//                {TokenType::TRUE,   "t"},
                {TokenType::NUMBER, "N"},
                {TokenType::NOT,    "n"},
                {TokenType::END, "#"}
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
