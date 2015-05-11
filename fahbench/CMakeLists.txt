
# Find boost and CUDA using CMake
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS program_options filesystem system)
find_package(CUDA QUIET)

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
if(${CUDA_FOUND})
    include_directories(${CUDA_INCLUDE_DIRS})
endif(${CUDA_FOUND})

# When we install, it needs to look in install_dir/lib for OpenMM
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Make library
add_library(fahbench SHARED
        StateTests.cpp
        Simulation.cpp
        GPUInfo.cpp
        Utils.cpp
)

# Link
target_link_libraries(fahbench
	"${OPENMM_LIB}"
	${Boost_LIBRARIES}
	${OPENCL_LIBRARIES}
)
if(${CUDA_FOUND})
    target_link_libraries(fahbench "${CUDA_CUDART_LIBRARY}")
endif(${CUDA_FOUND})

# Make executables
add_subdirectory(gui)
add_subdirectory(cmd)

# Install
INSTALL(TARGETS fahbench
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)

# Copy OpenMM plugins
FILE(GLOB OPENMM_PLUGIN_FILES "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMM*${CMAKE_SHARED_LIBRARY_SUFFIX}")
INSTALL(FILES ${OPENMM_PLUGIN_FILES}
	DESTINATION lib/plugins)

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

