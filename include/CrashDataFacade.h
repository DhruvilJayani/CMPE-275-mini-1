#ifndef CRASHDATAFACADE_H
#define CRASHDATAFACADE_H

#include <vector>
#include <memory>
#include <string>
#include "CrashRecord.h"
#include "IDataReader.h"
#include "CrashDataArrays.h"

class CrashDataFacade {
public:
    
    CrashDataFacade(std::unique_ptr<IDataReader> reader);

    
    bool loadData(const std::string& filename);

    
    bool loadDataInArray(const std::string& filename, CrashDataArrays& crashData);

    
    std::vector<CrashRecord> searchByBorough(const std::string& borough) const;
    std::vector<CrashRecord> searchByDateRange(const std::string& startDate, const std::string& endDate) const;
    std::vector<CrashRecord> searchByInjuryCount(int minInjuryCount) const;

    
    const std::vector<CrashRecord>& getRecords() const;

private:
    std::unique_ptr<IDataReader> dataReader;
    std::vector<CrashRecord> records;
};

#endif 
