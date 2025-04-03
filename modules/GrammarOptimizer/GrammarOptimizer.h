#pragma once

#include "sstream"
#include "Dictionary.h"

namespace GrammarOptimizer
{
	void Optimize(std::istream& input, std::ostream& output)
	{
		auto dictionary = CreateDictionaryFromInput(input);
		dictionary.RemoveLeftRecursion();
 		dictionary.Factorize();

		auto firstStar = dictionary.ComputeFirstStar();
		auto follow = dictionary.ComputeFollow(firstStar);

		for (const auto& nonTerminal : dictionary.GetNonTerminals())
		{
			for (const auto& rule : nonTerminal->GetRules())
			{
				output << nonTerminal->GetValue() << " -> ";
				for (const auto& symbol : rule)
				{
					output << symbol->GetValue();
				}
				output << " | ";
				bool isFirst = true;
				for (const auto& guidingSymbol : firstStar[nonTerminal->GetValue()])
				{
					if (
						rule[0]->GetType() == SYMBOL_TYPE::TERMINAL && rule[0]->GetValue() == guidingSymbol
						)
					{
						if (!isFirst)
						{
							output << " ";
						}
						isFirst = false;
						if (guidingSymbol == "e")
						{
							bool isSecondFirst = true;
							for (const auto& followSymbol : follow[nonTerminal->GetValue()])
							{
								if (!isSecondFirst)
								{
									output << " ";
								}
								isSecondFirst = false;
								output << followSymbol;
							}
							continue;
						}
						output << guidingSymbol;
						continue;
					}
					if (rule[0]->GetType() == SYMBOL_TYPE::NON_TERMINAL)
					{
						if (std::find(firstStar[rule[0]->GetValue()].begin(), firstStar[rule[0]->GetValue()].end(), guidingSymbol) != firstStar[rule[0]->GetValue()].end())
						{
							if (!isFirst)
							{
								output << " ";
							}
							isFirst = false;
							output << guidingSymbol;
						}
					}
				}
				output << std::endl;
			}
		}
	}
}
