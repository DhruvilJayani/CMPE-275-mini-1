// New class to store data as arrays instead of objects
#ifndef CRASHDATAARRAYS_H
#define CRASHDATAARRAYS_H

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

class CrashDataArrays {
public:
    std::vector<std::string> crashDates;
    std::vector<std::string> crashTimes;
    std::vector<std::string> boroughs;
    std::vector<int> zipCodes;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<std::string> onStreetNames;
    std::vector<int> numberOfPersonsInjured;
    
    void addRecord(const std::vector<std::string>& data) {
        crashDates.push_back(data[0].empty() ? "" : data[0]);
        crashTimes.push_back(data[1].empty() ? "" : data[1]);
        boroughs.push_back(data[2].empty() ? "" : data[2]);
        // zipCodes.push_back(data[3].empty() ? 0 : std::stoi(data[3]));
        // latitudes.push_back(data[4].empty() ? 0.0f : std::stof(data[4]));
        // longitudes.push_back(data[5].empty() ? 0.0f : std::stof(data[5]));
        // onStreetNames.push_back(data[6].empty() ? "" : data[6]);
    
        try {
            numberOfPersonsInjured.push_back(data[10].empty() ? 0 : std::stoi(data[10]));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing injury count (" << data[7] << "): " << e.what() << ". Setting to 0." << std::endl;
            numberOfPersonsInjured.push_back(0);
        }
    }


    // Function to search for records by borough
    std::vector<int> searchByBorough(const std::string& targetBorough) {
        std::unordered_map<std::string, std::vector<int>> boroughMap;

        // Create a map of boroughs with their respective indices
        // omp_set_num_threads(3);
        // #pragma omp parallel for
        for (size_t i = 0; i < boroughs.size(); ++i) {
            #pragma omp critical
            {
                boroughMap[boroughs[i]].push_back(i);
            }
        }

        // Return the matching borough indices
        return boroughMap[targetBorough];  // O(1) lookup
    }

    // Function to search for records by number of persons injured
    std::vector<int> searchByInjuryCount(int targetInjuryCount) {
        std::unordered_map<int, std::vector<int>> injuryCountMap;

        // Create a map of injury counts with their respective indices
        // omp_set_num_threads(3);
        // #pragma omp parallel for 
        for (size_t i = 0; i < numberOfPersonsInjured.size(); ++i) {
            #pragma omp critical
            {
                injuryCountMap[numberOfPersonsInjured[i]].push_back(i);
            }
        }

        // Return the matching injury count indices
        return injuryCountMap[targetInjuryCount];  // O(1) lookup
    }
    
};

#endif // CRASHDATAARRAYS_H
