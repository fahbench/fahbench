add_executable(FAHBench-cmd
               cmd-main.cpp
               CommandLineUpdater.cpp
               )

set_target_properties(FAHBench-cmd PROPERTIES INSTALL_RPATH "$ORIGIN/../lib")
target_link_libraries(FAHBench-cmd fahbench)

install(TARGETS FAHBench-cmd
        RUNTIME DESTINATION bin
        )