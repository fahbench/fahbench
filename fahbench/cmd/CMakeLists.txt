add_executable(FAHBench-cmd
               cmd-main.cpp
               CommandLineUpdater.cpp
               )

set_target_properties(FAHBench-cmd PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
target_link_libraries(FAHBench-cmd fahbench boost-program_options)

if(${WIN32})
install(TARGETS FAHBench-cmd RUNTIME DESTINATION .)
else()
install(TARGETS FAHBench-cmd RUNTIME DESTINATION bin)
endif()