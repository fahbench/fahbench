
find_package(Qt5Widgets)
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
find_package(CUDA)

# Find openmm
find_path(OPENMM_INCLUDE_PATH OpenMM.h HINTS ENV OPENMM_INCLUDE_PATH)
find_library(OPENMM_LIB OpenMM PATHS ENV OPENMM_LIB_PATH)
#add_library(OpenMM SHARED IMPORTED)
#set_property(TARGET OpenMM PROPERTY IMPORTED_LOCATION "${OPENMM_LIB}")
get_filename_component(OPENMM_LIB_DIR ${OPENMM_LIB} DIRECTORY)

# Find opencl
find_library(OPENCL_LIBRARY OpenCL)
find_path(OPENCL_INCLUDE_PATH CL/cl.h PATHS "/usr/local/cuda/include")

# Configure version information
configure_file(FAHBenchVersion.h.in FAHBenchVersion.h)

include_directories(${OPENMM_INCLUDE_PATH} ${OPENCL_INCLUDE_PATH} ${CUDA_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
# When we install, it needs to look in install_dir/lib for OpenMM
# TODO: test how this behaves on windows
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_executable(FAHBench
        FAHBench.cpp
        StateTests.cpp
        SimulationWorker.cpp
        GPUInfo.cpp
        Utils.cpp
        window/window.cpp
        window/OneShotButton.cpp
        CommandLineUpdater.cpp
)

target_link_libraries(FAHBench
    Qt5::Widgets 
    "${OPENMM_LIB}"
    "${Boost_LIBRARIES}"
    "${OPENCL_LIBRARY}"
    "${CUDA_CUDART_LIBRARY}"
)

# Install the program
INSTALL(TARGETS FAHBench
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)

# Copy OpenMM plugins
INSTALL(FILES
  "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMOpenCL${CMAKE_SHARED_LIBRARY_SUFFIX}"
  "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCUDA${CMAKE_SHARED_LIBRARY_SUFFIX}"
  "${OPENMM_LIB_DIR}/plugins/${CMAKE_SHARED_LIBRARY_PREFIX}OpenMMCPU${CMAKE_SHARED_LIBRARY_SUFFIX}"
DESTINATION lib/plugins)

if(${UNIX})
  INSTALL(FILES "${OPENMM_LIB}" DESTINATION lib)
endif(${UNIX})

# Windows requires Qt dll's to be installed alongside
# the executable.
if(${WIN32})
  # Get path to install dll's
  get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
  get_target_property(QtCore_location Qt5::Core LOCATION)
  get_target_property(QtGui_location Qt5::Gui LOCATION)

  # TODO: Install icuin53.dll, icudt53.dll, icuuc53.dll
  #       (or compile qt sans unicode)

  INSTALL(FILES
          ${OPENMM_LIB_DIR}/OpenMM.dll
          ${OPENMM_LIB_DIR}/pthreadVC2_x64.dll
          ${QtWidgets_location}
          ${QtCore_location}
          ${QtGui_location}
  DESTINATION bin)
endif(${WIN32})
