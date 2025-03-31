#include "GrammarProcessor.h"
#include "GrammarParser.h"
#include "TableExporter.h"
#include "TableGenerator.h"
#include <fstream>
#include <iostream>

GrammarProcessor::GrammarProcessor(const std::string& inputFileName)
{
	ParseInputFile(inputFileName);
}

void GrammarProcessor::ParseInputFile(const std::string& inputFileName)
{
	std::ifstream input(inputFileName);
	if (!input.is_open())
	{
		throw std::runtime_error("Input file is not opened");
	}

	std::string line;
	while (std::getline(input, line))
	{
		m_grammar.emplace_back(GrammarParser::ParseLine(line));
	}
}

void GrammarProcessor::ProcessGrammar()
{
	TableGenerator generator(m_grammar);
	m_table = generator.Generate();
}

void GrammarProcessor::ExportTable(const std::string& outputFileName) const
{
	TableExporter::ExportToCSV(m_table, outputFileName);
}