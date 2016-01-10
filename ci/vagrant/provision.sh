#!/usr/bin/env bash

apt-get update
apt-get install -y python-software-properties     # apt-add-repository
apt-add-repository -y ppa:ubuntu-toolchain-r/test # newer gcc
apt-add-repository -y ppa:ubuntu-sdk-team/ppa     # qt5
apt-get update
# Install below packages for following reasons, resp.
#   c++11, `make`, opengl headers not in amdappsdk,
#   for pme cpu, for qt gui (resp)
apt-get install -y      \
    g++-4.9             \
    build-essential     \
    mesa-common-dev     \
    fftw3-dev           \
    qt5-default

# cmake
wget --no-check-certificate https://cmake.org/files/v3.4/cmake-3.4.1-Linux-x86_64.sh
mkdir cmake && bash cmake-3.4.1-Linux-x86_64.sh --prefix=cmake --skip-license

# Install AMD APP SDK
# There's no wget link, so make sure you have the tar.bz2 in the
# fahbench source directory (which is /vagrant)
amdtar=`find /vagrant/* -maxdepth 0 -name "AMD-APP-SDK-*.tar.bz2" | tail -1`
tar -xf $amdtar
amdsh=`find * -maxdepth 0 -name "AMD-APP-SDK-*.sh" | tail -1`
bash $amdsh -- --silent --acceptEULA=yes


# Install OpenMM
bash /vagrant/ci/vagrant/install-openmm.sh


# Install FAHBench
bash /vagrant/ci/vagrant/install-fahbench.sh

