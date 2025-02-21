#include "CSVDataReader.h"
#include "CrashRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

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

    int count = 0;
    int rowsPrinted = 0;

    while (std::getline(file, line)) {
        std::vector<std::string> data = parseCSVLine(line);
        records.emplace_back(data);
        ++count;

        // Print the first 5 rows with column numbers
        // if (rowsPrinted < 5) {
        //     std::cout << "Row " << rowsPrinted + 1 << ": ";
        //     for (size_t i = 0; i < data.size(); ++i) {
        //         std::cout << "Column " << (i + 1) << ": " << data[i] << " | ";
        //     }
        //     std::cout << std::endl;
        //     ++rowsPrinted;
        // }
    }

    file.close();
    return records;
}
