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
    CrashDataArrays crashData;
    std::ifstream file(filename, std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return crashData;
    }

    // Read entire file into memory
    size_t fileSize = file.tellg();
    file.seekg(0);
    std::string buffer(fileSize, '\0');
    file.read(&buffer[0], fileSize);
    file.close();

    // Split into lines
    std::vector<std::string> lines;
    size_t lineStart = 0;
    while (lineStart < buffer.size()) {
        size_t lineEnd = buffer.find('\n', lineStart);
        if (lineEnd == std::string::npos) lineEnd = buffer.size();
        
        if (lineStart == 0) { // Skip header
            lineStart = lineEnd + 1;
            continue;
        }
        
        lines.emplace_back(buffer.substr(lineStart, lineEnd - lineStart));
        lineStart = lineEnd + 1;
    }

    crashData.resize(lines.size());

    #pragma omp parallel for
    for (size_t i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];
        size_t fieldStart = 0;
        int fieldIndex = 0;
        bool inQuotes = false;

        // Fast CSV parsing
        for (size_t pos = 0; pos <= line.size(); ++pos) {
            if (pos == line.size() || (line[pos] == ',' && !inQuotes)) {
                std::string field = line.substr(fieldStart, pos - fieldStart);
                
                // Remove surrounding quotes
                if (field.size() >= 2 && field.front() == '"' && field.back() == '"') {
                    field = field.substr(1, field.size() - 2);
                }

                switch (fieldIndex) {
                    case 0: crashData.crashDates[i] = std::move(field); break;
                    case 1: crashData.crashTimes[i] = std::move(field); break;
                    case 2: crashData.boroughs[i] = std::move(field); break;
                    case 10: 
                        try {
                            crashData.numberOfPersonsInjured[i] = field.empty() ? 0 : std::stoi(field);
                        } catch (...) {
                            crashData.numberOfPersonsInjured[i] = 0;
                        }
                        break;
                    // Add other fields as needed
                }
                
                fieldStart = pos + 1;
                fieldIndex++;
            } else if (line[pos] == '"') {
                inQuotes = !inQuotes;
            }
        }
    }

    crashData.buildIndexes();
    return crashData;
}
