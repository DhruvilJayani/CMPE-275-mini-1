#ifndef CSVDATAREADER_H
#define CSVDATAREADER_H

#include "IDataReader.h"
#include <vector>
#include <string>
#include <string_view>

class CSVDataReader : public IDataReader {
public:
    std::vector<CrashRecord> readData(const std::string& filename) override;
    CrashDataArrays readDataInArray(const std::string& filename) override;  

private:
    std::vector<std::string_view> split(std::string_view line, char delimiter);
};

#endif 
