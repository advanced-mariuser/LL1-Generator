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

        // Парсинг номера строки
        std::getline(ss, token, ';');
        row.number = std::stoi(token);

        // Парсинг нетерминала
        std::getline(ss, row.nonTerminal, ';');

        // Парсинг направляющих символов
        std::getline(ss, token, ';');
        std::stringstream symbolsStream(token);
        std::string symbol;
        while (std::getline(symbolsStream, symbol, ','))
        {
            row.symbols.push_back(symbol);
        }

        // Парсинг isShift
        std::getline(ss, token, ';');
        row.isShift = std::stoi(token);

        // Парсинг isError
        std::getline(ss, token, ';');
        row.isError = std::stoi(token);

        // Парсинг pointer с обработкой NULL
        std::getline(ss, token, ';');
        row.pointer = ParsePointer(token);

        // Парсинг isStack
        std::getline(ss, token, ';');
        row.isStack = std::stoi(token);

        // Парсинг isEnd
        std::getline(ss, token, ';');
        row.isEnd = std::stoi(token);

        m_table[row.number] = row;
    }
}

int CSVParser::ParsePointer(const std::string &token)
{
    if (token == "NULL")
    {
        return -1; // Используем -1 для обозначения NULL
    }
    return std::stoi(token);
}

std::map<int, TableRow> CSVParser::GetTable() const
{
    return m_table;
}