#ifndef CRASHRECORD_H
#define CRASHRECORD_H

#include <string>
#include <vector>
#include <string_view>

class CrashRecord {
public:
    
    CrashRecord(const std::vector<std::string>& data);

    void printRecord() const;

    
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

#endif 
