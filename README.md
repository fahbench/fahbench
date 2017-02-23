FAHBench
========

FAHBench is the official Folding@Home benchmark. Like the Folding@Home
"cores" being executed by hundres of thousands of donors across the world
to solve hard problems in protein dynamics, FAHBench is built on the
molecular dynamics engine [OpenMM]. FAHBench works on any OpenCL device, as
well as CUDA-capable Nvidia GPUs. FAHBench is available as a GUI or
command-line based application for Windows and Linux. It is licensed under
GPLv2.

Technical Overview
------------------

The benchmark runs a short molecular dynamics (MD) simulation
on various systems. The score is the number of MD steps completed per unit
time.

The score depends strongly on the size of the system being simulated.
Traditionally, explicit solvent DHFR (~20k atoms) is the benchmarking
system of choice. FAHBench includes other systems ("workunits") for
benchmarking. A scaled score is reported to compare results on
different-sized systems to DHFR. Please take this scaled score with a grain of salt.

Technical Details
-----------------

### Units

The score is quantified with the peculiar unit of "nanoseconds per day".
This gives the ratio of simulated time to real-life time.

The theoretical scaling of molecular dynamics is `N log N` (where N is the
number of atoms). This factor is used to provide a "scaled score"
comparable to a DHFR-sized system. In practice, scaling is rarely `N log
N`.

### The CPU Platform

OpenMM supports CPU calculations as well. This capability is not included
in the released binaries. You can compile OpenMM with the CPU platform
enabled and test it with the `--platform CPU` argument on the command line.
Please know that CPU calculations on Folding@Home are performed with
[GROMACS](http://gromacs.org), a more performant CPU code.

### Custom work units

The parameters of a simulation are specified with three files from a
standard Folding@Home work unit (WU):
 
 - system.xml
 - integrator.xml
 - state.xml

as well as a FAHBench-specific file named `wu.json`. This file encodes the
number of steps to perform and other meta-information. To install a
custom work unit, copy these four files into a subdirectory of
`share/fahbench/workunits/`.


Building
--------

FAHBench is built with [CMake] and requires the following external libraries:

 - OpenCL - linked to find OpenCL devices.
 - CUDA runtime - *optional* - linked to find CUDA devices.
 - [Qt5] - *optional* - for building the GUI.
 
We also vendor [Boost] and [OpenMM] as submodules.

[CMake]: http://www.cmake.org/
[Boost]: http://www.boost.org/
[Qt5]: http://qt-project.org/
[OpenMM]: http://openmm.org/

### Linux

 1. Get the prerequisites

    ```bash
    sudo apt-get install    \
        build-essential     \
        mesa-common-dev     \
        fftw3-dev           \
        qt5-default
    ```

 1. From a clean build directory

    ```bash
    ccmake [source_dir]
    # make sure all dependencies are found
    # type c - configure, g - generate
    make
    make install
    ```

### Windows

 1. Download and install 
      - Visual Studio Community 2015
      - Qt > 5.6
      - CMake > 3.6
      - AMD APP SDK
      - (Optional) CUDA SDK

    These all have nice GUI installers.

 1. Run CMake on the fahbench source directory. Finagle it until it has
    found all of the dependencies you just spent so long getting in order.
    Some settings that may help:
     
     - `CMAKE_PREFIX_PATH` to `.../Qt/5.6/msvc2015_64/`

 1. Build and install! CMake will copy the relevant OpenMM and Qt `dll`s

### Vagrant

 1. Make sure your git submodules are initialized (openmm)

 1. Download a `.tar.bz2` release of the AMDAPPSDK into the packaging directory.
    The provisioning script can't download it because you have to accept
    a license agreement through a web browser. The file has to be of the form
    `AMD-APP-SDK-*.tar.bz2`

 1. Run `vagrant up`.

 1. Enter the virtual machine with `vagrant ssh`

 1. Run `install-fahbench.sh`. Build artifacts
    will be copied to the `dist/` directory in the packaging directory.


