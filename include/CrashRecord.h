#ifndef CRASHRECORD_H
#define CRASHRECORD_H

#include <string>
#include <vector>
#include <string_view>


class CrashRecord {
public:
    CrashRecord(const std::vector<std::string_view>& data);
    
    void printRecord() const;

    // Getters for search operations
    std::string getBorough() const;
    int getPersonsInjured() const;
    std::string getCrashDate() const;  // Add this getter for the Date Range search

private:
    std::string crashDate;
    std::string crashTime;
    std::string borough;
    int zipCode;
    float latitude;
    float longitude;
    std::string onStreetName;
    int numberOfPersonsInjured;
};

// Function declarations for searching records
std::vector<CrashRecord> searchByBorough(const std::vector<CrashRecord>& records, const std::string& borough);
std::vector<CrashRecord> searchByDateRange(const std::vector<CrashRecord>& records, const std::string& startDate, const std::string& endDate);
std::vector<CrashRecord> searchByInjuryCount(const std::vector<CrashRecord>& records, int minInjuryCount);

#endif // CRASHRECORD_H
