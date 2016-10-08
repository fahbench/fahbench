
# Install boost from source
include(BuildBoost)

# Find cuda
find_package(CUDA QUIET)
if(CUDA_FOUND)
    set(USE_CUDA ON
        CACHE BOOL "Include CUDA")
else()
    set(USE_CUDA OFF
        CACHE BOOL "Include CUDA")
endif(CUDA_FOUND)

# Find openmm
find_path(OPENMM_INCLUDE_PATH OpenMM.h
          HINTS ENV OPENMM_INCLUDE_PATH
          PATHS ~/opt/openmm-core21/include)
find_library(OPENMM_LIB OpenMM
             HINTS ENV OPENMM_LIB_PATH
             PATHS ~/opt/openmm-core21/lib/)
get_filename_component(OPENMM_LIB_DIR ${OPENMM_LIB} DIRECTORY)

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

# Configure version information
configure_file(FAHBenchVersion.h FAHBenchVersion.h)

# Specify includes
include_directories(
        SYSTEM # won't generate warnings
        "${OPENMM_INCLUDE_PATH}"
        ${OpenCL_INCLUDE_DIRS}
        "${BOOST_INCLUDE_DIR}"
)
if(${USE_CUDA})
    include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
endif(${USE_CUDA})

# Make library
set(fahbench_sources
    StateTests.cpp
    Simulation.cpp
    SimulationResult.cpp
    WorkUnit.cpp
    GPUInfo-OpenCL.cpp
    Device.cpp
    Utils.cpp
    )
if(${USE_CUDA})
    set(fahbench_sources ${fahbench_sources} GPUInfo-CUDA.cpp)
else()
    set(fahbench_sources ${fahbench_sources} GPUInfo-No-CUDA.cpp)
endif()

add_library(fahbench SHARED ${fahbench_sources})
add_dependencies(fahbench boost workunits)
set_target_properties(fahbench PROPERTIES INSTALL_RPATH "$ORIGIN")
target_include_directories(fahbench PUBLIC ${OPENMM_INCLUDE_PATH})
target_include_directories(fahbench PUBLIC ${OpenCL_INCLUDE_DIRS})
target_include_directories(fahbench PUBLIC ${BOOST_INCLUDE_DIR})
target_include_directories(fahbench INTERFACE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
if(${USE_CUDA})
    target_include_directories(fahbench PUBLIC ${CUDA_INCLUDE_DIRS})
endif(${USE_CUDA})

# Link
target_link_libraries(fahbench
                      ${BOOST_LIBRARIES}
                      "${OPENMM_LIB}"
                      ${OpenCL_LIBRARIES}
                      )

# Make executables
add_subdirectory(gui)
add_subdirectory(cmd)

# Mock install structure in build directory
if(${WIN32})
    # CMake can't do symlinks on windows
else()
    add_custom_command(TARGET fahbench
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory share/
                       COMMAND ${CMAKE_COMMAND} -E make_directory share/fahbench/
                       COMMAND ${CMAKE_COMMAND} -E create_symlink ../../../workunits/workunits share/fahbench/workunits
                       )
    add_custom_command(TARGET fahbench
                       POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory lib/
                       COMMAND ${CMAKE_COMMAND} -E create_symlink "${OPENMM_LIB_DIR}/plugins" lib/plugins
                       )
endif()

# Install
install(TARGETS fahbench
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )

# Copy OpenMM plugins
set(INSTALL_ALL_OPENMM_PLUGINS OFF
    CACHE BOOL "Copy all available OpenMM plugins instead of just the defaults")

if(${INSTALL_ALL_OPENMM_PLUGINS})
    file(GLOB OPENMM_PLUGIN_FILES "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMM*${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
    set(OPENMM_PLUGIN_FILES
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMOpenCL${CMAKE_SHARED_LIBRARY_SUFFIX}"
        "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCPU${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )
    if(${USE_CUDA})
        set(OPENMM_PLUGIN_FILES ${OPENMM_PLUGIN_FILES}
            "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCUDA${CMAKE_SHARED_LIBRARY_SUFFIX}"
            )
    endif(${USE_CUDA})
endif()
install(FILES ${OPENMM_PLUGIN_FILES} DESTINATION lib/plugins)


# For linux, put libOpenMM.so in the lib/ directory
if(${UNIX})
    install(FILES "${OPENMM_LIB}" DESTINATION lib)
endif(${UNIX})

# Windows requires dll's to be installed alongside the executable
if(${WIN32})
    install(FILES
            ${OPENMM_LIB_DIR}/OpenMM.dll
            DESTINATION bin)
endif(${WIN32})

