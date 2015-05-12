FAHBench
========

FAHBench is the official Folding@Home benchmark. Like the Folding@Home
"cores" being executed by hundres of thousands of donors across the world
to solve hard problems in protein dynamics, FAHBench is built on the
molecular dynamics engine [OpenMM]. FAHBench works on any OpenCL device, as
well as CUDA-capable Nvidia GPUs. FAHBench is available as a GUI or
command-line based application for Windows and Linux. It is licensed under
GPLv2.

Technical details
-----------------

The benchmark consists of a set number steps of molecular dynamics (MD) on
the enzyme dihydrofolate reductase (DHFR) in all-atom detail. This
scientific computation is performed by [OpenMM], an MD engine highly-tuned
for the extreme parallelism afforded by GPUs. This application also allows
    you to run arbitrary atomistic simulations by supplying your own OpenMM
    input files. Note that the results of these simulations are not saved,
    only the performance value is reported. 

Speed is quantified with the peculiar unit of "nanoseconds per day". This
gives the ratio of simulated time to real-life time.

### The CPU Platform

OpenMM supports CPU calculations as well. You can test this with the
`--platform CPU` argument on the command line. Please know that CPU
calculations on Folding@Home are performed with
[GROMACS](http://gromacs.org), a more performant CPU code.

### Custom input files

The parameters of a simulation are specified with three files:
 
 - system.xml
 - integrator.xml
 - state.xml

These files are identical to those distributed as part of a Folding@Home
work unit (WU). They can be generated via OpenMM's `XmlSerializer` class.

### Custom OpenMM versions

Binary packages include the OpenMM library as well as the CUDA and OpenCL
plugins. You can [build your own copy of OpenMM] and replace or supliment
the OpenMM library and plugins.

Plugins are installed under `lib/plugins/`. The OpenMM library is installed
under `lib/libOpenMM.so` on Linux and `bin/OpenMM.dll` on Windows.

Building
--------

FAHBench is built with [CMake] and requires the following libraries:

 - [Boost]
 - [OpenMM] - molecular dynamics calculations.
 - OpenCL - linked to find OpenCL devices.
 - CUDA runtime - *optional* - linked to find CUDA devices.
 - [Qt5] - *optional* - for building the GUI.

[CMake]: http://www.cmake.org/
[Boost]: http://www.boost.org/
[Qt5]: http://qt-project.org/
[OpenMM]: http://openmm.org/
[build your own copy of OpenMM]: http://github.com/pandegroup/openmm/

### Linux

 1. Get the prerequisites

    ```bash
    sudo apt-get install \
        libboost-dev libboost-program-options-dev libboost-filesystem-dev \
        qt5-default \
        nvidia-cuda-dev nvidia-opencl-dev
    ```

 1. Get OpenMM. TODO: Further instructions

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
	  - Visual Studio Community 2013
	  - Qt > 5.2
	  - CMake > 2.8.11
	  - CUDA SDK = 6.5

	These all have nice GUI installers.

 2. Download Boost. You actually have to extract the archive, and build it
    from the command line. We use some modules that are not header-only.

	```batch
	bootstrap
	.\b2 address-model=64  
	REM need 64 bit ^^. 
	```

 3. Download OpenMM and build with CMake. I don't think the provided
    binaries (VC2010) will work. Build the release configuration. You can
    disable building the python bindings (which may be a source of build
    errors). Build the `INSTALL` project to install OpenMM.

 4. Run CMake on the fahbench source directory. Finagle it until it has
    found all of the dependencies you just spent so long getting in order.
    Start by setting:
	 
     - `CMAKE_PREFIX_PATH` to `.../Qt/5.4/msvc2013_64/`
	 - `BOOST_ROOT` to where you compiled Boost.
	 - `OPENMM_xxx` to where you installed OpenMM.

 5. Build and install! CMake will copy the relevant OpenMM and Qt `dll`s to
    the `bin/` install directory.

