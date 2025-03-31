#include "GrammarProcessor.h"
#include <iostream>

int main(const int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
		return EXIT_FAILURE;
	}

	const std::string inputFileName = argv[1];
	const std::string outputFileName = argv[2];

	try
	{
		GrammarProcessor processor(inputFileName);
		processor.ProcessGrammar();
		processor.ExportTable(outputFileName);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}