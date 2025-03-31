#include "CSVParser.h"
#include "GrammarProcessor.h"
#include "Lexer.h"
#include "SyntacticRecognizer.h"
#include "TokensConnector.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

const std::string TABLE_PATH = "modules/ll1_table.csv";
const std::string GENERATE_MODE = "generate";
const std::string RECOGNIZE_MODE = "recognize";
const std::string OPTIMIZE_MODE = "optimize";

struct Args
{
	std::string mode;
    std::string inputFileName;
};

Args ParseArgs(int argc, char* argv[])
{
    if (argc < 3)
    {
        throw std::invalid_argument(
                "Usage: " + std::string(argv[0]) + "optimize|generate|recognize <input_string>\n"
        );
    }

    Args args{};
	args.mode = argv[1];
    args.inputFileName = argv[2];

    return args;
}

void GenerateTable(const std::string& inputFileName)
{
	GrammarProcessor processor(inputFileName);
	processor.ProcessGrammar();
	processor.ExportTable(TABLE_PATH);
}

void Recognize(const std::string& inputFileName)
{
	std::ifstream input(inputFileName);
	auto lexer = Lexer();

	lexer.ScanInput(input);
	auto tokens = lexer.GetTokenList();

	auto convertedTokens = TokensConnector::ConvertTokens(tokens);

	CSVParser csvParser(TABLE_PATH);
	std::map<size_t, TableRow> table = csvParser.GetTable();

	SyntacticRecognizer recognizer(table, convertedTokens);
	if (recognizer.Parse())
	{
		std::cout << "Ok. Input is valid." << std::endl;
	} else
	{
		std::cout << "Error. Input is invalid." << std::endl;
	}
}

void Optimize(const std::string& inputFileName)
{
	//TODO: R4DEONN implement it pls
}

int main(int argc, char* argv[])
{
    try
    {
        Args args = ParseArgs(argc, argv);
    	if (args.mode == GENERATE_MODE)
    	{
    		GenerateTable(args.inputFileName);
    	}
    	else if (args.mode == RECOGNIZE_MODE)
    	{
    		Recognize(args.inputFileName);
    	}
    	else if (args.mode == OPTIMIZE_MODE)
    	{
    		Optimize(args.inputFileName);
    	}
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}