#include "CrashRecord.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>

CrashRecord::CrashRecord(const std::vector<std::string>& data) {
    // Assume data has sufficient tokens. Add error checking if needed.
    crashDate = std::string(data[0]);
    crashTime = std::string(data[1]);
    borough   = std::string(data[2]);
    onStreetName = std::string(data[7]);
    
    try {
        numberOfPersonsInjured = data[10].empty() ? 0 : std::stoi(std::string(data[10]));
    } catch (const std::exception& e) {
        std::cerr << "Error parsing injury count (" << data[10] << "): " << e.what() << ". Setting to 0." << std::endl;
        numberOfPersonsInjured = 0;
    }
}

void CrashRecord::printRecord() const {
    std::cout 
        << std::left << std::setw(15) << (crashDate.empty() ? "N/A" : crashDate)
        << std::setw(18) << (numberOfPersonsInjured == 0 ? "N/A" : std::to_string(numberOfPersonsInjured))
        << std::endl;
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
