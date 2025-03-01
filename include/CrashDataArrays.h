#ifndef CRASHDATAARRAYS_H
#define CRASHDATAARRAYS_H

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

class CrashDataArrays {
public:
    // Data arrays
    std::vector<std::string> crashDates;
    std::vector<std::string> crashTimes;
    std::vector<std::string> boroughs;
    std::vector<int> zipCodes;
    std::vector<float> latitudes;
    std::vector<float> longitudes;
    std::vector<std::string> onStreetNames;
    std::vector<int> numberOfPersonsInjured;

    // Indexes
    std::unordered_map<std::string, std::vector<int>> boroughIndex;
    std::unordered_map<int, std::vector<int>> injuryCountIndex;

    void resize(size_t size) {
        crashDates.resize(size);
        crashTimes.resize(size);
        boroughs.resize(size);
        zipCodes.resize(size);
        latitudes.resize(size);
        longitudes.resize(size);
        onStreetNames.resize(size);
        numberOfPersonsInjured.resize(size);
    }

    void setRecord(size_t index, const std::vector<std::string>& data) {
        crashDates[index] = data[0];
        crashTimes[index] = data[1];
        boroughs[index] = data[2];
        
        
        numberOfPersonsInjured[index] = data[10].empty() ? 0 : fast_atoi(data[10]);
    }

    static int fast_atoi(const std::string& str) {
        int val = 0;
        for (char c : str) {
            if (c >= '0' && c <= '9') 
                val = val * 10 + (c - '0');
        }
        return val;
    }

    // Fast float parsing (simple version)
    static float fast_atof(const std::string& str) {
        float result = 0.0f;
        float sign = 1.0f;
        float decimal = 0.1f;
        bool found_dot = false;
        
        for (char c : str) {
            if (c == '-') {
                sign = -1.0f;
            } else if (c == '.') {
                found_dot = true;
            } else if (c >= '0' && c <= '9') {
                if (found_dot) {
                    result += (c - '0') * decimal;
                    decimal *= 0.1f;
                } else {
                    result = result * 10.0f + (c - '0');
                }
            }
        }
        return sign * result;
    }

    void buildIndexes() {
        #pragma omp parallel
        {
            // Create thread-local indexes
            std::unordered_map<std::string, std::vector<int>> localBoroughIndex;
            std::unordered_map<int, std::vector<int>> localInjuryIndex;

            #pragma omp for nowait
            for (size_t i = 0; i < boroughs.size(); ++i) {
                localBoroughIndex[boroughs[i]].push_back(i);
                localInjuryIndex[numberOfPersonsInjured[i]].push_back(i);
            }

            // Merge thread-local indexes
            #pragma omp critical
            {
                for (auto& pair : localBoroughIndex) {
                    auto& vec = boroughIndex[pair.first];
                    vec.insert(vec.end(), pair.second.begin(), pair.second.end());
                }
                for (auto& pair : localInjuryIndex) {
                    auto& vec = injuryCountIndex[pair.first];
                    vec.insert(vec.end(), pair.second.begin(), pair.second.end());
                }
            }
        }
    }
    
  

    void addRecord(const std::vector<std::string>& data) {
        int index = crashDates.size();  // current index before pushing new data
        
        crashDates.push_back(data[0].empty() ? "" : data[0]);
        crashTimes.push_back(data[1].empty() ? "" : data[1]);
        boroughs.push_back(data[2].empty() ? "" : data[2]);
    

        try {
            numberOfPersonsInjured.push_back(data[10].empty() ? 0 : std::stoi(data[10]));
        } catch (const std::exception& e) {
            numberOfPersonsInjured.push_back(0);
        }
        
    }

    std::vector<int> searchByBorough(const std::string& targetBorough) {
        std::vector<int> indices;
        
        for (size_t i = 0; i < boroughs.size(); ++i) {
            if (boroughs[i] == targetBorough) {
                indices.push_back(i);
            }
        }
        
        return indices;
    }

    std::vector<int> searchByInjuryCount(int targetInjuryCount) {
        std::vector<int> indices;
    
        for (size_t i = 0; i < numberOfPersonsInjured.size(); ++i) {
            if (numberOfPersonsInjured[i] == targetInjuryCount) {
                indices.push_back(i);
            }
        }
    
        return indices;
    }
};

#endif 
