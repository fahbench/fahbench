---
title: Building and Installing
layout: page
---

FAHBench is built with [CMake] and requires the following external libraries:

 - OpenCL SDK. We generally use the AMD APP SDK.
   OpenCL drivers must be installed on user machines to drive GPU calculations.
 - CUDA (*optional*) can be leveraged to drive GPU calculations on nvidia
   hardware. If the CUDA runtime is not available on user machines,
   FAHBench will gracefully disable CUDA as an option. The released
   binaries aren't compiled to support CUDA at all. Folding@Home does not
   use CUDA.
 - [Qt5] (*optional*) is necessary for the GUI.

Additionally, we vendor these dependencies in git submodules:
 
 - [Boost] for general utilities
 - [OpenMM] ([upstream], [with fah patches]) contains the scientific code to run molecular
   dynamics calculations.


[CMake]: http://www.cmake.org/
[Boost]: http://www.boost.org/
[Qt5]: http://qt-project.org/
[OpenMM]: http://openmm.org/
[upstream]: http://github.com/pandegroup/openmm/
[with fah patches]: http://github.com/pandegroup/openmm/

Linux
-----

 1. Get the prerequisites

    ```
    sudo apt-get install    \
        build-essential     \
        mesa-common-dev     \
        fftw3-dev           \
        qt5-default
    ```

 1. Install the AMD APP SDK

 1. From a clean build directory, use `ccmake` to configure the build.
    Make sure the right OpenCL and Qt libraries are being found.

 1. Run `make && make install`!


### Windows

 1. Download and install 
      - Visual Studio Community 2015
      - Qt >= 5.6
      - CMake >= 3.6
      - AMD APP SDK
      - (Optional) CUDA SDK

    These all have nice GUI installers.

 1. (Optional) Download and build fftw3 for the CPU platform. I haven't done
    this for the official releases. The CPU platform runs much slower without
    these optimized libraries.


 1. Run CMake on the fahbench source directory. Finagle it until it has
    found all of the dependencies.
    Setting the following may help:
     
     - `CMAKE_PREFIX_PATH` to `.../Qt/5.6/msvc2015_64/`

 1. Open the generated solution file (`.sln`) and "build" the `INSTALL` target. 

### Vagrant

 1. Make sure your git submodules are initialized (boost, openmm)

 1. Download a `.tar.bz2` release of the AMDAPPSDK into this directory.
    The provisioning script can't download it because you have to accept
    a license agreement through a web browser. The file has to be of the form
    `AMD-APP-SDK-*.tar.bz2`

 1. Run `vagrant up`.

 1. Enter the virtual machine with `vagrant ssh`

 1. Run `install-fahbench.sh`. Build artifacts
    will be copied to the `dist/` directory in the packaging subdirectory of the source repository.

