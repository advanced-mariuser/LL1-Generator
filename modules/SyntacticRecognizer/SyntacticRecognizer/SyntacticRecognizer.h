#ifndef SYNTACTICRECOGNIZER_H
#define SYNTACTICRECOGNIZER_H

#include "Table.h"
#include "Token.h"

class SyntacticRecognizer
{
public:
    explicit SyntacticRecognizer(const std::map<size_t, TableRow>& table, const std::vector<Token>& tokens);

    bool Parse();

private:
    std::map<size_t, TableRow> m_table;
    std::vector<Token> m_tokens;
    std::vector<size_t> m_stack;
    size_t m_currentState = 1;
    size_t m_currentTokenIndex = 0;
    size_t m_inputPosition = 0;

    bool ProcessState(size_t currentState);

    bool HandleShift();

    void HandlePointerAndStack(const TableRow& row, size_t currentState);

    static void PrintTrace(const std::string& message);

    bool IsStateNotFound(size_t currentState);

    static bool IsCurrentStateInSymbols(const TableRow& row, const std::string& currentSymbol);

    bool IsEnd(size_t currentState);
};

#endif // SYNTACTICRECOGNIZER_H