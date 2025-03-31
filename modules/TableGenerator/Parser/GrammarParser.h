#pragma once
#include "Grammar.h"
#include <regex>
#include <string>

class GrammarParser
{
public:
	static GrammarRow ParseLine(const std::string& line);

private:
	static void ParseRightParts(GrammarRow& result, const std::string& rightPart);
	static void ParseGuideSymbols(GrammarRow& result, const std::string& guides);
	static std::string RemoveExtraBlanks(const std::string& str);
};