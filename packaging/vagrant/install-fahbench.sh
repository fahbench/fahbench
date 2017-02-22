#!/usr/bin/env bash

amdroot=$(find * -maxdepth 0 -name "AMDAPPSDK*" -exec echo `pwd`/{} \; | tail -1)

mkdir -p fahbench-build && cd fahbench-build
$HOME/cmake/bin/cmake /source/                           \
    -DCMAKE_INSTALL_PREFIX=/vagrant/dist/fahbench        \
    -DCMAKE_BUILD_TYPE=Release

make -j4                      \
    && make install           \
    && $HOME/cmake/bin/cpack  \
    && cp FAHBench-*.tar.gz /vagrant/dist/
