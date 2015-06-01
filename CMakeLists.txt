cmake_minimum_required (VERSION 2.8.11)
project(FAHBench)

set(FAHBench_VERSION_MAJOR 2)
set(FAHBench_VERSION_MINOR 1)
set(FAHBench_VERSION_REVIS 1)

# Cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# For QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Use c++11
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_subdirectory(fahbench)

# Copy data to build directory
file(GLOB OPENMM_DATA_FILES openmm_data/*.xml)
install(FILES ${OPENMM_DATA_FILES} DESTINATION share/openmm_data/)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${FAHBench_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${FAHBench_VERSION_MINOR}")
set(CPACK_PACKAGE_NAME "fahbench")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Folding@Home GPU Benchmarking")
set(CPACK_PACKAGE_DESCRIPTION "This package uses OpenMM to run GPU simulations of protein folding. The reported speed can be used as a benchmark for evaluating Folding@Home hardware.")
set(CPACK_PACKAGE_CONTACT "matthew.harrigan@outlook.com")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-program-options1.54.0 libboost-filesystem1.54.0 libqt5widgets5")
set(CPACK_BINARY_DEB "ON")

include (CPack)

