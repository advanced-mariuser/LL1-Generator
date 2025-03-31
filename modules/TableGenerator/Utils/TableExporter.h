#pragma once
#include "Table.h"
#include "Utils.h"
#include <fstream>
#include <string>

class TableExporter
{
public:
	static void ExportToCSV(const Table& table, const std::string& fileName);
};

inline void TableExporter::ExportToCSV(const Table& table, const std::string& fileName)
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file " + fileName);
	}

	for (const auto& [number, nonTerminal, guidingSymbols, isShift, isError, pointer, isStack, isEnd] : table)
	{
		file << number << ";"
			 << nonTerminal << ";"
			 << Implode(guidingSymbols, " ") << ";"
			 << isShift << ";"
			 << isError << ";"
			 << GetDecoratedPointer(pointer) << ";"
			 << isStack << ";"
			 << isEnd << std::endl;
	}
}