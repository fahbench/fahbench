#!/usr/bin/env bash

amdroot=$(find * -maxdepth 0 -name "AMDAPPSDK*" -exec echo `pwd`/{} \; | tail -1)

mkdir -p openmm-build && cd openmm-build
CC=gcc-4.9 \
CXX=g++-4.9 \
    $HOME/cmake/bin/cmake /vagrant/openmm         \
        -DCMAKE_INSTALL_PREFIX=$HOME/openmm       \
        -DOPENMM_BUILD_C_AND_FORTRAN_WRAPPERS=OFF \
        -DOPENMM_BUILD_PYTHON_WRAPPERS=OFF        \
        -DOPENMM_BUILD_DRUDE_PLUGIN=OFF           \
        -DOPENMM_BUILD_AMOEBA_PLUGIN=OFF          \
        -DOPENMM_BUILD_RPMD_PLUGIN=OFF            \
        -DBUILD_TESTING=OFF                       \
        -DOPENMM_BUILD_OPENCL_LIB=ON              \
        -DOPENMM_BUILD_PME_PLUGIN=ON              \
        -DOPENCL_INCLUDE_DIR=$amdroot/include              \
        -DOPENCL_LIBRARY=$amdroot/lib/x86_64/libOpenCL.so  \


make -j4 && make install
