set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(${WIN32})
set(COMPONENTS Widgets WinExtras)
else()
set(COMPONENTS Widgets)
endif()

find_package(Qt5 COMPONENTS ${COMPONENTS})
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
	set(SOURCE_FILES
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
	if(${WIN32})
	set(SOURCE_FILES ${SOURCE_FILES} ProgressBar-Win.cpp)
	else()
	set(SOURCE_FILES ${SOURCE_FILES} ProgressBar-Linux.cpp)
	endif()

    add_executable(FAHBench-gui WIN32 ${SOURCE_FILES})
    set_target_properties(FAHBench-gui PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
    target_link_libraries(FAHBench-gui fahbench Qt5::Widgets Qt5::WinExtras)
	if(${WIN32})
	target_link_libraries(FAHBench-gui fahbench Qt5::WinExtras)
	endif()
    target_include_directories(FAHBench-gui PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

	if(${WIN32})
	install(TARGETS FAHBench-gui RUNTIME DESTINATION .)
	else()
	install(TARGETS FAHBench-gui RUNTIME DESTINATION bin)
	endif()

    # Windows requires Qt dll's to be installed alongside
    # the executable.
    if(${WIN32})        
		add_custom_command(TARGET FAHBench-gui POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"$<TARGET_FILE:Qt5::Widgets>"
				"$<TARGET_FILE:Qt5::Core>"
				"$<TARGET_FILE:Qt5::Gui>"
				"$<TARGET_FILE:Qt5::WinExtras>"
				"$<TARGET_FILE_DIR:fahbench>"
		)
		install(FILES
				"$<TARGET_FILE:Qt5::Widgets>"
				"$<TARGET_FILE:Qt5::Core>"
				"$<TARGET_FILE:Qt5::Gui>"
				"$<TARGET_FILE:Qt5::WinExtras>"
				DESTINATION .
		)
		install(FILES
				"$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>"
				DESTINATION platforms
		)
    endif(${WIN32})

endif()
