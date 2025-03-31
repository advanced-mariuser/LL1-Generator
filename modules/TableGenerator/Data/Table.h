#pragma once
#include <string>
#include <vector>

struct TableRow
{
	int number{};
	std::string nonTerminal{};
	std::vector<std::string> guidingSymbols;
	bool isShift{};
	bool isError{};
	int pointer{};
	bool isStack{};
	bool isEnd{};
};

using Table = std::vector<TableRow>;