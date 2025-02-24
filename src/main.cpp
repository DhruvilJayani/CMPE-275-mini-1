#include <iostream>
#include <memory>
#include "CrashDataFacade.h"
#include "CSVDataReader.h"
#include "CrashDataArrays.h"
#include <chrono>

int main() {
    // Create a facade using the CSV data reader
    std::unique_ptr<IDataReader> csvReader = std::make_unique<CSVDataReader>();
    CrashDataFacade crashAPI(std::move(csvReader));

    // Load data from the CSV file
    std::string filename = "/Users/dhruviljayani/Documents/assignments/sem - 2/CMPE - 275/minis/crash_data_processor(mini-1)/data/data.csv";


    // //Reading Data Through array of Objects
    // if (!crashAPI.loadData(filename)) {
    //     std::cerr << "Failed to load data from " << filename << std::endl;
    //     return 1;
    // }
    // //  Perform searches via the API
    // auto startTime = std::chrono::high_resolution_clock::now();
    // auto brooklynRecords = crashAPI.searchByBorough("BROOKLYN");
    // auto injuryRecords  = crashAPI.searchByInjuryCount(10);
    // std::cout << "Records in BROOKLYN: " << brooklynRecords.size() << std::endl;
    // std::cout << "Records with injury count >= 10: " << injuryRecords.size() << std::endl;
    // auto endTime = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    // // std::cout << "Search started at: " << std::chrono::duration_cast<std::chrono::milliseconds>(startTime.time_since_epoch()).count() << " ms\n";
    // // std::cout << "Search ended at: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime.time_since_epoch()).count() << " ms\n";
    // std::cout << "Total time taken: " << duration.count() << " ms\n";




    //Loading data Through Objects of Array
    CrashDataArrays crashData;
    if (!crashAPI.loadDataInArray(filename, crashData)) {       
        std::cerr << "Failed to load data from " << filename << std::endl;
        return 1;
    }
    // Declare variables to hold search results
    std::vector<int> brooklynIndices;
    std::vector<int> injuryCount10Indices;
    //Use OpenMP to allocate a thread for each function
    auto startTimeA = std::chrono::high_resolution_clock::now();
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            //Search by borough "BROOKLYN"
            brooklynIndices = crashData.searchByBorough("BROOKLYN");
            std::cout << "Found " << brooklynIndices.size() << " records for borough 'BROOKLYN'." << std::endl;
        }

        #pragma omp section
        {
            //Search by number of persons injured = 10
            injuryCount10Indices = crashData.searchByInjuryCount(10);
            std::cout << "Found " << injuryCount10Indices.size() << " records with 10 persons injured." << std::endl;
        }
    }
    auto endTimeA = std::chrono::high_resolution_clock::now();
    auto durationA = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeA - startTimeA);
    // std::cout << "Search started at: " << std::chrono::duration_cast<std::chrono::milliseconds>(startTime.time_since_epoch()).count() << " ms\n";
    // std::cout << "Search ended at: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime.time_since_epoch()).count() << " ms\n";
    std::cout << "Total time taken: " << durationA.count() << " ms\n";

    return 0;
}

// readData("/Users/dhruviljayani/Documents/assignments/sem - 2/CMPE - 275/minis/crash_data_processor(mini-1)/data/data.csv");
// cmake -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ ..