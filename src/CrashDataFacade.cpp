#include "CrashDataFacade.h"
#include <iostream>
#include <vector>
#include <string>

// void printTopRows(const std::vector<CrashRecord>& records) {
//     int rowsToPrint = std::min(5, static_cast<int>(records.size()));

//     for (int i = 0; i < rowsToPrint; ++i) {
//         const CrashRecord& record = records[i];
        
//         std::cout << "Row " << i + 1 << ":\n";
//         std::cout << "Column 0: " << record.getCrashDate() << " | ";
//         // std::cout << "Column 1: " << record.getCrashTime() << " | ";
//         std::cout << "Column 2: " << record.getBorough() << " | ";
//         // std::cout << "Column 3: " << record.getZipCode() << " | ";
//         // std::cout << "Column 4: " << record.getLatitude() << " | ";
//         // std::cout << "Column 5: " << record.getLongitude() << " | ";
//         // std::cout << "Column 6: " << record.getOnStreetName() << " | ";
//          std::cout << "Column 10: " << record.getPersonsInjured() << " | ";
//         std::cout << std::endl;
//     }
// }



CrashDataFacade::CrashDataFacade(std::unique_ptr<IDataReader> reader)
    : dataReader(std::move(reader))
{}

bool CrashDataFacade::loadData(const std::string& filename) {
    records = dataReader->readData(filename);
    // printTopRows(records);
    return !records.empty();
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
        if (record.getPersonsInjured() >= minInjuryCount)
            results.push_back(record);
    }
    return results;
}

const std::vector<CrashRecord>& CrashDataFacade::getRecords() const {
    return records;
}
