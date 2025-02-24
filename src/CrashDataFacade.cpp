#include "CrashDataFacade.h"
#include <iostream>
#include <vector>
#include <string>
#include "CrashDataArrays.h"
#include <omp.h>

CrashDataFacade::CrashDataFacade(std::unique_ptr<IDataReader> reader)
    : dataReader(std::move(reader))
{}

bool CrashDataFacade::loadData(const std::string& filename) {
    records = dataReader->readData(filename);
    // printTopRows(records);
    return !records.empty();
}

bool CrashDataFacade::loadDataInArray(const std::string& filename, CrashDataArrays& crashData) {
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            crashData = dataReader->readDataInArray(filename);
        }
    }
    return !crashData.crashDates.empty();
}


std::vector<CrashRecord> CrashDataFacade::searchByBorough(const std::string& borough) const {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        if (record.getBorough() == borough)
            results.push_back(record);
    }
    return results;
}

std::vector<CrashRecord> CrashDataFacade::searchByDateRange(const std::string& startDate, const std::string& endDate) const {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        std::string date = record.getCrashDate();
        if (date >= startDate && date <= endDate)
            results.push_back(record);
    }
    return results;
}

std::vector<CrashRecord> CrashDataFacade::searchByInjuryCount(int minInjuryCount) const {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        if (record.getPersonsInjured() == minInjuryCount)
            results.push_back(record);
    }
    return results;
}

const std::vector<CrashRecord>& CrashDataFacade::getRecords() const {
    return records;
}
