#!/usr/bin/env bash

add-apt-repository -y ppa:ubuntu-toolchain-r/test
apt-get update
# Install below packages for following reasons, resp.
#   `make`, opengl headers not in amdappsdk,
#   for pme cpu, for qt gui, for c++14 (resp)
apt-get install -y      \
    build-essential     \
    mesa-common-dev     \
    fftw3-dev           \
    qt5-default         \
    g++-5
apt-get dist-upgrade -y
apt-get autoremove -y

# cmake
cmake='cmake-3.6.3-Linux-x86_64.sh'
if [ ! -e $cmake ]; then
    wget -q --no-check-certificate https://cmake.org/files/v3.6/$cmake
fi
rm -rf cmake/
mkdir cmake && bash $cmake --prefix=cmake --skip-license

# Install AMD APP SDK
# There's no wget link, so make sure you have the tar.bz2 in the
# fahbench source directory (which is /vagrant)
rm -rf AMD-APP-SDK*
rm -rf /opt/AMDAPPSDK*
amdtar=`find /vagrant/* -maxdepth 0 -name "AMD-APP-SDK-*.tar.bz2" | tail -1`
tar -xf $amdtar
amdsh=`find * -maxdepth 0 -name "AMD-APP-SDK-*.sh" | tail -1`
bash $amdsh -- --silent --acceptEULA=yes > $amdsh.log
ln -sf /opt/AMDAPPSDK-* .
ln -sf /vagrant/vagrant/install-fahbench.sh .
