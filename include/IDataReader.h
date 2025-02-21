#ifndef IDATAREADER_H
#define IDATAREADER_H

#include <vector>
#include <string>
#include "CrashRecord.h"

class IDataReader {
public:
    virtual ~IDataReader() = default;
    virtual std::vector<CrashRecord> readData(const std::string& filename) = 0;
};

#endif // IDATAREADER_H
