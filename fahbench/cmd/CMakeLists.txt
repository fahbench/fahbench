add_executable(FAHBench-cmd
    cmd-main.cpp
    CommandLineUpdater.cpp
)

target_link_libraries(FAHBench-cmd fahbench)

INSTALL(TARGETS FAHBench-cmd
        RUNTIME DESTINATION bin
)