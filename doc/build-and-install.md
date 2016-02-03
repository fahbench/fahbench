---
title: Building and Installing
layout: page
---

FAHBench is built with [CMake] and requires the following libraries:

 - [Boost] (*automatically downloaded and built*)
 - [OpenMM] ([source], [with fah patches]) contain the scientific code to run molecular
   dynamics calculations.
 - OpenCL must be installed on user machines to drive GPU calculations.
 - CUDA (*optional*) can be leveraged to drive GPU calculations on nvidia
   hardware. If the CUDA runtime is not available on user machines,
   FAHBench will gracefully disable CUDA as an option. The released
   binaries aren't compiled to support CUDA at all. Folding@Home does not
   use CUDA.
 - [Qt5] (*optional*) is necessary for the GUI.

[CMake]: http://www.cmake.org/
[Boost]: http://www.boost.org/
[Qt5]: http://qt-project.org/
[OpenMM]: http://openmm.org/
[source]: http://github.com/pandegroup/openmm/
[with fah patches]: http://github.com/pandegroup/openmm/

Linux
-----

 1. Get the prerequisites

    ```
    sudo apt-get install qt5-default nvidia-cuda-dev nvidia-opencl-dev
    ```

 1. Configure an OpenMM build with CMake.

 1. From a clean build directory, use `ccmake` to configure the build with your
    OpenCL, OpenMM, and Qt libraries.

 1. Run `make && make install`!


### Windows

 1. Download and install 
      - Visual Studio Community 2015
      - Qt 5.6 (other releases do not include binaries for vs2015 but should work)
      - CMake > 3.1
      - AMD APP SDK
      - (Optional) CUDA SDK

    These all have nice GUI installers.

 1. (Optional) Download and build fftw3 for the CPU platform. I haven't done
    this for the official releases. The CPU platform runs much slower without
    these optimized libraries.

 1. Download OpenMM and build with CMake. The official binaries will not work
    with vs2015, and do not include fah-specific patches and backports. Build
    the release configuration. You can disable building the python bindings, C
    and Fortran bindings, drude, and amoeba plugins to simplify the build.

 1. Build the `INSTALL` project to install OpenMM.

 1. Run CMake on the fahbench source directory. Finagle it until it has
    found all of the dependencies you just spent so long getting in order.
    Start by setting:
     
     - `CMAKE_PREFIX_PATH` to `.../Qt/5.6/msvc2015_64/`
     - `OPENMM_xxx` to where you installed OpenMM.

 1. Build and install! CMake will copy the relevant OpenMM and Qt `dll`s to
    the `bin/` install directory.

### Vagrant

 1. Make sure your git submodules are initialized (openmm)

 1. Download a `.tar.bz2` release of the AMDAPPSDK into this directory.
    The provisioning script can't download it because you have to accept
    a license agreement through a web browser. The file has to be of the form
    `AMD-APP-SDK-*.tar.bz2`

 1. Run `vagrant up`.

 1. Enter the virtual machine with `vagrant ssh`

 1. Run `install-openmm.sh`, then `install-fahbench.sh`. Build artifacts
    will be copied to the `dist/` directory in this repository.

