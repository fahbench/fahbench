#!/usr/bin/env bash

cd $HOME
mkdir -p fahbench-build && cd fahbench-build
cmake ../fahbench/                                       \
    -DCMAKE_INSTALL_PREFIX=$HOME/fahbench-install/       \
    -DINSTALL_ALL_OPENMM_PLUGINS=ON                      \
    -DOPENMM_INCLUDE_PATH=$HOME/openmm/include           \
    -DOPENMM_LIB=$HOME/openmm/lib/libOpenMM.so           \
    -DENABLE_WARNINGS=ON                                 \
    -DMAKE_QT_GUI=ON                                     \

make -j2                      \
    && make install           \
