#include "CSVDataReader.h"
#include "CrashRecord.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <omp.h>

void parseCSVLineA(const std::string& line, std::vector<std::string>& fields);

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
    std::getline(file, line); 

    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    records.reserve(lines.size());

    omp_set_num_threads(2);
    #pragma omp parallel for
    for (size_t i = 0; i < lines.size(); ++i) {
        std::vector<std::string> data = parseCSVLine(lines[i]);
        CrashRecord record(data);

        
        #pragma omp critical
        {
            records.push_back(record);
        }
    }

    file.close();
    return records;
}



CrashDataArrays CSVDataReader::readDataInArray(const std::string& filename) {
    CrashDataArrays crashDataArrays;
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return crashDataArrays;
    }
    
    // Read entire file into memory. Earlier 10,000 Rows
    std::string content;
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();
    
    // Split into lines efficiently
    std::vector<std::string> lines;
    size_t start = 0, end = 0;
    while (end < content.size()) {
        if (content[end] == '\n' || content[end] == '\r') {
            if (start < end)
                lines.emplace_back(content.substr(start, end - start));
            if (content[end] == '\r' && end + 1 < content.size() && content[end+1] == '\n')
                end += 2;
            else
                end += 1;
            start = end;
        } else {
            end++;
        }
    }
    if (start < end)
        lines.emplace_back(content.substr(start, end - start));
    
    if (!lines.empty())
        lines.erase(lines.begin());
    
    const size_t numRecords = lines.size();
    crashDataArrays.resize(numRecords);
    
    // Parallel parse with no locks
    #pragma omp parallel for
    for (size_t i = 0; i < numRecords; ++i) {
        thread_local std::vector<std::string> fields;
        fields.clear();
        parseCSVLineA(lines[i], fields);
        crashDataArrays.setRecord(i, fields);
    }
    
    return crashDataArrays;
}

// Optimized CSV Parser
void parseCSVLineA(const std::string& line, std::vector<std::string>& fields) {
    fields.clear();
    fields.reserve(12); // Reserve for expected number of fields
    
    const char* start = line.data();
    const char* end = line.data() + line.size();
    const char* current = start;
    
    bool in_quotes = false;
    bool in_parentheses = false;
    char prev_char = 0;

    while (current < end) {
        if (*current == '"' && (current == start || prev_char != '\\')) {
            in_quotes = !in_quotes;
            prev_char = *current++;
            continue;
        }
        
        if (!in_quotes) {
            if (*current == '(') in_parentheses = true;
            if (*current == ')') in_parentheses = false;
            
            if (*current == ',' && !in_parentheses) {
                fields.emplace_back(start, current - start);
                start = current + 1;
            }
        }
        
        prev_char = *current++;
    }
    
    // Adding last field
    fields.emplace_back(start, current - start);
}