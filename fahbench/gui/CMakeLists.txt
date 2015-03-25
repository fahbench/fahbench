
find_package(Qt5Widgets)
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