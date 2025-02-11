
#include "CrashRecord.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    bool insideQuotes = false;

    for (char ch : line) {
        if (ch == '"' && (token.empty() || token.back() != '\\')) {
            // Toggle the insideQuotes flag when encountering an unescaped quote
            insideQuotes = !insideQuotes;
        } else if (ch == delimiter && !insideQuotes) {
            // Split the token when encountering a delimiter outside of quotes
            result.push_back(token);
            token.clear();
        } else {
            token += ch;
        }
    }
    // Push the last token if it's not empty
    if (!token.empty()) {
        result.push_back(token);
    }
    return result;
}

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

    while (std::getline(file, line)) {
        auto data = split(line, ',');  // Now this correctly handles quoted fields with commas
        records.emplace_back(data);
    }

    file.close();
    return records;
}

int main() {
    std::vector<CrashRecord> records = readData("/Users/dhruviljayani/Documents/assignments/sem - 2/CMPE - 275/minis/crash_data_processor(mini-1)/data/data.csv");

    // // Example usage of search functions:
     std::vector<CrashRecord> boroughResults = searchByBorough(records, "BROOKLYN");
    // std::vector<CrashRecord> dateRangeResults = searchByDateRange(records, "2023-01-01", "2023-12-31");
     std::vector<CrashRecord> injuryResults = searchByInjuryCount(records, 10);

    // // Displaying results
     std::cout << "Records in BROOKLYN: " << boroughResults.size() << std::endl;
    // std::cout << "Records from 2023: " << dateRangeResults.size() << std::endl;
     std::cout << "Records with 10+ injuries: " << injuryResults.size() << std::endl;

    //Print the top 2 rows for verification
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
