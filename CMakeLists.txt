cmake_minimum_required (VERSION 3.0)
project(FAHBench)

# Cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(GetGitRevisionDescription)
get_git_head_revision(FAHBench_VERSION_REFSPEC FAHBench_VERSION_SHA)

set(FAHBench_VERSION_MAJOR 2)
set(FAHBench_VERSION_MINOR 2)
set(FAHBench_VERSION_REVIS 4)
set(FAHBench_VERSION_ISRELEASE OFF)

# For QT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure compiler
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
include(Warnings)

add_subdirectory(workunits)
add_subdirectory(fahbench)


# This is a hack to get CLion to install
ADD_CUSTOM_TARGET(run_install COMMAND ${CMAKE_MAKE_PROGRAM} install)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${FAHBench_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${FAHBench_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${FAHBench_VERSION_REVIS}")
set(CPACK_PACKAGE_NAME "fahbench")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Folding@Home GPU Benchmarking")
set(CPACK_PACKAGE_DESCRIPTION "\
This package uses OpenMM to run GPU simulations of protein folding. \
The reported speed can be used as a benchmark for evaluating Folding@Home hardware.\
")
set(CPACK_PACKAGE_CONTACT "matthew.harrigan@outlook.com")

# .deb
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5widgets5")
set(CPACK_BINARY_DEB "ON")

# nsis windows installer
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/fahbench/gui/FahIcon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CPACK_NSIS_MUI_ICON}")
set(CPACK_NSIS_INSTALLED_ICON_NAME "${CPACK_NSIS_MUI_ICON}")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'")
set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\${PROJECT_NAME}-gui.exe'") 

if(${UNIX})
  set(CPACK_GENERATOR "TGZ")
endif(${UNIX})

if(${WIN32})
  SET(CPACK_GENERATOR "ZIP" "NSIS")
endif(${WIN32})


include (CPack)

