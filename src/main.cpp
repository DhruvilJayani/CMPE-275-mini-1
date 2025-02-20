#include <iostream>
#include <memory>
#include "CrashDataFacade.h"
#include "CSVDataReader.h"

int main() {
    // Create a facade using the CSV data reader
    std::unique_ptr<IDataReader> csvReader = std::make_unique<CSVDataReader>();
    CrashDataFacade crashAPI(std::move(csvReader));

    // Load data from the CSV file
    std::string filename = "/Users/dhruviljayani/Documents/assignments/sem - 2/CMPE - 275/minis/crash_data_processor(mini-1)/data/data.csv";
    if (!crashAPI.loadData(filename)) {
        std::cerr << "Failed to load data from " << filename << std::endl;
        return 1;
    }

    // Perform searches via the API
    auto brooklynRecords = crashAPI.searchByBorough("BROOKLYN");
    auto injuryRecords  = crashAPI.searchByInjuryCount(10);
    // auto dateRangeRecords = crashAPI.searchByDateRange("2023-01-01", "2023-12-31");

    std::cout << "Records in BROOKLYN: " << brooklynRecords.size() << std::endl;
    std::cout << "Records with 10+ injuries: " << injuryRecords.size() << std::endl;
    // std::cout << "Records within date range: " << dateRangeRecords.size() << std::endl;

    // Optionally, print the first record for verification
    if (!brooklynRecords.empty()) {
        brooklynRecords.front().printRecord();
    }

    return 0;
}

// readData("/Users/dhruviljayani/Documents/assignments/sem - 2/CMPE - 275/minis/crash_data_processor(mini-1)/data/data.csv");