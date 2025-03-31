#include "TableGenerator.h"
#include "Utils.h"
#include <ostream>

TableGenerator::TableGenerator(const Grammar& grammar)
	: m_grammar(grammar)
{
}

Table TableGenerator::Generate()
{
	InitializeTableFromGrammar();
	SetInitialPointers();
	ProcessGrammarRules();
	return m_table;
}

void TableGenerator::InitializeTableFromGrammar()
{
	for (const auto& [leftNonTerminal, rightParts, guideSymbols] : m_grammar)
	{
		m_currentRowNumber++;
		m_table.emplace_back(TableRow{
			m_currentRowNumber,
			leftNonTerminal,
			guideSymbols,
			false,
			true,
			0,
			false,
			false });

		if (m_currentRowNumber > 1 && leftNonTerminal == m_table[m_currentRowNumber - 2].nonTerminal)
		{
			m_table[m_currentRowNumber - 2].isError = false;
		}
	}
}

void TableGenerator::SetInitialPointers()
{
	m_table[0].pointer = static_cast<int>(m_grammar.size()) + 1;

	for (int i = 1; i < m_grammar.size(); i++)
	{
		m_table[i].pointer = static_cast<int>(m_grammar.size()) + CalculatePredictedNumber(i);
	}
}

int TableGenerator::CalculatePredictedNumber(const int currentIndex) const
{
	int predictedNumber = 1;
	for (int i = 1; i <= currentIndex; i++)
	{
		predictedNumber += static_cast<int>(m_grammar[i - 1].rightParts.size());
	}
	return predictedNumber;
}

void TableGenerator::ProcessGrammarRules()
{
	for (const auto& grammar_row : m_grammar)
	{
		GenerateTableRowsForRule(grammar_row);
	}
}

void TableGenerator::GenerateTableRowsForRule(const GrammarRow& rule)
{
	for (int i = 0; i < rule.rightParts.size(); i++)
	{
		m_currentRowNumber++;
		GenerateTableRowForSymbol(rule.rightParts[i], i == rule.rightParts.size() - 1, rule.guideSymbols);
	}
}

void TableGenerator::GenerateTableRowForSymbol(const std::string& symbol, const bool isLastSymbolInRule, const std::vector<std::string>& guideSymbols)
{
	TableRow row;
	row.number = m_currentRowNumber;
	row.nonTerminal = symbol;
	row.isError = true;

	InitializeTableRow(row, symbol);
	SetPointerForSymbol(row, symbol, isLastSymbolInRule);
	SetStackFlag(row, symbol, isLastSymbolInRule);
	SetGuidingSymbols(row, symbol, guideSymbols);
	HandleEndSymbol(row, symbol);

	m_table.emplace_back(row);
}

void TableGenerator::InitializeTableRow(TableRow& row, const std::string& symbol)
{
	row.isShift = !IsNonTerminal(symbol) && !IsEmptySymbol(symbol);
}

void TableGenerator::SetPointerForSymbol(TableRow& row, const std::string& symbol, const bool isLastSymbolInRule) const
{
	if (IsNonTerminal(symbol))
	{
		row.pointer = FindFirstRuleNumber(symbol, m_grammar);
	}
	else if (!IsNonTerminal(symbol))
	{
		row.pointer = !isLastSymbolInRule ? m_currentRowNumber + 1 : 0;
	}
}

void TableGenerator::SetStackFlag(TableRow& row, const std::string& symbol, const bool isLastSymbolInRule)
{
	row.isStack = IsNonTerminal(symbol) && !isLastSymbolInRule;
}

void TableGenerator::SetGuidingSymbols(TableRow& row, const std::string& symbol, const std::vector<std::string>& guideSymbols) const
{
	if (!IsNonTerminal(symbol) && !IsEmptySymbol(symbol))
	{
		row.guidingSymbols.push_back(symbol);
	}
	else
	{
		std::vector<std::string> guidingSymbols;
		if (IsEmptySymbol(symbol))
		{
			guidingSymbols = guideSymbols;
		}
		else
		{
			guidingSymbols = FindGuidingSymbolsInGrammarForNonTerminal(m_grammar, symbol);
		}

		row.guidingSymbols.insert(row.guidingSymbols.end(), guidingSymbols.begin(), guidingSymbols.end());
	}
}

void TableGenerator::HandleEndSymbol(TableRow& row, const std::string& symbol)
{
	if (IsEnd(symbol))
	{
		row.pointer = 0;
		row.isEnd = true;
	}
}