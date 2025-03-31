#include "TableGenerator/Generator/TableGenerator.h"
#include "TableGenerator/Parser/GrammarParser.h"
#include "TableGenerator/Processor/GrammarProcessor.h"
#include "TableGenerator/Utils/TableExporter.h"
#include "TableGenerator/Utils/Utils.h"
#include "catch2/catch_test_macros.hpp"
#include <fstream>

namespace
{
Grammar CreateSimpleGrammar()
{
	return {
		{ "<S>", { "<A>", "a" }, { "a", "b" } },
		{ "<A>", { "b" }, { "b" } }
	};
}

Grammar CreateGrammarWithEmptySymbol()
{
	return {
		{ "<S>", { "<A>", "a" }, { "a", "b" } },
		{ "<A>", { "e" }, { "b" } }
	};
}

Grammar CreateGrammarWithEndSymbol()
{
	return {
		{ "<S>", { "<A>", "#" }, { "a", "b" } },
		{ "<A>", { "b" }, { "b" } }
	};
}

void CreateTestFile(const std::string& filename, const std::string& content)
{
	std::ofstream file(filename);
	file << content;
	file.close();
}
} // namespace

TEST_CASE("GrammarParser tests", "[GrammarParser]")
{
	SECTION("Parse valid grammar line")
	{
		std::string line = "<S> -> <A> a | a b";
		GrammarRow result = GrammarParser::ParseLine(line);

		REQUIRE(result.leftNonTerminal == "<S>");
		REQUIRE(result.rightParts.size() == 2);
		REQUIRE(result.rightParts[0] == "<A>");
		REQUIRE(result.rightParts[1] == "a");
		REQUIRE(result.guideSymbols.size() == 2);
		REQUIRE(result.guideSymbols[0] == "a");
		REQUIRE(result.guideSymbols[1] == "b");
	}

	SECTION("Parse line with multiple guide symbols")
	{
		std::string line = "<A> -> b c | x y z";
		GrammarRow result = GrammarParser::ParseLine(line);

		REQUIRE(result.leftNonTerminal == "<A>");
		REQUIRE(result.rightParts.size() == 2);
		REQUIRE(result.rightParts[0] == "b");
		REQUIRE(result.rightParts[1] == "c");
		REQUIRE(result.guideSymbols.size() == 3);
		REQUIRE(result.guideSymbols[0] == "x");
		REQUIRE(result.guideSymbols[1] == "y");
		REQUIRE(result.guideSymbols[2] == "z");
	}

	SECTION("Parse line with empty right part")
	{
		std::string line = "<A> -> e | x";
		GrammarRow result = GrammarParser::ParseLine(line);

		REQUIRE(result.leftNonTerminal == "<A>");
		REQUIRE(result.rightParts.size() == 1);
		REQUIRE(result.rightParts[0] == "e");
		REQUIRE(result.guideSymbols.size() == 1);
		REQUIRE(result.guideSymbols[0] == "x");
	}

	SECTION("Parse invalid line returns empty GrammarRow")
	{
		std::string line = "invalid line format";
		GrammarRow result = GrammarParser::ParseLine(line);

		REQUIRE(result.leftNonTerminal.empty());
		REQUIRE(result.rightParts.empty());
		REQUIRE(result.guideSymbols.empty());
	}

	SECTION("Parse line with extra whitespace")
	{
		std::string line = "  <S>   ->   <A>   $   |   a   b   ";
		GrammarRow result = GrammarParser::ParseLine(line);

		REQUIRE(result.leftNonTerminal == "<S>");
		REQUIRE(result.rightParts.size() == 2);
		REQUIRE(result.rightParts[0] == "<A>");
		REQUIRE(result.rightParts[1] == "$");
		REQUIRE(result.guideSymbols.size() == 2);
		REQUIRE(result.guideSymbols[0] == "a");
		REQUIRE(result.guideSymbols[1] == "b");
	}
}

