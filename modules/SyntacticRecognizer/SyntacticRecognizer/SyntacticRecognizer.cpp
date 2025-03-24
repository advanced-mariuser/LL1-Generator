#include "SyntacticRecognizer.h"
#include <iostream>
#include <algorithm>

SyntacticRecognizer::SyntacticRecognizer(const std::map<int, TableRow> &table)
        : m_table(table) {}

bool SyntacticRecognizer::Parse(const std::vector<Token>& tokens)
{
    Initialize();

    while (IsStackFilled())
    {
        int currentState = m_stateStack.back();
        m_stateStack.pop_back();

        PrintTrace("Processing state: " + std::to_string(currentState));
        if (!ProcessState(currentState, tokens))
        {
            return false; // Ошибка разбора
        }

        // Если достигнут конец разбора, возвращаем true
        if (IsEnd(currentState))
        {
            PrintTrace("End of parsing reached.");
            return true;
        }
    }

    // Если стек пуст, но конец разбора не достигнут, это ошибка
    PrintTrace("Error: Unexpected end of parsing");
    return false;
}

void SyntacticRecognizer::Initialize()
{
    m_stateStack.push_back(1); // Начинаем с первой строки таблицы
    m_inputPosition = 0;
}

bool SyntacticRecognizer::ProcessState(int currentState, const std::vector<Token>& tokens)
{
    if (IsStateNotFound(currentState))
    {
        PrintTrace("Error: Invalid state " + std::to_string(currentState));
        return false;
    }

    const TableRow &row = m_table.at(currentState);
    PrintTrace("Current state: " + std::to_string(row.number) + ", Non-terminal: " + row.nonTerminal);

    if (m_inputPosition >= tokens.size())
    {
        PrintTrace("Error: Unexpected end of input");
        return false;
    }

    std::string currentSymbol = tokens[m_inputPosition].GetData();
    // Проверяем, принадлежит ли символ направляющим символам
    bool isSymbolValid = IsCurrentStateInSymbols(row, currentSymbol);

    // Если isError == 1 и символ недопустим, выбрасываем ошибку
    if (row.isError && !isSymbolValid)
    {
        PrintTrace("Error: Unexpected symbol '" + currentSymbol + "' at position " + std::to_string(m_inputPosition));
        return false;
    }

    // Если символ допустим, продолжаем разбор
    if (isSymbolValid)
    {
        // Если это сдвиг (shift), обрабатываем символ
        if (row.isShift && !HandleShift(tokens))
        {
            return false;
        }

        // Обрабатываем указатель и стек
        HandlePointerAndStack(row, currentState);
    }
    else
    {
        // Если символ недопустим, но isError == 0, переходим к следующему состоянию
        m_stateStack.push_back(currentState + 1);
    }

    return true;
}

bool SyntacticRecognizer::HandleShift(const std::vector<Token>& tokens)
{
    if (m_inputPosition >= tokens.size())
    {
        PrintTrace("Error: Unexpected end of input");
        return false;
    }

    std::string currentSymbol = tokens[m_inputPosition].GetData();
    PrintTrace("Shift: " + currentSymbol);
    m_inputPosition++;
    return true;
}

void SyntacticRecognizer::HandlePointerAndStack(const TableRow &row, int currentState)
{
    if (row.pointer > 0)
    {
        m_stateStack.push_back(row.pointer);
    } else if (row.pointer == -1)
    {
        int nextState = m_stack.front();
        m_stack.pop_back();
        m_stateStack.push_back(nextState);
    }

    if (row.isStack)
    {
        m_stack.push_back(currentState + 1);
    }
}

void SyntacticRecognizer::PrintTrace(const std::string &message)
{
    std::cout << message << std::endl;
}

bool SyntacticRecognizer::IsStackFilled()
{
    return !m_stateStack.empty();
}

bool SyntacticRecognizer::IsStateNotFound(int currentState)
{
    return m_table.find(currentState) == m_table.end();
}

bool SyntacticRecognizer::IsCurrentStateInSymbols(const TableRow &row, const std::string &currentSymbol)
{
    return std::find(row.symbols.begin(), row.symbols.end(), currentSymbol) != row.symbols.end();
}

bool SyntacticRecognizer::IsEnd(int currentState)
{
    return m_table.at(currentState).isEnd;
}