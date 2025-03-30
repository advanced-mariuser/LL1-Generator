#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

CSVParser::CSVParser(const std::string &csvFilePath)
{
    ParseCSV(csvFilePath);
}

void CSVParser::ParseCSV(const std::string &csvFilePath)
{
    std::ifstream file(csvFilePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + csvFilePath);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;
        TableRow row;

        std::getline(ss, token, ';');
        row.number = std::stoi(token);

        std::getline(ss, row.nonTerminal, ';');

        std::getline(ss, token, ';');
        std::stringstream symbolsStream(token);
        std::string symbol;
        while (std::getline(symbolsStream, symbol, ' '))
        {
            row.symbols.push_back(symbol);
        }

        std::getline(ss, token, ';');
        row.isShift = std::stoi(token);

        std::getline(ss, token, ';');
        row.isError = std::stoi(token);

        std::getline(ss, token, ';');
        row.pointer = ParsePointer(token);

        std::getline(ss, token, ';');
        row.isStack = std::stoi(token);

        std::getline(ss, token, ';');
        row.isEnd = std::stoi(token);

        m_table[row.number] = row;
    }
}

int CSVParser::ParsePointer(const std::string &token)
{
    if (token == "NULL")
    {
        return -1;
    }
    return std::stoi(token);
}

std::map<size_t, TableRow> CSVParser::GetTable() const
{
    return m_table;
}