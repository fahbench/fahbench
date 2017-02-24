#!/usr/bin/env bash

amdroot=$(find * -maxdepth 0 -name "AMDAPPSDK*" -exec echo `pwd`/{} \; | tail -1)

mkdir -p fahbench-build && cd fahbench-build
CC=gcc-5 CXX=g++-5                                   \
  $HOME/cmake/bin/cmake /source/                     \
    -DCMAKE_BUILD_TYPE=Release

make -j4                        \
    && $HOME/cmake/bin/cpack    \
    && mkdir -p /vagrant/dist/  \
    && cp FAHBench-*.tar.gz /vagrant/dist/
