#include "SyntacticRecognizer/CSVParser/CSVParser.h"
#include "catch2/catch_test_macros.hpp"
#include <filesystem>
#include <fstream>

TEST_CASE("CSVParser unit tests", "[CSVParser]")
{
    const std::string testFilePath = "test_csv_parser_temp.csv";

    SECTION("Test with valid CSV file")
    {
        const std::string csvContent =
                "0;S;A B C;1;0;5;0;0\n"
                "1;A;D E;0;1;-1;1;0\n"
                "2;B;F;1;0;NULL;0;1\n"
                "3;C;G H I;0;0;10;0;0\n";

        std::ofstream outFile(testFilePath);
        outFile << csvContent;
        outFile.close();

        CSVParser parser(testFilePath);
        auto table = parser.GetTable();

        REQUIRE(table.size() == 4);

        SECTION("Verify first row parsing")
        {
            auto row = table[0];
            REQUIRE(row.number == 0);
            REQUIRE(row.nonTerminal == "S");
            REQUIRE(row.guidingSymbols == std::vector<std::string>{"A", "B", "C"});
            REQUIRE(row.isShift == true);
            REQUIRE(row.isError == false);
            REQUIRE(row.pointer == 5);
            REQUIRE(row.isStack == false);
            REQUIRE(row.isEnd == false);
        }

        SECTION("Verify second row parsing")
        {
            auto row = table[1];
            REQUIRE(row.number == 1);
            REQUIRE(row.nonTerminal == "A");
            REQUIRE(row.guidingSymbols == std::vector<std::string>{"D", "E"});
            REQUIRE(row.isShift == false);
            REQUIRE(row.isError == true);
            REQUIRE(row.pointer == -1);  // NULL case
            REQUIRE(row.isStack == true);
            REQUIRE(row.isEnd == false);
        }

        SECTION("Verify third row parsing")
        {
            auto row = table[2];
            REQUIRE(row.number == 2);
            REQUIRE(row.nonTerminal == "B");
            REQUIRE(row.guidingSymbols == std::vector<std::string>{"F"});
            REQUIRE(row.isShift == true);
            REQUIRE(row.isError == false);
            REQUIRE(row.pointer == -1);  // NULL case
            REQUIRE(row.isStack == false);
            REQUIRE(row.isEnd == true);
        }

        SECTION("Verify fourth row parsing")
        {
            auto row = table[3];
            REQUIRE(row.number == 3);
            REQUIRE(row.nonTerminal == "C");
            REQUIRE(row.guidingSymbols == std::vector<std::string>{"G", "H", "I"});
            REQUIRE(row.isShift == false);
            REQUIRE(row.isError == false);
            REQUIRE(row.pointer == 10);
            REQUIRE(row.isStack == false);
            REQUIRE(row.isEnd == false);
        }
    }

    SECTION("Test error handling")
    {
        SECTION("Non-existent file")
        {
            REQUIRE_THROWS_AS(CSVParser("non_existent_file.csv"), std::runtime_error);
        }

//        SECTION("Malformed CSV - missing field")
//        {
//            std::ofstream outFile(testFilePath);
//            outFile << "0;S;A B C;1;0;5;0\n";  // Missing last field
//            outFile.close();
//
//            REQUIRE_THROWS_AS(CSVParser(testFilePath), std::exception);
//        }

        SECTION("Malformed CSV - invalid number")
        {
            std::ofstream outFile(testFilePath);
            outFile << "0;S;A B C;not_a_number;0;5;0;0\n";
            outFile.close();

            REQUIRE_THROWS_AS(CSVParser(testFilePath), std::exception);
        }

//        SECTION("Malformed CSV - empty file")
//        {
//            std::ofstream outFile(testFilePath);
//            outFile.close();
//
//            REQUIRE_THROWS_AS(CSVParser(testFilePath), std::exception);
//        }

        SECTION("Malformed CSV - empty guiding symbols")
        {
            std::ofstream outFile(testFilePath);
            outFile << "0;S;;1;0;5;0;0\n";  // Empty guiding symbols
            outFile.close();

            CSVParser parser(testFilePath);
            auto table = parser.GetTable();
            REQUIRE(table[0].guidingSymbols.empty());
        }
    }

    SECTION("Test edge cases")
    {
//        SECTION("Empty guiding symbols with spaces")
//        {
//            std::ofstream outFile(testFilePath);
//            outFile << "0;S;   ;1;0;5;0;0\n";  // Spaces in guiding symbols
//            outFile.close();
//
//            CSVParser parser(testFilePath);
//            auto table = parser.GetTable();
//            REQUIRE(table[0].guidingSymbols.empty());
//        }

        SECTION("Single guiding symbol")
        {
            std::ofstream outFile(testFilePath);
            outFile << "0;S;SingleSymbol;1;0;5;0;0\n";
            outFile.close();

            CSVParser parser(testFilePath);
            auto table = parser.GetTable();
            REQUIRE(table[0].guidingSymbols == std::vector<std::string>{"SingleSymbol"});
        }

//        SECTION("Multiple spaces between guiding symbols")
//        {
//            std::ofstream outFile(testFilePath);
//            outFile << "0;S;A  B   C;1;0;5;0;0\n";  // Multiple spaces
//            outFile.close();
//
//            CSVParser parser(testFilePath);
//            auto table = parser.GetTable();
//            REQUIRE(table[0].guidingSymbols == std::vector<std::string>{"A", "B", "C"});
//        }
    }

    std::filesystem::remove(testFilePath);
}