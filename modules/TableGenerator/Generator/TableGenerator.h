#pragma once
#include "Grammar.h"
#include "Table.h"

class TableGenerator
{
public:
	explicit TableGenerator(const Grammar& grammar);
	Table Generate();

private:
	const Grammar& m_grammar;
	Table m_table;
	int m_currentRowNumber = 0;

	void InitializeTableFromGrammar();
	void ProcessGrammarRules();
	void GenerateTableRowsForRule(const GrammarRow& rule);
	void GenerateTableRowForSymbol(const std::string& symbol, bool isLastSymbolInRule, const std::vector<std::string>& guideSymbols);

	static void InitializeTableRow(TableRow& row, const std::string& symbol);
	void SetPointerForSymbol(TableRow& row, const std::string& symbol, bool isLastSymbolInRule) const;
	static void SetStackFlag(TableRow& row, const std::string& symbol, bool isLastSymbolInRule);
	void SetGuidingSymbols(TableRow& row, const std::string& symbol, const std::vector<std::string>& guideSymbols) const;
	static void HandleEndSymbol(TableRow& row, const std::string& symbol);

	void SetInitialPointers();
	[[nodiscard]] int CalculatePredictedNumber(int currentIndex) const;
};