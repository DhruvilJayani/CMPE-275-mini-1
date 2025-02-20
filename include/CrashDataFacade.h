#ifndef CRASHDATAFACADE_H
#define CRASHDATAFACADE_H

#include <vector>
#include <memory>
#include <string>
#include "CrashRecord.h"
#include "IDataReader.h"

class CrashDataFacade {
public:
    // The facade accepts any concrete IDataReader implementation.
    CrashDataFacade(std::unique_ptr<IDataReader> reader);

    // Load data from a file; returns true if successful.
    bool loadData(const std::string& filename);

    // API for basic searching
    std::vector<CrashRecord> searchByBorough(const std::string& borough) const;
    std::vector<CrashRecord> searchByDateRange(const std::string& startDate, const std::string& endDate) const;
    std::vector<CrashRecord> searchByInjuryCount(int minInjuryCount) const;

    // Optionally, expose all loaded records
    const std::vector<CrashRecord>& getRecords() const;

private:
    std::unique_ptr<IDataReader> dataReader;
    std::vector<CrashRecord> records;
};

#endif // CRASHDATAFACADE_H
