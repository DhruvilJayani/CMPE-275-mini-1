#include "CSVDataReader.h"
#include "CrashRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <omp.h>

std::vector<std::string_view> CSVDataReader::split(std::string_view line, char delimiter) {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == delimiter) {
            tokens.push_back(line.substr(start, i - start));
            start = i + 1;
        }
    }
    tokens.push_back(line.substr(start));
    return tokens;
}

std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::string currentField;
    bool inQuotes = false;
    bool inParentheses = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"' && (i == 0 || line[i - 1] != '\\')) {
            inQuotes = !inQuotes; // Toggle in/out of quotes
        } 
        else if (c == '(') {
            inParentheses = true;
        } 
        else if (c == ')') {
            inParentheses = false;
        } 
        else if (c == ',' && !inQuotes && !inParentheses) {
            // Only split on commas when NOT inside quotes or parentheses
            result.push_back(currentField);
            currentField.clear();
            continue;
        }

        currentField += c;
    }
    
    result.push_back(currentField); // Add the last field
    return result;
}

std::vector<CrashRecord> CSVDataReader::readData(const std::string& filename) {
    std::vector<CrashRecord> records;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return records;
    }

    std::string line;
    std::getline(file, line); // Skip header

    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Resize the vector to hold the correct number of records
    records.reserve(lines.size()); // Reserve memory, but don't initialize yet

    // Parallelize the loop that processes the lines
    omp_set_num_threads(2);
    #pragma omp parallel for
    for (size_t i = 0; i < lines.size(); ++i) {
        std::vector<std::string> data = parseCSVLine(lines[i]);
        CrashRecord record(data);  // Create a new CrashRecord using the parsed data

        // Use a critical section to assign the record back to the correct index
        #pragma omp critical
        {
            records.push_back(record);  // Instead of resizing, we use push_back
        }
    }

    file.close();
    return records;
}



CrashDataArrays CSVDataReader::readDataInArray(const std::string& filename) {
    CrashDataArrays crashDataArrays;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return crashDataArrays;
    }

    std::string line;
    std::getline(file, line); // Skip header

    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    // Resize the vector to hold the correct number of records
    lines.reserve(lines.size()); // No need for explicit reserve for crashDataArrays as it dynamically grows

    // Parallelize the loop that processes the lines
    // omp_set_num_threads(3);
    // #pragma omp parallel for
    for (size_t i = 0; i < lines.size(); ++i) {
        std::vector<std::string> data = parseCSVLine(lines[i]);

        // Create a new CrashDataArrays record using the parsed data
        // #pragma omp critical
        {
            crashDataArrays.addRecord(data);  // Add record to the CrashDataArrays object
        }
    }

    file.close();
    return crashDataArrays;
}