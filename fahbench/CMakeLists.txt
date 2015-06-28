
# Find boost and CUDA using CMake
include(BuildBoost)
if(${WIN32})
set(Boost_USE_STATIC_LIBS ON)
endif(${WIN32})
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS program_options filesystem system)
find_package(CUDA QUIET)

IF(CUDA_FOUND)
    set(USE_CUDA ON
    CACHE BOOL "Include CUDA")
ELSE()
    set(USE_CUDA OFF
    CACHE BOOL "Include CUDA")
ENDIF(CUDA_FOUND)

# Find openmm
find_path(OPENMM_INCLUDE_PATH OpenMM.h HINTS ENV OPENMM_INCLUDE_PATH)
find_library(OPENMM_LIB OpenMM PATHS ENV OPENMM_LIB_PATH)
get_filename_component(OPENMM_LIB_DIR ${OPENMM_LIB} DIRECTORY)

# Find opencl
find_package(OpenCL)

# Configure version information
configure_file(FAHBenchVersion.h.in FAHBenchVersion.h)

include_directories(
    "${OPENMM_INCLUDE_PATH}"
    ${OPENCL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
if(${USE_CUDA})
    include_directories(${CUDA_INCLUDE_DIRS})
endif(${USE_CUDA})

# When we install, it needs to look in install_dir/lib for OpenMM
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")

# Make library
add_library(fahbench SHARED
        StateTests.cpp
        Simulation.cpp
        GPUInfo.cpp
        Device.cpp
        Utils.cpp
)

# Link
target_link_libraries(fahbench
    "${OPENMM_LIB}"
    ${Boost_LIBRARIES}
    ${OPENCL_LIBRARIES}
)

# Make executables
add_subdirectory(gui)
add_subdirectory(cmd)

# Install
INSTALL(TARGETS fahbench
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)

# Copy OpenMM plugins
set(INSTALL_ALL_OPENMM_PLUGINS OFF
    CACHE BOOL "Copy all available OpenMM plugins instead of just the defaults")
    
if(${INSTALL_ALL_OPENMM_PLUGINS})
    FILE(GLOB OPENMM_PLUGIN_FILES "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMM*${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
    set(OPENMM_PLUGIN_FILES
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMOpenCL${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCUDA${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCPU${CMAKE_SHARED_LIBRARY_SUFFIX}"
    )
endif()
INSTALL(FILES ${OPENMM_PLUGIN_FILES} DESTINATION lib/plugins)
            

# For linux, put libOpenMM.so in the lib/ directory
if(${UNIX})
  INSTALL(FILES "${OPENMM_LIB}" DESTINATION lib)
  
  # Find real path to boost libraries
  set (_resolvedFiles "")
  foreach (_file ${Boost_LIBRARIES})
    get_filename_component(_resolvedFile "${_file}" REALPATH)
    list (APPEND _resolvedFiles "${_resolvedFile}")
  endforeach()
  INSTALL(FILES ${_resolvedFiles} DESTINATION lib)
endif(${UNIX})

# Windows requires dll's to be installed alongside the executable
if(${WIN32})
  INSTALL(FILES
          ${OPENMM_LIB_DIR}/OpenMM.dll
          ${OPENMM_LIB_DIR}/pthreadVC2_x64.dll
  DESTINATION bin)
endif(${WIN32})

