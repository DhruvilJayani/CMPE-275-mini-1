#include "CrashRecord.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string_view>

CrashRecord::CrashRecord(const std::vector<std::string_view>& data) {
    crashDate = data[0];  
    crashTime = data[1];  
    borough = data[2];
    
    // Handling latitude and longitude conversion safely
    latitude = data[4].empty() ? 0.0f : std::stof(std::string(data[4]));  // Convert string_view to string before parsing
    longitude = data[5].empty() ? 0.0f : std::stof(std::string(data[5]));

    onStreetName = data[7];  
    
    // Safe conversion for number of persons injured
    try {
        numberOfPersonsInjured = data[10].empty() ? 0 : std::stoi(std::string(data[10]));  // Convert string_view to string
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument for number of persons injured: " << data[10] << ", setting to 0." << std::endl;
        numberOfPersonsInjured = 0;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range value for number of persons injured: " << data[10] << ", setting to 0." << std::endl;
        numberOfPersonsInjured = 0;
    }
}


void CrashRecord::printRecord() const {
    // Column headers for better clarity
    std::cout 
     << std::left << std::setw(15) << "Crash Date"
    //           << std::setw(15) << "Crash Time"
    //           << std::setw(15) << "Borough"
    //           << std::setw(12) << "Zip Code"
    //           << std::setw(10) << "Latitude"
    //           << std::setw(12) << "Longitude"
    //           << std::setw(15) << "On Street"
              << std::setw(18) << "Persons Injured" << std::endl;

    // Printing the actual record
    std::cout 
     << std::left << std::setw(15) << (crashDate.empty() ? "N/A" : crashDate)
    //           << std::setw(15) << (crashTime.empty() ? "N/A" : crashTime)
    //           << std::setw(15) << (borough.empty() ? "N/A" : borough)
    //           << std::setw(12) << (zipCode == 0 ? "N/A" : std::to_string(zipCode))
    //           << std::setw(10) << (latitude == 0.0f ? "N/A" : std::to_string(latitude))
    //           << std::setw(12) << (longitude == 0.0f ? "N/A" : std::to_string(longitude))
    //           << std::setw(15) << (onStreetName.empty() ? "N/A" : onStreetName)
              << std::setw(18) << (numberOfPersonsInjured == 0 ? "N/A" : std::to_string(numberOfPersonsInjured)) << std::endl;
}

std::string CrashRecord::getBorough() const {
    return borough;
}

int CrashRecord::getPersonsInjured() const {
    return numberOfPersonsInjured;
}

std::string CrashRecord::getCrashDate() const {
    return crashDate;
}

// Search function to find records by Borough
std::vector<CrashRecord> searchByBorough(const std::vector<CrashRecord>& records, const std::string& borough) {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        if (record.getBorough() == borough) {
            results.push_back(record);
        }
    }
    return results;
}

// Search function to find records within a specific date range
std::vector<CrashRecord> searchByDateRange(const std::vector<CrashRecord>& records, const std::string& startDate, const std::string& endDate) {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        if (record.getCrashDate() >= startDate && record.getCrashDate() <= endDate) {
            results.push_back(record);
        }
    }
    return results;
}

// Search function to find records with a minimum number of injuries
std::vector<CrashRecord> searchByInjuryCount(const std::vector<CrashRecord>& records, int minInjuryCount) {
    std::vector<CrashRecord> results;
    for (const auto& record : records) {
        if (record.getPersonsInjured() >= minInjuryCount) {
            results.push_back(record);
        }
    }
    return results;
}
