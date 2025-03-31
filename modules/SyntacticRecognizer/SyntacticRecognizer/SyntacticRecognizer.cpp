#include "SyntacticRecognizer.h"
#include <iostream>
#include <algorithm>

SyntacticRecognizer::SyntacticRecognizer(const std::map<size_t, TableRow>& table,
                                         const std::vector<Token>& tokens)
        : m_table(table), m_tokens(tokens) {}

bool SyntacticRecognizer::Parse()
{
    while (m_currentState > 0 && m_currentTokenIndex < m_tokens.size())
    {
        PrintTrace("Processing state: " + std::to_string(m_currentState));
        if (!ProcessState(m_currentState))
        {
            return false;
        }

        if (IsEnd(m_currentState))
        {
            PrintTrace("End of parsing reached.");
            return true;
        }
    }

    PrintTrace("Error: Unexpected end of parsing");
    return false;
}

bool SyntacticRecognizer::ProcessState(size_t currentState)
{
    if (IsStateNotFound(currentState))
    {
        PrintTrace("Error: Invalid state " + std::to_string(currentState));
        return false;
    }

    const TableRow& row = m_table.at(currentState);
    const Token& currentToken = m_tokens[m_currentTokenIndex];
    PrintTrace("Current state: " + std::to_string(row.number) + ", Non-terminal: " + row.nonTerminal + " Token: " + currentToken.GetData());

    if (m_currentTokenIndex >= m_tokens.size())
    {
        PrintTrace("Error: Unexpected end of input");
        return false;
    }

    std::string currentSymbol = currentToken.GetData();
    // Проверяем входит ли символ направляющим символам
    bool isSymbolValid = IsCurrentStateInSymbols(row, currentSymbol);

    if (row.isError && !isSymbolValid)
    {
        PrintTrace("Error: Unexpected symbol '" + currentSymbol + "' at position " + std::to_string(currentToken.GetPos()) + "\n Expected: ");
        for(auto sym : row.symbols)
        {
            std::cout << sym << " ";
        }
        std::cout << std::endl;
        return false;
    }

    if (isSymbolValid)
    {
        // Если сдвиг обрабатываем символ
        if (row.isShift && !HandleShift())
        {
            return false;
        }

        // Обрабатываем указатель и стек
        HandlePointerAndStack(row, currentState);
    } else
    {
        // Если символ недопустим но нет ошибки то на некст строку
        m_currentState = currentState + 1;
    }

    return true;
}

bool SyntacticRecognizer::HandleShift()
{
    if (m_currentTokenIndex >= m_tokens.size())
    {
        PrintTrace("Error: Unexpected end of input");
        return false;
    }

    std::string currentSymbol = m_tokens[m_currentTokenIndex].GetData();
    PrintTrace("Shift: " + currentSymbol);
    m_currentTokenIndex++;
    return true;
}

void SyntacticRecognizer::HandlePointerAndStack(const TableRow& row, size_t currentState)
{
    if (row.pointer > 0)
    {
        m_currentState = row.pointer;
    } else if (row.pointer == -1)
    {
        if (!m_stack.empty())
        {
            int nextState = m_stack.back();
            m_stack.pop_back();
            m_currentState = nextState;
        }
    }

    if (row.isStack)
    {
        m_stack.push_back(currentState + 1);
    }
}

void SyntacticRecognizer::PrintTrace(const std::string& message)
{
    std::cout << message << std::endl;
}

bool SyntacticRecognizer::IsStateNotFound(size_t currentState)
{
    return m_table.find(currentState) == m_table.end();
}

bool SyntacticRecognizer::IsCurrentStateInSymbols(const TableRow& row, const std::string& currentSymbol)
{
    return std::find(row.symbols.begin(), row.symbols.end(), currentSymbol) != row.symbols.end();
}

bool SyntacticRecognizer::IsEnd(size_t currentState)
{
    return m_table.at(currentState).isEnd;
}