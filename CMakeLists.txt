cmake_minimum_required (VERSION 3.6)
project(FAHBench)

# Cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GetGitRevisionDescription)
get_git_head_revision(FAHBench_VERSION_REFSPEC FAHBench_VERSION_SHA)

set(FAHBench_VERSION_MAJOR 2)
set(FAHBench_VERSION_MINOR 3)
set(FAHBench_VERSION_REVIS 3)
set(FAHBench_VERSION_ISRELEASE OFF)

if(${WIN32})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Built/)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Built/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Built/lib)
endif()

# Configure compiler
set(CMAKE_CXX_STANDARD 14)
include(Warnings)

# Find cuda
find_package(CUDA QUIET)
if(CUDA_FOUND)
    set(USE_CUDA ON
        CACHE BOOL "Include CUDA")
else()
    set(USE_CUDA OFF
        CACHE BOOL "Include CUDA")
endif(CUDA_FOUND)

# Find opencl
find_package(OpenCL)
mark_as_advanced(CLEAR OpenCL_INCLUDE_DIR OpenCL_LIBRARY)

if(NOT ${OpenCL_FOUND})
    message(STATUS "OpenCL wasn't found! Trying to trick it into using AMDAPPSDKROOT environment var")
    find_library(OpenCL_LIBRARY OpenCL
                 HINTS ENV AMDAPPSDKROOT
                 PATH_SUFFIXES lib/x86_64)
endif()
find_package(OpenCL REQUIRED)

add_subdirectory(boost)
add_subdirectory(openmm)
add_subdirectory(fahbench)
add_subdirectory(workunits)
add_dependencies(fahbench workunits)

# build a CPack driven installer package
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${FAHBench_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${FAHBench_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${FAHBench_VERSION_REVIS}")
set(CPACK_PACKAGE_NAME "FAHBench")
set(CPACK_PACKAGE_VENDOR "Folding@Home")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Folding@Home GPU Benchmarking")
set(CPACK_PACKAGE_DESCRIPTION "\
This package uses OpenMM to run GPU simulations of protein folding. \
The reported speed can be used as a benchmark for evaluating Folding@Home hardware.\
")
set(CPACK_PACKAGE_CONTACT "matthew.harrigan@outlook.com")
set(CPACK_PACKAGE_EXECUTABLES "FAHBench-gui;FAHBench")

# wix windows installer
set(CPACK_WIX_UPGRADE_GUID "2BE18E2A-10A1-4C58-8880-CE6B8EF381F4")
set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/fahbench/gui/FahIcon.ico")

# This install visual studio runtime dll's
# include(InstallRequiredSystemLibraries)

if(${UNIX})
  set(CPACK_GENERATOR "TGZ")
endif(${UNIX})

if(${WIN32})
  SET(CPACK_GENERATOR "ZIP" "WIX")
endif(${WIN32})

include (CPack)
