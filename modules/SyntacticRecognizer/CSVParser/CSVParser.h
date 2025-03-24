#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "TableRow.h"

class CSVParser
{
public:
    explicit CSVParser(const std::string &csvFilePath);

    std::map<int, TableRow> GetTable() const;

private:
    std::map<int, TableRow> m_table;

    void ParseCSV(const std::string &csvFilePath);

    static int ParsePointer(const std::string &token) ;
};

#endif // CSVPARSER_H