TEST_CASE("Utils function tests", "[Utils]")
{
	SECTION("Implode with empty vector")
	{
		std::vector<std::string> empty;
		REQUIRE(Implode(empty, ",").empty());
	}

	SECTION("Implode with single element")
	{
		std::vector<std::string> single = { "hello" };
		REQUIRE(Implode(single, ",") == "hello");
	}

	SECTION("Implode with multiple elements")
	{
		std::vector<std::string> multiple = { "a", "b", "c" };
		REQUIRE(Implode(multiple, ",") == "a,b,c");
		REQUIRE(Implode(multiple, " ") == "a b c");
	}

	SECTION("GetDecoratedPointer")
	{
		REQUIRE(GetDecoratedPointer(0) == "NULL");
		REQUIRE(GetDecoratedPointer(42) == "42");
	}

	SECTION("IsNonTerminal")
	{
		REQUIRE(IsNonTerminal("<S>") == true);
		REQUIRE(IsNonTerminal("<A>") == true);
		REQUIRE(IsNonTerminal("a") == false);
		REQUIRE(IsNonTerminal("") == false);
		REQUIRE(IsNonTerminal("<unclosed") == false);
		REQUIRE(IsNonTerminal("unopened>") == false);
	}

	SECTION("IsEmptySymbol")
	{
		REQUIRE(IsEmptySymbol("e") == true);
		REQUIRE(IsEmptySymbol("") == false);
		REQUIRE(IsEmptySymbol("empty") == false);
		REQUIRE(IsEmptySymbol("E") == false);
	}

	SECTION("IsEnd")
	{
		REQUIRE(IsEnd("#") == true);
		REQUIRE(IsEnd("") == false);
		REQUIRE(IsEnd("hash") == false);
	}

	SECTION("FindGuidingSymbolsInGrammarForNonTerminal")
	{
		Grammar grammar = {
			{ "<S>", { "<A>" }, { "a", "b" } },
			{ "<A>", { "b" }, { "b" } }
		};

		auto symbols = FindGuidingSymbolsInGrammarForNonTerminal(grammar, "<S>");
		REQUIRE(symbols.size() == 2);
		REQUIRE(symbols[0] == "a");
		REQUIRE(symbols[1] == "b");

		symbols = FindGuidingSymbolsInGrammarForNonTerminal(grammar, "<A>");
		REQUIRE(symbols.size() == 1);
		REQUIRE(symbols[0] == "b");

		symbols = FindGuidingSymbolsInGrammarForNonTerminal(grammar, "<B>");
		REQUIRE(symbols.empty());

		symbols = FindGuidingSymbolsInGrammarForNonTerminal(grammar, "b");
		REQUIRE(symbols.empty());
	}

	SECTION("FindFirstRuleNumber")
	{
		Grammar grammar = {
			{ "<S>", { "<A>" }, { "a" } },
			{ "<A>", { "b" }, { "b" } },
			{ "<A>", { "c" }, { "c" } }
		};

		REQUIRE(FindFirstRuleNumber("<S>", grammar) == 1);
		REQUIRE(FindFirstRuleNumber("<A>", grammar) == 2);
		REQUIRE(FindFirstRuleNumber("<B>", grammar) == 0);
		REQUIRE(FindFirstRuleNumber("b", grammar) == 0);
	}
}

