
find_package(Qt5Widgets)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(CUDA)

# Find openmm
find_path(OPENMM_INCLUDE_PATH OpenMM.h HINTS ENV OPENMM_INCLUDE_PATH)
find_library(OPENMM_LIB OpenMM PATHS ENV OPENMM_LIB_PATH)
get_filename_component(OPENMM_LIB_DIR ${OPENMM_LIB} DIRECTORY)

# Find opencl
find_library(OPENCL_LIBRARY OpenCL)
find_path(OPENCL_INCLUDE_PATH CL/cl.h PATHS "/usr/local/cuda/include")

# Configure version information
configure_file(FAHBenchVersion.h.in FAHBenchVersion.h)

include_directories(${OPENMM_INCLUDE_PATH} ${OPENCL_INCLUDE_PATH} ${CUDA_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
add_executable(FAHBench
	FAHBench.cpp
	StateTests.cpp
	SimulationWorker.cpp
	GPUInfo.cpp
    window/window.cpp
	window/OneShotButton.cpp
)

target_link_libraries(FAHBench ${Boost_LIBRARIES} Qt5::Widgets ${OPENCL_LIBRARY} ${CUDA_CUDART_LIBRARY} ${OPENMM_LIB})
#target_link_libraries(FAHBench debug OpenMM_d debug OpenMMSerialization_d)
#target_link_libraries(FAHBench optimized OpenMM optimized OpenMMSerialization)


INSTALL(TARGETS FAHBench
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

# Get path to install dll's
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtGui_location Qt5::Gui LOCATION)

# TODO: Install icuin53.dll, icudt53.dll, icuuc53.dll
#       (or compile qt sans unicode)

INSTALL(FILES
	${OPENMM_LIB_DIR}/OpenMM.dll
	${OPENMM_LIB_DIR}/pthreadVC2_x64.dll
    ${OPENMM_LIB_DIR}/plugins/OpenMMOpenCL.dll
	${OPENMM_LIB_DIR}/plugins/OpenMMCUDA.dll 
	${QtWidgets_location}
	${QtCore_location}
	${QtGui_location}
    #${QT_BIN_DIR}/Qt5Widgets.dll
	#${QT_BIN_DIR}/Qt5Core.dll
DESTINATION bin)

