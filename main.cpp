#include <iostream>
#include <stdexcept>
#include "SyntacticRecognizer.h"
#include "CSVParser.h"

struct Args
{
    std::string csvFilePath;
    std::string input;
};

Args ParseArgs(int argc, char* argv[])
{
    if (argc < 3)
    {
        throw std::invalid_argument(
                "Usage: " + std::string(argv[0]) + " <csv_file_path> <input_string>\n"
        );
    }

    Args args{};
    args.csvFilePath = argv[1];
    args.input = argv[2];

    return args;
}

int main(int argc, char* argv[])
{
    try
    {
        Args args = ParseArgs(argc, argv);

        CSVParser csvParser(args.csvFilePath);
        std::map<size_t, TableRow> table = csvParser.GetTable();

        SyntacticRecognizer recognizer(table);
        if (recognizer.Parse(args.input))
        {
            std::cout << "Ok. Input is valid." << std::endl;
        } else
        {
            std::cout << "Error. Input is invalid." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}