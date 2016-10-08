set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets QUIET)
if(Qt5Widgets_DIR)
    set(MAKE_QT_GUI ON
        CACHE BOOL "Build a GUI using Qt5")
else()
    if(NOT DEFINED MAKE_QT_GUI)
        message(WARNING "Could not find Qt libraries. Not building a GUI")
    endif()
    set(MAKE_QT_GUI OFF
        CACHE BOOL "Couldn't find Qt5 libraries...")
endif()


if(${MAKE_QT_GUI})
    add_executable(FAHBench-gui WIN32
                   MainWindow.cpp
                   CentralWidget.cpp
                   ResultsWidget.cpp
                   DeviceTableModel.cpp
                   WorkUnitTableModel.cpp
                   SimulationWorker.cpp
                   gui-main.cpp
                   Resources.qrc
                   FahIcon.rc
                   )

    set_target_properties(FAHBench-gui PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
    target_link_libraries(FAHBench-gui fahbench Qt5::Widgets)
    target_include_directories(FAHBench-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    install(TARGETS FAHBench-gui
            RUNTIME DESTINATION bin
            )

    # Windows requires Qt dll's to be installed alongside
    # the executable.
    if(${WIN32})
        # Get path to install dll's
        get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
        get_target_property(QtCore_location Qt5::Core LOCATION)
        get_target_property(QtGui_location Qt5::Gui LOCATION)
        get_filename_component(Qt_DLL_ROOT "${QtCore_location}" PATH)

        install(FILES
                "${QtWidgets_location}"
                "${QtCore_location}"
                "${QtGui_location}"
                "${Qt_DLL_ROOT}/icudt54.dll"
                "${Qt_DLL_ROOT}/icuin54.dll"
                "${Qt_DLL_ROOT}/icuuc54.dll"
                DESTINATION bin)
        install(FILES
                "${Qt_DLL_ROOT}/../plugins/platforms/qwindows.dll"
                DESTINATION bin/platforms)
    endif(${WIN32})

endif()
