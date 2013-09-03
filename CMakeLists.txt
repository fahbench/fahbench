cmake_minimum_required (VERSION 2.6)
PROJECT(FAHBench)

FIND_PACKAGE(Qt4 4.8.4 REQUIRED)
INCLUDE(${QT_USE_FILE})

set(FAHBench_VERSION_MAJOR 1)
set(FAHBench_VERSION_MINOR 2)
set(FAHBench_VERSION_REVIS 1)

configure_file("${PROJECT_SOURCE_DIR}/FAHBenchVersion.h.in" "${PROJECT_SOURCE_DIR}/FAHBenchVersion.h")

set(OPENMM_DIR "C:/Libs/openmm51/" CACHE PATH "OpenMM Installation Directory")

get_filename_component(QT_ROOT_DIR ${QT_INCLUDE_DIR} PATH)
set(QT_BIN_DIR ${QT_ROOT_DIR}/bin CACHE PATH "Qt Binaries Directory")
set(CUDA_ROOT_DIR "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v5.5/" CACHE PATH "CUDA INSTALL PATH")
set(OPENMM_LIB_DIR ${OPENMM_DIR}/lib)
set(OPENMM_INC_DIR ${OPENMM_DIR}/include)
include_directories(${OPENMM_INC_DIR} ${CUDA_ROOT_DIR}/include ${QT_USE_FILE} ${PROJECT_SOURCE_DIR})
link_directories(${OPENMM_LIB_DIR} ${CUDA_ROOT_DIR}/lib/Win32)
set(qt_HEADERS SimulationWorker.h window/window.h window/OneShotButton.h)
qt4_wrap_cpp(qt4_HEADERS_MOC ${qt_HEADERS})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable(FAHBench FAHBench.cpp StateTests.cpp SimulationWorker.cpp gpuinfo.cpp
               window/window.cpp window/OneShotButton.cpp icon.rc ${qt4_HEADERS_MOC})

target_link_libraries(FAHBench OpenCL ${QT_LIBRARIES})
target_link_libraries(FAHBench debug OpenMM_d debug OpenMMSerialization_d)
target_link_libraries(FAHBench optimized OpenMM optimized OpenMMSerialization)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install CACHE PATH "Install Location" FORCE)

INSTALL(TARGETS FAHBench RUNTIME DESTINATION ./)
INSTALL(FILES ${OPENMM_LIB_DIR}/OpenMM.dll ${OPENMM_LIB_DIR}/OpenMMSerialization.dll ${OPENMM_LIB_DIR}/pthreadVC2.dll
        ${OPENMM_LIB_DIR}/plugins/OpenMMOpenCL.dll  ${OPENMM_LIB_DIR}/plugins/OpenMMCUDA.dll 
        ${QT_BIN_DIR}/QtGui4.dll ${QT_BIN_DIR}/QtCore4.dll DESTINATION ./)
FILE(GLOB XMLS "${CMAKE_SOURCE_DIR}/scratch/*.xml")
INSTALL(FILES ${XMLS} DESTINATION ./scratch/)