
find_package(Qt5Widgets)

# Find openmm
find_path(OPENMM_INCLUDE_PATH OpenMM.h HINTS ENV OPENMM_INCLUDE_PATH)

# Find opencl
find_library(OPENCL_LIBRARY OpenCL)
find_path(OPENCL_INCLUDE_PATH CL/cl.h PATHS "/usr/local/cuda/include")

configure_file(FAHBenchVersion.h.in FAHBenchVersion.h)

include_directories(${OPENMM_INCLUDE_PATH} ${OPENCL_INCLUDE_PATH})
add_executable(FAHBench FAHBench.cpp StateTests.cpp SimulationWorker.cpp gpuinfo.cpp
               window/window.cpp window/OneShotButton.cpp)

target_link_libraries(FAHBench Qt5::Widgets OpenCL )
#target_link_libraries(FAHBench debug OpenMM_d debug OpenMMSerialization_d)
#target_link_libraries(FAHBench optimized OpenMM optimized OpenMMSerialization)


#INSTALL(TARGETS FAHBench RUNTIME DESTINATION ./)
#INSTALL(FILES ${OPENMM_LIB_DIR}/OpenMM.dll ${OPENMM_LIB_DIR}/OpenMMSerialization.dll ${OPENMM_LIB_DIR}/pthreadVC2.dll
#        ${OPENMM_LIB_DIR}/plugins/OpenMMOpenCL.dll  ${OPENMM_LIB_DIR}/plugins/OpenMMCUDA.dll 
#        ${QT_BIN_DIR}/QtGui4.dll ${QT_BIN_DIR}/QtCore4.dll DESTINATION ./)
#FILE(GLOB XMLS "${CMAKE_SOURCE_DIR}/scratch/*.xml")
#INSTALL(FILES ${XMLS} DESTINATION ./scratch/)