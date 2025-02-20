#ifndef CRASHRECORD_H
#define CRASHRECORD_H

#include <string>
#include <vector>
#include <string_view>

class CrashRecord {
public:
    // Constructs a CrashRecord from a vector of string_views.
    CrashRecord(const std::vector<std::string>& data);

    void printRecord() const;

    // Getters for search operations
    std::string getBorough() const;
    int getPersonsInjured() const;
    std::string getCrashDate() const;

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

#endif // CRASHRECORD_H
