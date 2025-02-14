#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <string_view>
#include "CrashRecord.h"

// Split function to handle CSV parsing
std::vector<std::string_view> split(std::string_view line, char delimiter) {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == delimiter) {
            tokens.push_back(line.substr(start, i - start));
            start = i + 1;
        }
    }
    // Add the last token
    tokens.push_back(line.substr(start));
    return tokens;
}

// Updated readData function to use string_view
std::vector<CrashRecord> readData(const std::string& filename) {
    std::vector<CrashRecord> records;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return records;
    }

    std::string line;
    // Skip the header
    std::getline(file, line);

    int count = 0;
    while (std::getline(file, line)) {
        // Create the CrashRecord object using string_views instead of copying strings
        std::vector<std::string_view> data = split(line, ',');  // Split the line by comma
        count++;
        records.emplace_back(data);  // Pass the data vector which contains string_view objects
        std::cout << count << std::endl;
    }

    file.close();
    return records;
}

int main() {
    std::vector<CrashRecord> records = readData("/Users/saisujithvalluru/Desktop/class-projects/mini-1.csv");

    // // Example usage of search functions:
     std::vector<CrashRecord> boroughResults = searchByBorough(records, "BROOKLYN");
    // std::vector<CrashRecord> dateRangeResults = searchByDateRange(records, "2023-01-01", "2023-12-31");
     std::vector<CrashRecord> injuryResults = searchByInjuryCount(records, 10);

    // // Displaying results
     std::cout << "Records in BROOKLYN: " << boroughResults.size() << std::endl;
    // std::cout << "Records from 2023: " << dateRangeResults.size() << std::endl;
     std::cout << "Records with 10+ injuries: " << injuryResults.size() << std::endl;

    // Print the top 2 rows for verification
    // if (records.size() >= 2) {
    //     std::cout << "Top 2 Records:" << std::endl;

    //     // Print the column headers only once
    //     records[0].printRecord();  // Print the first row
    //     std::cout << std::endl;  // Space between the rows
    //     records[1].printRecord();  // Print the second row
    //     std::cout << std::endl;  // Space between the rows
    //     records[2].printRecord();
    // } else {
    //     std::cout << "Not enough records to print." << std::endl;
    // }


    return 0;
}


//
// std::cout << "Total Records: " << records.size() << std::endl;
    // for (const auto& record : records) {
    //     record.printRecord();
    // }

//To Execute
//first Remove build rm -rf build
//the Build it using this 
// mkdir build && cd build
// cmake ..
// make
// time ./CrashDataProcessor