TEST_CASE("TableGenerator tests", "[TableGenerator]")
{
	SECTION("InitializeTableFromGrammar with simple grammar")
	{
		Grammar grammar = CreateSimpleGrammar();
		TableGenerator generator(grammar);

		auto table = generator.Generate();
		REQUIRE(table.size() == 5);

		REQUIRE(table[0].number == 1);
		REQUIRE(table[0].nonTerminal == "<S>");
		REQUIRE(table[0].isError == true);

		REQUIRE(table[1].number == 2);
		REQUIRE(table[1].nonTerminal == "<A>");
		REQUIRE(table[1].isError == true);
	}

	SECTION("Generate table with empty symbol")
	{
		Grammar grammar = CreateGrammarWithEmptySymbol();
		TableGenerator generator(grammar);

		auto table = generator.Generate();
		REQUIRE(table.size() == 5);

		REQUIRE(table[4].nonTerminal == "e");
		REQUIRE(table[4].isShift == false);
	}

	SECTION("Generate table with end symbol")
	{
		Grammar grammar = CreateGrammarWithEndSymbol();
		TableGenerator generator(grammar);

		auto table = generator.Generate();
		REQUIRE(table.size() == 5);

		REQUIRE(table[3].nonTerminal == "#");
		REQUIRE(table[3].isEnd == true);
		REQUIRE(table[3].pointer == 0);
	}

	SECTION("Pointers are set correctly through Generate()")
	{
		Grammar grammar = {
			{ "<S>", { "<A>", "<B>", "#" }, { "a", "c" } },
			{ "<A>", { "a" }, { "a" } },
			{ "<A>", { "c", "<A>" }, { "c" } },
			{ "<B>", { "b", "<A>" }, { "b" } }
		};

		TableGenerator generator(grammar);
		auto table = generator.Generate();

		REQUIRE(table[1].nonTerminal == "<A>");
		REQUIRE(table[1].pointer == 8);

		REQUIRE(table[2].nonTerminal == "<A>");
		REQUIRE(table[2].pointer == 9);

		REQUIRE(table[3].nonTerminal == "<B>");
		REQUIRE(table[3].pointer == 11);

		REQUIRE(table[4].nonTerminal == "<A>");
		REQUIRE(table[4].pointer == 2);

		REQUIRE(table[5].nonTerminal == "<B>");
		REQUIRE(table[5].pointer == 4);

		REQUIRE(table[6].nonTerminal == "#");
		REQUIRE(table[6].pointer == 0);
	}

	SECTION("Stack flags are set correctly through Generate()")
	{
		Grammar grammar = {
			{ "<S>", { "<A>", "<B>", "#" }, { "a", "c" } },
			{ "<A>", { "a" }, { "a" } },
			{ "<A>", { "c", "<A>" }, { "c" } },
			{ "<B>", { "b", "<A>" }, { "b" } }
		};

		TableGenerator generator(grammar);
		auto table = generator.Generate();

		REQUIRE(table[1].nonTerminal == "<A>");
		REQUIRE(table[1].isStack == false);

		REQUIRE(table[2].nonTerminal == "<A>");
		REQUIRE(table[2].isStack == false);

		REQUIRE(table[3].nonTerminal == "<B>");
		REQUIRE(table[3].isStack == false);

		REQUIRE(table[4].nonTerminal == "<A>");
		REQUIRE(table[4].isStack == true);

		REQUIRE(table[5].nonTerminal == "<B>");
		REQUIRE(table[5].isStack == true);
	}

	SECTION("Generation handles end symbol correctly")
	{
		Grammar grammar = {
			{ "<S>", { "#" }, { "a" } }
		};
		TableGenerator generator(grammar);
		auto table = generator.Generate();

		REQUIRE(table.size() >= 2);
		REQUIRE(table[1].nonTerminal == "#");
		REQUIRE(table[1].isEnd == true);
		REQUIRE(table[1].pointer == 0);

		Grammar grammar2 = {
			{ "<S>", { "a" }, { "a" } }
		};
		TableGenerator generator2(grammar2);
		auto table2 = generator2.Generate();
		REQUIRE(table2[1].isEnd == false);
	}
}

