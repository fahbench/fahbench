# Configure version information
configure_file(FAHBenchVersion.h FAHBenchVersion.h)

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
set_target_properties(fahbench PROPERTIES INSTALL_RPATH "$ORIGIN")
target_include_directories(fahbench PUBLIC ${OpenCL_INCLUDE_DIRS})
target_include_directories(fahbench INTERFACE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
if(${USE_CUDA})
    target_include_directories(fahbench PUBLIC ${CUDA_INCLUDE_DIRS})
endif(${USE_CUDA})

# Link
target_link_libraries(fahbench
                      boost
					  boost-filesystem
                      openmm
                      ${OpenCL_LIBRARIES}
                      )
add_dependencies(fahbench openmm-opencl openmm-cpu)

# Make executables
add_subdirectory(gui)
add_subdirectory(cmd)

# Install
if(${WIN32})
install(TARGETS fahbench RUNTIME DESTINATION .)
else()
install(TARGETS fahbench RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
endif()
