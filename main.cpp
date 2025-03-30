#include <iostream>
#include <stdexcept>
#include "SyntacticRecognizer.h"
#include "CSVParser.h"

struct Args
{
    std::string input;
};

Args ParseArgs(int argc, char* argv[])
{
    if (argc < 2)
    {
        throw std::invalid_argument(
                "Usage: " + std::string(argv[0]) + " <input_string>\n"
        );
    }

    Args args{};
    args.input = argv[1];

    return args;
}

int main(int argc, char* argv[])
{
    try
    {
        Args args = ParseArgs(argc, argv);

        CSVParser csvParser("modules/ll1_table.csv");
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