TEST_CASE("GrammarProcessor integration tests", "[GrammarProcessor]")
{
	SECTION("Process simple grammar from file")
	{
		const std::string testFile = "test_grammar.txt";
		CreateTestFile(testFile,
			"<S> -> <A> a | a b\n"
			"<A> -> b | b\n");

		GrammarProcessor processor(testFile);
		processor.ProcessGrammar();

		REQUIRE_NOTHROW(processor.ExportTable("test_output.csv"));

		std::remove(testFile.c_str());
		std::remove("test_output.csv");
	}

	SECTION("Process invalid file throws")
	{
		REQUIRE_THROWS_AS(GrammarProcessor("nonexistent_file.txt"), std::runtime_error);
	}

	SECTION("ExportTable creates valid file")
	{
		const std::string testFile = "test_grammar.txt";
		CreateTestFile(testFile,
			"<S> -> <A> a | a b\n"
			"<A> -> b | b\n");

		GrammarProcessor processor(testFile);
		processor.ProcessGrammar();
		processor.ExportTable("test_output.csv");

		std::ifstream output("test_output.csv");
		REQUIRE(output.good());
		std::string line;
		std::getline(output, line);
		REQUIRE_FALSE(line.empty());

		output.close();
		std::remove(testFile.c_str());
		std::remove("test_output.csv");
	}
}

TEST_CASE("TableExporter tests", "[TableExporter]")
{
	SECTION("Export empty table")
	{
		Table emptyTable;
		REQUIRE_NOTHROW(TableExporter::ExportToCSV(emptyTable, "empty.csv"));

		std::ifstream output("empty.csv");
		REQUIRE(output.good());
		REQUIRE(output.peek() == std::ifstream::traits_type::eof());

		output.close();
		std::remove("empty.csv");
	}

	SECTION("Export simple table")
	{
		Table table = {
			{ 1, "<S>", { "a", "b" }, false, false, 2, true, false },
			{ 2, "a", { "a" }, true, false, 0, false, false }
		};

		REQUIRE_NOTHROW(TableExporter::ExportToCSV(table, "simple.csv"));

		std::ifstream output("simple.csv");
		REQUIRE(output.good());

		std::string line1, line2;
		std::getline(output, line1);
		std::getline(output, line2);

		REQUIRE(line1 == "1;<S>;a b;0;0;2;1;0");
		REQUIRE(line2 == "2;a;a;1;0;NULL;0;0");

		output.close();
		std::remove("simple.csv");
	}

	SECTION("Export to invalid path throws")
	{
		Table table = { { 1, "<S>", {}, false, false, 0, false, false } };
		REQUIRE_THROWS_AS(TableExporter::ExportToCSV(table, "/invalid/path/table.csv"), std::runtime_error);
	}
}

TEST_CASE("Test common grammar")
{
	const std::string testFile = "test_grammar.txt";
	CreateTestFile(testFile,
		"<S> -> <A><B># | a c\n"
		"<A> -> a | a\n"
		"<A> -> c<A> | c\n"
		"<B> -> b<A> | b\n");

	GrammarProcessor processor(testFile);
	processor.ProcessGrammar();
	processor.ExportTable("test_output.csv");

	std::ifstream output("test_output.csv");
	REQUIRE(output.good());

	std::vector<std::string> expectedLines = {
		"1;<S>;a c;0;1;5;0;0",
		"2;<A>;a;0;0;8;0;0",
		"3;<A>;c;0;1;9;0;0",
		"4;<B>;b;0;1;11;0;0",
		"5;<A>;a c;0;1;2;1;0",
		"6;<B>;b;0;1;4;1;0",
		"7;#;#;1;1;NULL;0;1",
		"8;a;a;1;1;NULL;0;0",
		"9;c;c;1;1;10;0;0",
		"10;<A>;a c;0;1;2;0;0",
		"11;b;b;1;1;12;0;0",
		"12;<A>;a c;0;1;2;0;0"
	};

	std::string line;
	size_t lineNumber = 0;
	while (std::getline(output, line))
	{
		REQUIRE(lineNumber < expectedLines.size());
		REQUIRE(line == expectedLines[lineNumber]);
		lineNumber++;
	}

	REQUIRE(lineNumber == expectedLines.size());

	output.close();
	std::remove(testFile.c_str());
	std::remove("test_output.csv");
}