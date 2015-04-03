
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
add_executable(FAHBench-gui
    MainWindow.cpp
    CentralWidget.cpp
    SimulationWorker.cpp
    OneShotButton.cpp
    gui-main.cpp    
)

target_link_libraries(FAHBench-gui fahbench Qt5::Widgets)

INSTALL(TARGETS FAHBench-gui
        RUNTIME DESTINATION bin
)
endif()
