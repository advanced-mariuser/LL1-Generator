#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "Table.h"

#include <map>

class CSVParser
{
public:
    explicit CSVParser(const std::string &csvFilePath);

    [[nodiscard]] std::map<size_t, TableRow> GetTable() const;

private:
    std::map<size_t, TableRow> m_table;

    void ParseCSV(const std::string &csvFilePath);

    static int ParsePointer(const std::string &token) ;
};

#endif // CSVPARSER_H