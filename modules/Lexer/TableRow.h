#pragma once

#include <string>
#include <vector>
#include <map>

struct TableRow
{
    int number{};
    std::string nonTerminal{};
    std::vector<std::string> symbols;
    bool isShift{};
    bool isError{};
    int pointer{};
    bool isStack{};
    bool isEnd{};
};