cmake_minimum_required(VERSION 3.10)
project(CrashDataProcessor)

set(CMAKE_CXX_STANDARD 17)

# Explicitly set OpenMP flags
find_package(OpenMP REQUIRED)

# Set the flags for OpenMP (if OpenMP is found)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
else()
    message(WARNING "OpenMP not found!")
endif()

# Include directories for OpenMP and your project
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(/opt/homebrew/opt/libomp/include)

# Add the executable target
add_executable(CrashDataProcessor
    src/main.cpp
    src/CrashRecord.cpp
    src/CSVDataReader.cpp
    src/CrashDataFacade.cpp
)

# Link OpenMP after the target has been created
if(OpenMP_CXX_FOUND)
    target_link_libraries(CrashDataProcessor PUBLIC OpenMP::OpenMP_CXX)
endif()
