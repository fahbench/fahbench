cmake_minimum_required (VERSION 2.8.11)
project(FAHBench)

set(FAHBench_VERSION_MAJOR 2)
set(FAHBench_VERSION_MINOR 0)
set(FAHBench_VERSION_REVIS 0)

# For QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Use c++11
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_subdirectory(fahbench)




