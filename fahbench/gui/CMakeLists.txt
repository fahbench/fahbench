
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
	SimulationTableModel.cpp
	DeviceTableModel.cpp
    SimulationWorker.cpp
    OneShotButton.cpp
    gui-main.cpp
	FahIcon.rc
)

target_link_libraries(FAHBench-gui fahbench Qt5::Widgets)

INSTALL(TARGETS FAHBench-gui
        RUNTIME DESTINATION bin
)

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
          ${QtWidgets_location}
          ${QtCore_location}
          ${QtGui_location}
  DESTINATION bin)
endif(${WIN32})

endif()
