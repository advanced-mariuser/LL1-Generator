#pragma once
#include <iostream>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

inline std::string Implode(const std::vector<std::string>& elements, const std::string& delimiter)
{
	if (elements.empty())
	{
		return "";
	}

	return std::accumulate(
		std::next(elements.begin()),
		elements.end(),
		elements[0],
		[delimiter](const std::string& a, const std::string& b) {
			return a + delimiter + b;
		});
}

inline std::string GetDecoratedPointer(const int number)
{
	return number == 0 ? "NULL" : std::to_string(number);
}

inline bool IsNonTerminal(const std::string& token)
{
	return token[0] == '<' && token[token.size() - 1] == '>';
}

inline bool IsEmptySymbol(const std::string& token)
{
	return token == "e";
}

inline bool IsEnd(const std::string& token)
{
	return token == "#";
}

inline std::vector<std::string> FindGuidingSymbolsInGrammarForNonTerminal(const Grammar& grammar, const std::string& token)
{
	if (!IsNonTerminal(token))
	{
		return {};
	}

	std::vector<std::string> result;
	for (const auto& [leftNonTerminal, rightParts, guideSymbols] : grammar)
	{
		if (token == leftNonTerminal)
		{
			result.insert(result.end(), guideSymbols.begin(), guideSymbols.end());
		}
	}
	return result;
}

inline int FindFirstRuleNumber(const std::string& nonTerminal, const Grammar& grammar)
{
	for (int i = 0; i < grammar.size(); i++)
	{
		if (nonTerminal == grammar[i].leftNonTerminal)
		{
			return i + 1;
		}
	}
	return 0;
}