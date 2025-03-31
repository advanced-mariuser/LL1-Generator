#pragma once
#include <string>
#include <vector>

struct GrammarRow
{
	std::string leftNonTerminal;
	std::vector<std::string> rightParts;
	std::vector<std::string> guideSymbols;
};

using Grammar = std::vector<GrammarRow>;