#!/usr/bin/env bash

amdroot=$(find * -maxdepth 0 -name "AMDAPPSDK*" -exec echo `pwd`/{} \; | tail -1)

ln -s /vagrant/ci/vagrant/boost-configure-gcc-4.9.jam user-config.jam

mkdir -p fahbench-build && cd fahbench-build
CC=gcc-4.9 \
CXX=g++-4.9 \
    $HOME/cmake/bin/cmake /vagrant/                          \
        -DCMAKE_INSTALL_PREFIX=/vagrant/dist/fahbench        \
        -DOPENCL_INCLUDE_DIRS=$amdroot/include               \
        -DOPENCL_LIBRARIES=$amdroot/lib/x86_64/libOpenCL.so  \
        -DINSTALL_ALL_OPENMM_PLUGINS=ON                      \
        -DOPENMM_INCLUDE_PATH=$HOME/openmm/include           \
        -DOPENMM_LIB=$HOME/openmm/lib/libOpenMM.so           \
        -DENABLE_WARNINGS=ON                                 \
        -DMAKE_QT_GUI=ON                                     \

make -j4                      \
    && make install           \
    && $HOME/cmake/bin/cpack  \
    && cp fahbench-*.tar.gz /vagrant/dist/
