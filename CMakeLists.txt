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

# Copy data to build directory
file(GLOB OPENMM_DATA_FILES openmm_data/*.xml)
file(COPY ${OPENMM_DATA_FILES} DESTINATION openmm_data/)
install(FILES ${OPENMM_DATA_FILES} DESTINATION openmm_data/)

