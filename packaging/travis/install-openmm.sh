#!/usr/bin/env bash

mkdir -p openmm-build && cd openmm-build
cmake ../fahbench/openmm                      \
    -DCMAKE_INSTALL_PREFIX=$HOME/openmm       \
    -DOPENMM_BUILD_C_AND_FORTRAN_WRAPPERS=OFF \
    -DOPENMM_BUILD_PYTHON_WRAPPERS=OFF        \
    -DOPENMM_BUILD_DRUDE_PLUGIN=OFF           \
    -DOPENMM_BUILD_AMOEBA_PLUGIN=OFF          \
    -DOPENMM_BUILD_RPMD_PLUGIN=OFF            \
    -DBUILD_TESTING=OFF                       \
    -DOPENMM_BUILD_OPENCL_LIB=ON              \
    -DOPENMM_BUILD_PME_PLUGIN=ON              \

make -j2 && make install
