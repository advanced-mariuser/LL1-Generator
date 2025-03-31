#include "GrammarParser.h"

const std::string FULL_RULE_REGEX = R"(\s*(<[^>]+>)\s*->\s*([^|]+?)\s*\|\s*([^\s]+(?:\s+[^\s]+)*)\s*)";
const std::string RIGHT_PART_OF_RULE_REGEX = R"(\s*(<[^>]+>|[^<>\s]+)\s*)";
const std::string GUIDE_SYMBOLS_REGEX = R"(\s*([^\s]+)\s*)";
const std::string REMOVE_EXTRA_BLANKS_REGEX = R"(^\s+|\s+$)";

GrammarRow GrammarParser::ParseLine(const std::string& line)
{
	GrammarRow result;
	const std::regex pattern(FULL_RULE_REGEX);
	std::smatch matches;

	if (std::regex_match(line, matches, pattern))
	{
		result.leftNonTerminal = matches[1].str();

		std::string rightPart = matches[2].str();
		rightPart = RemoveExtraBlanks(rightPart);
		ParseRightParts(result, rightPart);

		std::string guides = matches[3].str();
		guides = RemoveExtraBlanks(guides);
		ParseGuideSymbols(result, guides);
	}

	return result;
}

void GrammarParser::ParseRightParts(GrammarRow& result, const std::string& rightPart)
{
	const std::regex rightPattern(RIGHT_PART_OF_RULE_REGEX);
	const auto rightBegin = std::sregex_iterator(rightPart.begin(), rightPart.end(), rightPattern);
	const auto rightEnd = std::sregex_iterator();

	for (auto i = rightBegin; i != rightEnd; ++i)
	{
		std::string token = i->str();
		token = RemoveExtraBlanks(token);
		if (!token.empty())
		{
			result.rightParts.push_back(token);
		}
	}
}

void GrammarParser::ParseGuideSymbols(GrammarRow& result, const std::string& guides)
{
	const std::regex guidePattern(GUIDE_SYMBOLS_REGEX);
	const auto guidesBegin = std::sregex_iterator(guides.begin(), guides.end(), guidePattern);
	const auto guidesEnd = std::sregex_iterator();

	for (auto i = guidesBegin; i != guidesEnd; ++i)
	{
		std::string symbol = i->str();
		symbol = RemoveExtraBlanks(symbol);
		if (!symbol.empty())
		{
			result.guideSymbols.push_back(symbol);
		}
	}
}

std::string GrammarParser::RemoveExtraBlanks(const std::string& str)
{
	return std::regex_replace(str, std::regex(REMOVE_EXTRA_BLANKS_REGEX), "");
}
