#pragma once

#include "unordered_map"
#include "map"
#include "set"
#include "numeric"
#include "Symbol.h"

std::string SALT = "'";

std::string addSuffixBeforeClosingBracket(std::string str, std::string ch)
{
	str.insert(str.size() - 1, ch);
	return str;
}

class Dictionary
{
public:
	void Add(const std::shared_ptr<Symbol>& symbol)
	{
		if (std::find_if(
			m_allSymbols.begin(),
			m_allSymbols.end(),
			[&symbol](const std::shared_ptr<Symbol>& s)
			{ return *s == *symbol; }) != m_allSymbols.end())
		{
			return;
		}
		m_allSymbols.push_back(symbol);

		if (symbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
		{
			auto nonTerminal = std::static_pointer_cast<NonTerminal>(symbol);
			if (nonTerminal)
			{
				m_nonTerminals.push_back(nonTerminal);
			}
			return;
		}

		auto terminal = std::static_pointer_cast<Terminal>(symbol);
		if (terminal)
		{
			m_terminals.push_back(terminal);
		}
	}

	bool CheckValidGrammar()
	{
		std::map<std::string, size_t> nonTerminalIndexMap;
		for (size_t i = 0; i < m_nonTerminals.size(); ++i)
		{
			nonTerminalIndexMap[m_nonTerminals[i]->GetValue()] = i;
		}
		for (const auto& nonTerminal: m_nonTerminals)
		{
			for (const auto& rule: nonTerminal->GetRules())
			{
				auto nonTerminalIndex = nonTerminalIndexMap[nonTerminal->GetValue()];
				auto symbolIndex = nonTerminalIndexMap[rule[0]->GetValue()];

				if (rule[0]->GetType() == SYMBOL_TYPE::NON_TERMINAL &&
					nonTerminalIndex > symbolIndex)
				{
					return false;
				}
			}
		}
		return true;
	}

	void RemoveLeftRecursion()
	{
		std::vector<std::shared_ptr<NonTerminal>> vectorToCreate;
		for (const auto& nonTerminal: m_nonTerminals)
		{
			bool isCreated = false;
			auto newNonTerminal = std::make_shared<NonTerminal>(
				addSuffixBeforeClosingBracket(nonTerminal->GetValue(), SALT));
			std::vector<int> indexToErase;
			auto& rules = nonTerminal->GetRules();
			for (int i = 0; i < rules.size(); i++)
			{
				auto rule = rules[i];
				if (*rule[0] == *nonTerminal)
				{
					if (!isCreated)
					{
						isCreated = true;
						vectorToCreate.push_back(newNonTerminal);
//						Add(newNonTerminal);
					}

					auto newRule = Rule(rule.size() - 1);
					auto newRule2 = Rule(rule.size() - 1);
					std::copy(rule.begin() + 1, rule.end(), newRule.begin());
					std::copy(rule.begin() + 1, rule.end(), newRule2.begin());
					newRule2.push_back(newNonTerminal);

					indexToErase.push_back(i);
					newNonTerminal->AddRule(newRule);
					newNonTerminal->AddRule(newRule2);
				}
			}
			if (!isCreated)
			{
				continue;
			}
			for (int i = indexToErase.size() - 1; i >= 0; i--)
			{
				auto index = indexToErase[i];
				rules.erase(rules.begin() + index);
			}

			auto size = rules.size();
			for (int i = 0; i < size; i++)
			{
				auto rule = rules[i];
				auto newRule = Rule(rule.size());
				std::copy(rule.begin(), rule.end(), newRule.begin());

				newRule.push_back(newNonTerminal);

				rules.push_back(newRule);
			}
		}

		std::for_each(vectorToCreate.begin(), vectorToCreate.end(), [&](const auto& item)
		{
			Add(item);
		});
	}

	[[nodiscard]] const std::vector<std::shared_ptr<NonTerminal>>& GetNonTerminals() const
	{
		return m_nonTerminals;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Terminal>>& GetTerminals() const
	{
		return m_terminals;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Symbol>>& GetAllSymbols() const
	{
		return m_allSymbols;
	}

	std::unordered_map<std::string, std::set<std::string>> ComputeFirstStar() const
	{
		size_t size = m_nonTerminals.size();
		std::vector<std::vector<bool>> adjacencyMatrix(size, std::vector<bool>(size, false));

		std::unordered_map<std::string, size_t> nonTerminalIndexMap;
		for (size_t i = 0; i < m_nonTerminals.size(); ++i)
		{
			nonTerminalIndexMap[m_nonTerminals[i]->GetValue()] = i;
		}

		for (const auto& nonTerminal: m_nonTerminals)
		{
			for (const auto& rule: nonTerminal->GetRules())
			{
				if (!rule.empty())
				{
					const auto& firstSymbol = rule[0];
					size_t from = nonTerminalIndexMap[nonTerminal->GetValue()];
					if (firstSymbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
					{
						size_t to = nonTerminalIndexMap[firstSymbol->GetValue()];
						adjacencyMatrix[from][to] = true;
					}
				}
			}
		}
		for (size_t k = 0; k < size; ++k)
		{
			for (size_t i = 0; i < size; ++i)
			{
				for (size_t j = 0; j < size; ++j)
				{
					adjacencyMatrix[i][j] = adjacencyMatrix[i][j] || (adjacencyMatrix[i][k] && adjacencyMatrix[k][j]);
				}
			}
		}

		std::unordered_map<std::string, std::set<std::string>> firstStar;

		bool isChanged;
		do
		{
			isChanged = false;
			for (const auto& nonTerminal: m_nonTerminals)
			{
				const auto& nonTerminalValue = nonTerminal->GetValue();

				for (const auto& rule: nonTerminal->GetRules())
				{
					if (!rule.empty())
					{
						const auto& firstSymbol = rule[0];
						if (firstSymbol->GetType() == SYMBOL_TYPE::TERMINAL)
						{
							if (!firstStar[nonTerminalValue].count(firstSymbol->GetValue()))
							{
								firstStar[nonTerminalValue].insert(firstSymbol->GetValue());
								isChanged = true;
							}
						}
						else if (firstSymbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
						{
							for (const auto& terminal: firstStar[firstSymbol->GetValue()])
							{
								if (!firstStar[nonTerminalValue].count(terminal))
								{
									firstStar[nonTerminalValue].insert(terminal);
									isChanged = true;
								}
							}
						}
					}
				}
			}
		} while (isChanged);

		return firstStar;
	}

	std::unordered_map<std::string, std::set<std::string>> ComputeFollow(
		const std::unordered_map<std::string, std::set<std::string>>& firstStar) const
	{
		std::unordered_map<std::string, std::set<std::string>> followSets;

		if (m_nonTerminals.empty())
		{
			return followSets;
		}

		bool isChanged;
		do
		{
			isChanged = false;

			for (const auto& nonTerminal : m_nonTerminals)
			{
				const std::string& A = nonTerminal->GetValue();
				for (const auto& rule : nonTerminal->GetRules())
				{
					for (size_t i = 0; i < rule.size(); ++i)
					{
						auto symbol = rule[i];
						if (symbol->GetType() == SYMBOL_TYPE::NON_TERMINAL)
						{
							std::string B = symbol->GetValue();

							// Если после B есть символы
							if (i + 1 < rule.size())
							{
								auto nextSymbol = rule[i + 1];

								// Если следующий символ - терминал, добавляем его в FOLLOW(B)
								if (nextSymbol->GetType() == SYMBOL_TYPE::TERMINAL)
								{
									if (followSets[B].insert(nextSymbol->GetValue()).second)
										isChanged = true;
								}
									// Если следующий символ - нетерминал, добавляем FIRST(nextSymbol) в FOLLOW(B)
								else
								{
									for (const auto& terminal : firstStar.at(nextSymbol->GetValue()))
									{
										if (terminal != "e" && followSets[B].insert(terminal).second)
											isChanged = true;
									}

									// Если FIRST(nextSymbol) содержит ε, добавляем FOLLOW(A) в FOLLOW(B)
									if (firstStar.at(nextSymbol->GetValue()).count("e"))
									{
										for (const auto& terminal : followSets[A])
										{
											if (followSets[B].insert(terminal).second)
												isChanged = true;
										}
									}
								}
							}
							else // Если B последний в правиле, добавляем FOLLOW(A) в FOLLOW(B)
							{
								for (const auto& terminal : followSets[A])
								{
									if (followSets[B].insert(terminal).second)
										isChanged = true;
								}
							}
						}
					}
				}
			}
		} while (isChanged);

		return followSets;
	}

	void Factorize()
	{
		bool globalChanged;
		std::vector<std::shared_ptr<NonTerminal>> nonTerminalsToAdd;
		do
		{
			globalChanged = false;
			for (auto& nonTerminal: m_nonTerminals)
			{
				bool localChanged;
				do
				{
					localChanged = false;
					auto& rules = nonTerminal->GetRules();
					std::unordered_map<std::string, std::vector<Rule>> groups;

					for (const auto& rule: rules)
					{
						if (rule.empty()) continue;
						auto firstSymbol = rule[0];
						groups[firstSymbol->GetValue()].push_back(rule);
					}

					for (auto it = groups.begin(); it != groups.end();)
					{
						auto& group = it->second;
						if (group.size() < 2)
						{
							++it;
							continue;
						}

						size_t prefixLength = 0;
						size_t minLen = std::numeric_limits<size_t>::max();
						for (const auto& r: group)
						{
							if (r.size() < minLen) minLen = r.size();
						}

						for (size_t i = 0; i < minLen; ++i)
						{
							auto& symbol = group[0][i];
							bool allSame = true;
							for (const auto& r: group)
							{
								if (i >= r.size() || *r[i] != *symbol)
								{
									allSame = false;
									break;
								}
							}
							if (allSame)
							{
								prefixLength = i + 1;
							}
							else
							{
								break;
							}
						}

						if (prefixLength == 0)
						{
							++it;
							continue;
						}

						std::string baseName = addSuffixBeforeClosingBracket(nonTerminal->GetValue(), SALT);
						std::string newName = baseName;
						int counter = 1;
						while (true)
						{
							auto exists = std::find_if(m_allSymbols.begin(), m_allSymbols.end(),
								[&newName](const auto& s)
								{
									return s->GetType() == SYMBOL_TYPE::NON_TERMINAL &&
										   s->GetValue() == newName;
								}) != m_allSymbols.end();
							if (!exists) break;
							newName = addSuffixBeforeClosingBracket(baseName, std::to_string(counter++));
						}

						auto newNonTerminal = std::make_shared<NonTerminal>(newName);
						nonTerminalsToAdd.push_back(newNonTerminal);

						Rule newRule;
						for (size_t i = 0; i < prefixLength; ++i)
						{
							newRule.push_back(group[0][i]);
						}
						newRule.push_back(newNonTerminal);
						nonTerminal->AddRule(newRule);

						for (const auto& oldRule: group)
						{
							Rule suffix;
							for (size_t i = prefixLength; i < oldRule.size(); ++i)
							{
								suffix.push_back(oldRule[i]);
							}
							if (suffix.empty())
							{
								suffix.push_back(std::make_shared<Terminal>("e"));
							}
							newNonTerminal->AddRule(suffix);
						}

						for (auto itRule = rules.begin(); itRule != rules.end();)
						{
							if (std::find(group.begin(), group.end(), *itRule) != group.end())
							{
								itRule = rules.erase(itRule);
							}
							else
							{
								++itRule;
							}
						}

						localChanged = true;
						globalChanged = true;
						it = groups.erase(it);
					}
				} while (localChanged);
			}
		} while (globalChanged);

		for (const auto& nonTerminal: nonTerminalsToAdd)
		{
			Add(nonTerminal);
		}
	}

private:
	std::vector<std::shared_ptr<NonTerminal>> m_nonTerminals;
	std::vector<std::shared_ptr<Terminal>> m_terminals;
	std::vector<std::shared_ptr<Symbol>> m_allSymbols;
};

Dictionary CreateDictionaryFromInput(std::istream& input)
{
	Dictionary dictionary;
	std::string row;

	std::regex ruleRegex(R"((<[^>]+>)\s*->\s*(.+))");
	std::smatch matches;

	while (std::getline(input, row))
	{
		if (std::regex_match(row, matches, ruleRegex))
		{
			std::string left = matches[1].str();
			std::string right = matches[2].str();

			auto nonTerminalIt = std::find_if(
				dictionary.GetNonTerminals().begin(),
				dictionary.GetNonTerminals().end(),
				[left](const std::shared_ptr<NonTerminal>& nonTerminal)
				{ return nonTerminal->GetValue() == left; }
			);

			auto nonTerminalPtr = std::make_shared<NonTerminal>(left);
			if (nonTerminalIt == dictionary.GetNonTerminals().end())
			{
				nonTerminalPtr = std::make_shared<NonTerminal>(left);
				dictionary.Add(nonTerminalPtr);
			}
			else
			{
				nonTerminalPtr = *nonTerminalIt;
			}

			std::regex symbolRegex(R"(<[^>]*>?|[^<])");
			std::vector<std::shared_ptr<Symbol>> ruleSymbols;

			for (std::sregex_iterator it(right.begin(), right.end(), symbolRegex), end; it != end; ++it)
			{
				std::string symbol = it->str();
				if (symbol.front() == '<' && symbol.back() == '>')
				{
					auto nonTerminal = std::make_shared<NonTerminal>(symbol);
					ruleSymbols.push_back(nonTerminal);
				}
				else
				{
					auto terminal = std::make_shared<Terminal>(symbol);
					ruleSymbols.push_back(terminal);
					dictionary.Add(terminal);
				}
			}

			Rule rule(ruleSymbols.begin(), ruleSymbols.end());
			nonTerminalPtr->AddRule(rule);
		}
	}

	return dictionary;
}