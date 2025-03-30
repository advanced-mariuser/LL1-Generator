#ifndef SYNTACTICRECOGNIZER_H
#define SYNTACTICRECOGNIZER_H

#include "TableRow.h"

class SyntacticRecognizer
{
public:
    explicit SyntacticRecognizer(const std::map<size_t, TableRow>& table);

    bool Parse(const std::string& input);

private:
    std::map<size_t, TableRow> m_table;
    std::vector<size_t> m_stack;
    size_t m_currentState = 1;
    size_t m_inputPosition = 0;

    bool ProcessState(size_t currentState, const std::string& input);

    bool HandleShift(const std::string& input);

    void HandlePointerAndStack(const TableRow& row, size_t currentState);

    static void PrintTrace(const std::string& message);

    bool IsStateNotFound(size_t currentState);

    static bool IsCurrentStateInSymbols(const TableRow& row, const std::string& currentSymbol);

    bool IsEnd(int currentState);
};

#endif // SYNTACTICRECOGNIZER_H