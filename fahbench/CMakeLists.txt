
# Install boost from source
include(BuildBoost)

# Find cuda
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

# Specify includes
include_directories(
    SYSTEM # won't generate warnings
    "${OPENMM_INCLUDE_PATH}"
    ${OPENCL_INCLUDE_DIRS}
    "${BOOST_INCLUDE_DIR}"
)
if(${USE_CUDA})
    include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
endif(${USE_CUDA})

# When we install, it needs to look in install_dir/lib for OpenMM
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/")

# Make library
add_library(fahbench SHARED
        StateTests.cpp
        Simulation.cpp
        SimulationResult.cpp
        WorkUnit.cpp
        GPUInfo.cpp
        Device.cpp
        Utils.cpp
)
add_dependencies(fahbench boost)

# Link
target_link_libraries(fahbench
    ${BOOST_LIBRARIES}
    "${OPENMM_LIB}"
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
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCPU${CMAKE_SHARED_LIBRARY_SUFFIX}"        
    )
    if(${USE_CUDA})
    set(OPENMM_PLUGIN_FILES
        ${OPENMM_PLUGIN_FILES}
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCUDA${CMAKE_SHARED_LIBRARY_SUFFIX}"        
    )
    endif(${USE_CUDA})
endif()
INSTALL(FILES ${OPENMM_PLUGIN_FILES} DESTINATION lib/plugins)
            

# For linux, put libOpenMM.so in the lib/ directory
if(${UNIX})
  INSTALL(FILES "${OPENMM_LIB}" DESTINATION lib)
endif(${UNIX})

# Windows requires dll's to be installed alongside the executable
if(${WIN32})
  INSTALL(FILES
    ${OPENMM_LIB_DIR}/OpenMM.dll
    ${OPENMM_LIB_DIR}/pthreadVC2_x64.dll
    DESTINATION bin)
endif(${WIN32})

