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
add_dependencies(fahbench openmm-opencl)

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