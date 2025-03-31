#pragma once
#include "Grammar.h"
#include "Table.h"
#include <string>

class GrammarProcessor
{
public:
	explicit GrammarProcessor(const std::string& inputFileName);
	void ProcessGrammar();
	void ExportTable(const std::string& outputFileName) const;

private:
	Grammar m_grammar;
	Table m_table;

	void ParseInputFile(const std::string& inputFileName);
};