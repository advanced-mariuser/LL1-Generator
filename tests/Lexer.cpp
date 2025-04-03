#include "Lexer/Lexer/Lexer.h"
#include "catch2/catch_test_macros.hpp"
#include <sstream>
#include <vector>
#include <iostream>

TEST_CASE("Lexer comprehensive tests", "[Lexer]")
{
    Lexer lexer;

    std::map<TokenType, std::string> tokenTypeToString = {
            {TokenType::NUMBER,               "NUMBER"},
            {TokenType::VARIABLE_DECLARATION, "VARIABLE_DECLARATION"},
            {TokenType::IF_STATEMENT,         "IF_STATEMENT"},
            {TokenType::THEN,                 "THEN"},
            {TokenType::ELSE_BLOCK,           "ELSE_BLOCK"},
            {TokenType::PLUS,                 "PLUS"},
            {TokenType::MINUS,                "MINUS"},
            {TokenType::DIVISION,             "DIVISION"},
            {TokenType::MULTIPLICATION,       "MULTIPLICATION"},
            {TokenType::RETURN,               "RETURN"},
            {TokenType::FUNCTION,             "FUNCTION"},
            {TokenType::ASSIGNMENT,           "ASSIGNMENT"},
            {TokenType::LOGICAL,              "LOGICAL"},
            {TokenType::FOR_STATEMENT,        "FOR_STATEMENT"},
            {TokenType::IDENTIFIER,           "IDENTIFIER"},
            {TokenType::STRING,               "STRING"},
            {TokenType::COMMENT,              "COMMENT"},
            {TokenType::COMMENT_BLOCK,        "COMMENT_BLOCK"},
            {TokenType::BRACKET,              "BRACKET"},
            {TokenType::CURLY_BRACKET,        "CURLY_BRACKET"},
            {TokenType::SQUARE_BRACKET,       "SQUARE_BRACKET"},
            {TokenType::SEPARATOR,            "SEPARATOR"},
            {TokenType::OR,                   "OR"},
            {TokenType::AND,                  "AND"},
            {TokenType::MOD,                  "MOD"},
            {TokenType::DIV,                  "DIV"},
            {TokenType::NOT,                  "NOT"},
            {TokenType::ERROR,                "ERROR"},
            {TokenType::END,                  "END"}
    };

    SECTION("Empty input")
    {
        std::istringstream input("");
        lexer.ScanInput(input);
        auto tokens = lexer.GetTokenList();

        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].GetType() == TokenType::END);
        REQUIRE(tokens[0].GetData() == "end");
    }

    SECTION("Basic tokens recognition")
    {
        std::istringstream input("var x = 42 + y;");
        lexer.ScanInput(input);
        auto tokens = lexer.GetTokenList();

        REQUIRE(tokens.size() == 8); // including END token

        REQUIRE(tokens[0].GetType() == TokenType::VARIABLE_DECLARATION);
        REQUIRE(tokens[0].GetData() == "var");
        REQUIRE(tokens[0].GetRow() == 1);
        REQUIRE(tokens[0].GetPos() == 1);

        REQUIRE(tokens[1].GetType() == TokenType::IDENTIFIER);
        REQUIRE(tokens[1].GetData() == "x");
        REQUIRE(tokens[1].GetRow() == 1);
        REQUIRE(tokens[1].GetPos() == 5);

        REQUIRE(tokens[2].GetType() == TokenType::ASSIGNMENT);
        REQUIRE(tokens[2].GetData() == "=");

        REQUIRE(tokens[3].GetType() == TokenType::NUMBER);
        REQUIRE(tokens[3].GetData() == "42");

        REQUIRE(tokens[4].GetType() == TokenType::PLUS);
        REQUIRE(tokens[4].GetData() == "+");

        REQUIRE(tokens[5].GetType() == TokenType::IDENTIFIER);
        REQUIRE(tokens[5].GetData() == "y");

        REQUIRE(tokens[6].GetType() == TokenType::SEPARATOR);
        REQUIRE(tokens[6].GetData() == ";");
    }

    SECTION("String literals")
    {
        std::istringstream input(R"(str = "Hello \"world\"!";)");
        lexer.ScanInput(input);
        auto tokens = lexer.GetTokenList();

        REQUIRE(tokens.size() == 6); // including END token
        REQUIRE(tokens[2].GetType() == TokenType::STRING);
        REQUIRE(tokens[2].GetData() == R"("Hello \"world\"!")");
        REQUIRE(tokens[2].GetRow() == 1);
        REQUIRE(tokens[2].GetPos() == 7);
    }

    SECTION("Comments handling")
    {
        SECTION("Line comment")
        {
            std::istringstream input("x = 5; // This is comment\ny = 10;");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens.size() == 9); // including END token
            REQUIRE(tokens[3].GetType() == TokenType::COMMENT);
            REQUIRE(tokens[3].GetData() == "//");
            REQUIRE(tokens[4].GetType() == TokenType::COMMENT);
            REQUIRE(tokens[4].GetData() == "This is comment");
            REQUIRE(tokens[4].GetRow() == 1);
            REQUIRE(tokens[4].GetPos() == 9);
        }

        SECTION("Block comment")
        {
            std::istringstream input("x = /* start */ 5 /* end */;");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens.size() == 8); // including END token
            REQUIRE(tokens[2].GetType() == TokenType::COMMENT_BLOCK);
            REQUIRE(tokens[2].GetData() == "/*");
            REQUIRE(tokens[3].GetType() == TokenType::COMMENT_BLOCK);
            REQUIRE(tokens[3].GetData() == "start");
            REQUIRE(tokens[5].GetType() == TokenType::COMMENT_BLOCK);
            REQUIRE(tokens[5].GetData() == "end");
        }
    }

    SECTION("Complex expressions")
    {
        std::istringstream input("if (x > 5) { return x + 1; } else { return 0; }");
        lexer.ScanInput(input);
        auto tokens = lexer.GetTokenList();

        for (auto token: tokens)
        {
            std::cout << tokenTypeToString[token.GetType()] << std::endl;
        }

        REQUIRE(tokens[0].GetType() == TokenType::IF_STATEMENT);
        REQUIRE(tokens[1].GetType() == TokenType::BRACKET);
        REQUIRE(tokens[2].GetType() == TokenType::IDENTIFIER);
        REQUIRE(tokens[3].GetType() == TokenType::LOGICAL);
        REQUIRE(tokens[4].GetType() == TokenType::NUMBER);
        REQUIRE(tokens[5].GetType() == TokenType::BRACKET);
        REQUIRE(tokens[6].GetType() == TokenType::CURLY_BRACKET);
        REQUIRE(tokens[7].GetType() == TokenType::RETURN);
        REQUIRE(tokens[12].GetType() == TokenType::ELSE_BLOCK);
    }

    SECTION("Position tracking")
    {
        std::istringstream input("x = 5;\ny = 10 + \n 20;");
        lexer.ScanInput(input);
        auto tokens = lexer.GetTokenList();

        REQUIRE(tokens[0].GetRow() == 1);
        REQUIRE(tokens[0].GetPos() == 1);
        REQUIRE(tokens[3].GetRow() == 1);
        REQUIRE(tokens[3].GetPos() == 6);
        REQUIRE(tokens[4].GetRow() == 2);
        REQUIRE(tokens[4].GetPos() == 1);
        REQUIRE(tokens[8].GetRow() == 3);
        REQUIRE(tokens[8].GetPos() == 2);
    }

    SECTION("Edge cases")
    {
        SECTION("Empty string literal")
        {
            std::istringstream input(R"(str = "";)");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::STRING);
            REQUIRE(tokens[2].GetData() == R"("")");
        }

        SECTION("Unclosed string literal")
        {
            std::istringstream input(R"(str = "unclosed;)");
            REQUIRE_NOTHROW(lexer.ScanInput(input));
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::ERROR);
        }

        SECTION("Large number")
        {
            std::istringstream input("x = 9007199254740993;"); // > MAX_SAFE_INTEGER
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::ERROR);
        }

        SECTION("Large number more then 16 chars")
        {
            std::istringstream input("x = 900719925474099363;"); // 18 chars
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::ERROR);
        }

        SECTION("Long identifier")
        {
            std::istringstream input("x = thisIsAVeryLongIdentifierNameThatExceedsTwentyCharacters;");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::ERROR);
        }
    }

    SECTION("Special cases")
    {
        SECTION("Scientific notation")
        {
            std::istringstream input("x = 1.23e+10 + 5E-5;");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[2].GetType() == TokenType::NUMBER);
            REQUIRE(tokens[2].GetData() == "1.23e+10");
            REQUIRE(tokens[4].GetType() == TokenType::NUMBER);
            REQUIRE(tokens[4].GetData() == "5E-5");
        }

        SECTION("Logical operators")
        {
            std::istringstream input("if (x == 5 AND y != 10 OR z <= 20) {}");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[3].GetType() == TokenType::LOGICAL);
            REQUIRE(tokens[3].GetData() == "==");
            REQUIRE(tokens[5].GetType() == TokenType::AND);
            REQUIRE(tokens[5].GetData() == "AND");
            REQUIRE(tokens[7].GetType() == TokenType::LOGICAL);
            REQUIRE(tokens[7].GetData() == "!=");
            REQUIRE(tokens[9].GetType() == TokenType::OR);
            REQUIRE(tokens[9].GetData() == "OR");
            REQUIRE(tokens[11].GetType() == TokenType::LOGICAL);
            REQUIRE(tokens[11].GetData() == "<=");
        }
    }

    SECTION("Test by Nehoroshkova")
    {
        SECTION("First Test")
        {
            std::istringstream input("a.b[a.b] = a.b-(a.b([a.b])[a-b][a*b]);");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[0].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[0].GetData() == "a.b");
            REQUIRE(tokens[1].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[1].GetData() == "[");
            REQUIRE(tokens[2].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[2].GetData() == "a.b");
            REQUIRE(tokens[3].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[3].GetData() == "]");
            REQUIRE(tokens[4].GetType() == TokenType::ASSIGNMENT);
            REQUIRE(tokens[4].GetData() == "=");
            REQUIRE(tokens[5].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[5].GetData() == "a.b");
            REQUIRE(tokens[6].GetType() == TokenType::MINUS);
            REQUIRE(tokens[6].GetData() == "-");
            REQUIRE(tokens[7].GetType() == TokenType::BRACKET);
            REQUIRE(tokens[7].GetData() == "(");
            REQUIRE(tokens[8].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[8].GetData() == "a.b");
            REQUIRE(tokens[9].GetType() == TokenType::BRACKET);
            REQUIRE(tokens[9].GetData() == "(");
            REQUIRE(tokens[10].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[10].GetData() == "[");
            REQUIRE(tokens[11].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[11].GetData() == "a.b");
            REQUIRE(tokens[12].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[12].GetData() == "]");
            REQUIRE(tokens[13].GetType() == TokenType::BRACKET);
            REQUIRE(tokens[13].GetData() == ")");
            REQUIRE(tokens[14].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[14].GetData() == "[");
            REQUIRE(tokens[15].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[15].GetData() == "a");
            REQUIRE(tokens[16].GetType() == TokenType::MINUS);
            REQUIRE(tokens[16].GetData() == "-");
            REQUIRE(tokens[17].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[17].GetData() == "b");
            REQUIRE(tokens[18].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[18].GetData() == "]");
            REQUIRE(tokens[19].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[19].GetData() == "[");
            REQUIRE(tokens[20].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[20].GetData() == "a");
            REQUIRE(tokens[21].GetType() == TokenType::MULTIPLICATION);
            REQUIRE(tokens[21].GetData() == "*");
            REQUIRE(tokens[22].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[22].GetData() == "b");
            REQUIRE(tokens[23].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[23].GetData() == "]");
            REQUIRE(tokens[24].GetType() == TokenType::BRACKET);
            REQUIRE(tokens[24].GetData() == ")");
            REQUIRE(tokens[25].GetType() == TokenType::SEPARATOR);
            REQUIRE(tokens[25].GetData() == ";");
            REQUIRE(tokens[26].GetType() == TokenType::END);
            REQUIRE(tokens[26].GetData() == "end");
        }

        SECTION("Second Test")
        {
            std::istringstream input("if a <= b[a] then a[a][b][5+b] = ab5;");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[0].GetType() == TokenType::IF_STATEMENT);
            REQUIRE(tokens[0].GetData() == "if");
            REQUIRE(tokens[1].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[1].GetData() == "a");
            REQUIRE(tokens[2].GetType() == TokenType::LOGICAL);
            REQUIRE(tokens[2].GetData() == "<=");
            REQUIRE(tokens[3].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[3].GetData() == "b");
            REQUIRE(tokens[4].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[4].GetData() == "[");
            REQUIRE(tokens[5].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[5].GetData() == "a");
            REQUIRE(tokens[6].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[6].GetData() == "]");
            REQUIRE(tokens[7].GetType() == TokenType::THEN);
            REQUIRE(tokens[7].GetData() == "then");
            REQUIRE(tokens[8].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[8].GetData() == "a");
            REQUIRE(tokens[9].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[9].GetData() == "[");
            REQUIRE(tokens[10].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[10].GetData() == "a");
            REQUIRE(tokens[11].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[11].GetData() == "]");
            REQUIRE(tokens[12].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[12].GetData() == "[");
            REQUIRE(tokens[13].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[13].GetData() == "b");
            REQUIRE(tokens[14].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[14].GetData() == "]");
            REQUIRE(tokens[15].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[15].GetData() == "[");
            REQUIRE(tokens[16].GetType() == TokenType::NUMBER);
            REQUIRE(tokens[16].GetData() == "5");
            REQUIRE(tokens[17].GetType() == TokenType::PLUS);
            REQUIRE(tokens[17].GetData() == "+");
            REQUIRE(tokens[18].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[18].GetData() == "b");
            REQUIRE(tokens[19].GetType() == TokenType::SQUARE_BRACKET);
            REQUIRE(tokens[19].GetData() == "]");
            REQUIRE(tokens[20].GetType() == TokenType::ASSIGNMENT);
            REQUIRE(tokens[20].GetData() == "=");
            REQUIRE(tokens[21].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[21].GetData() == "ab5");
            REQUIRE(tokens[22].GetType() == TokenType::SEPARATOR);
            REQUIRE(tokens[22].GetData() == ";");
            REQUIRE(tokens[23].GetType() == TokenType::END);
            REQUIRE(tokens[23].GetData() == "end");
        }

        SECTION("Third Test")
        {
            std::istringstream input("if a > b then a.b.c = 52 else if a52.bc5d = a52*cd52 then");
            lexer.ScanInput(input);
            auto tokens = lexer.GetTokenList();

            REQUIRE(tokens[0].GetType() == TokenType::IF_STATEMENT);
            REQUIRE(tokens[0].GetData() == "if");
            REQUIRE(tokens[1].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[1].GetData() == "a");
            REQUIRE(tokens[2].GetType() == TokenType::LOGICAL);
            REQUIRE(tokens[2].GetData() == ">");
            REQUIRE(tokens[3].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[3].GetData() == "b");
            REQUIRE(tokens[4].GetType() == TokenType::THEN);
            REQUIRE(tokens[4].GetData() == "then");
            REQUIRE(tokens[5].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[5].GetData() == "a.b.c");
            REQUIRE(tokens[6].GetType() == TokenType::ASSIGNMENT);
            REQUIRE(tokens[6].GetData() == "=");
            REQUIRE(tokens[7].GetType() == TokenType::NUMBER);
            REQUIRE(tokens[7].GetData() == "52");
            REQUIRE(tokens[8].GetType() == TokenType::ELSE_BLOCK);
            REQUIRE(tokens[8].GetData() == "else");
            REQUIRE(tokens[9].GetType() == TokenType::IF_STATEMENT);
            REQUIRE(tokens[9].GetData() == "if");
            REQUIRE(tokens[10].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[10].GetData() == "a52.bc5d");
            REQUIRE(tokens[11].GetType() == TokenType::ASSIGNMENT);
            REQUIRE(tokens[11].GetData() == "=");
            REQUIRE(tokens[12].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[12].GetData() == "a52");
            REQUIRE(tokens[13].GetType() == TokenType::MULTIPLICATION);
            REQUIRE(tokens[13].GetData() == "*");
            REQUIRE(tokens[14].GetType() == TokenType::IDENTIFIER);
            REQUIRE(tokens[14].GetData() == "cd52");
            REQUIRE(tokens[15].GetType() == TokenType::THEN);
            REQUIRE(tokens[15].GetData() == "then");
            REQUIRE(tokens[16].GetType() == TokenType::END);
            REQUIRE(tokens[16].GetData() == "end");
        }
    }
}