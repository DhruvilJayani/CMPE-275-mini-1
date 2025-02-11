cmake_minimum_required(VERSION 3.10)
project(CrashDataProcessor)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(CrashDataProcessor src/main.cpp src/CrashRecord.cpp)