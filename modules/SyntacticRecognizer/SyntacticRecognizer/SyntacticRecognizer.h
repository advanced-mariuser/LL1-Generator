#ifndef SYNTACTICRECOGNIZER_H
#define SYNTACTICRECOGNIZER_H

#include "TableRow.h"
#include "Token/Token.h"

class SyntacticRecognizer
{
public:
    explicit SyntacticRecognizer(const std::map<int, TableRow> &table);

    bool Parse(const std::vector<Token>& tokens);

private:
    std::map<int, TableRow> m_table;
    std::vector<int> m_stack;
    std::vector<int> m_stateStack;
    size_t m_inputPosition{};

    void Initialize();

    bool ProcessState(int currentState, const std::vector<Token>& tokens);

    bool HandleShift(const std::vector<Token>& tokens);

    void HandlePointerAndStack(const TableRow &row, int currentState);

    static void PrintTrace(const std::string &message) ;

    bool IsStackFilled();

    bool IsStateNotFound(int currentState);

    static bool IsCurrentStateInSymbols(const TableRow &row, const std::string &currentSymbol);

    bool IsEnd(int currentState);
};

#endif // SYNTACTICRECOGNIZER_H