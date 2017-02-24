---
title: Technical Details
layout: page
image: sysoverview.png
---


<a name="scaled-score"></a> 

### Score <small>and Scaled Score</small>

The score is quantified with the peculiar unit of "nanoseconds per day".
This gives the ratio of simulated time to real-life time.

Traditionally, explicit solvent DHFR (~20k atoms) is the benchmarking
system of choice. FAHBench includes other systems ("workunits") for
benchmarking. A *scaled score* attempts to compare results on
different-sized systems to DHFR. Please take this scaled score with a grain
of salt.

The theoretical scaling of molecular dynamics is `N log N` (where N is the
number of atoms). This factor is used to scale the "scaled score" to get a
number comparable to that of a DHFR-sized system. In practice, scaling is
rarely `N log N`.

You might expect the scaling to be `N^2` because each particle feels an
effect from every other particle. MD calculations use a technique called
[particle mesh ewald][pme] to calculate the long-range pairwise
interactions in a clever way that reduces the scaling to `N log N`.

[pme]: https://en.wikipedia.org/wiki/Ewald_summation


<a name="precision"></a>

### Precision

Computers have to round numbers after a certain point (e.g. when tracking
the position or velocity of an atom). `single` precision uses 32-bit
floating point numbers (more rounding, less accurate), whereas `double`
uses 64-bit numbers (less rounding, more accurate).

Consumer GPUs are really bad at double precision calculations (so
manufacturers can sell more expensive enterprise units to researchers).
Luckily, molecular dynamics really only requires single precision to be
useful. Folding@Home uses single precision.


<a name="cpu-platform"></a>

### CPU Platform

OpenMM supports CPU calculations as well, albeit slower than MD engines
that focus on CPU calculations. CPU workunits on Folding@Home are performed
with [Gromacs], a more performant CPU code.

The pre-compiled Windows releases do not include an optimized `fftw3`
library because it's difficult to compile on Windows. This means the CPU
platform will run very slowly on Windows.

[Gromacs]: http://gromacs.org


<a name="opencl-on-cpu"></a>

### OpenCL on a CPU

OpenCL was designed as an open standard for heterogeneous, parallel
computing. This means you can run OpenCL programs ("kernels") on any piece
of hardware for which there is an OpenCL driver. If you have relevant CPU
OpenCL drivers, your processor might show up as an available device.

The differences in OpenCL implementations sometimes make the behavior of
programs unpredictable. OpenMM is mainly tested against AMD and Nvidia
implementations of OpenCL for GPUs. You may find running OpenCL runs on a
CPU break in unexpected ways. 

Folding@Home only runs on whitelisted GPUs. FAHBench will attempt to run on
anything.



<a name="solvent"></a>

### Explicit vs. Implicit Solvent

Proteins in biology are usually surrounded by water and ions, known as
solvent. In the past, researchers used approximations that avoided
including each solvent atom in the simulations. This means there are far
fewer atoms to simulate, but the results are not quite as accurate. This is
an implicit solvent model.

Now, computers and MD codes have gotten to the point where you might as
well include all solvent molecules in an explicit solvent model. 

FAHBench 1 had a binary toggle to select "implicit" or "explicit". Since
this is really a property of how the system was set up rather than an
implementation detail (contrast with precision, compute platform, device),
you must select specific implicit workunits to test implicit performance.
The workunit `dhfr-implicit` is in implicit solvent.



<a name="workunits"></a>

### Workunits

This is a FAH terminology that refers to a particular protein system,
usually corresponding to a FAH project number. A workunit fully specifies
the starting atomic positions and velocities. It includes the potential
energy function (force field) as well as the integration method.

FAHBench workunits differ slightly from Folding@Home workunits. The files
are not compressed or encrypted. It lacks the `core.xml` file but includes
a metadata file called `wu.json`.

You can create your own [custom workunits][custom-wu] by serializing xmls
from OpenMM or adapting a production Folding@Home workunit.

[custom-wu]: {{site.url}}/custom-wus.html



<a name="cuda"></a>

### Cuda

Nvidia GPUs can use the Cuda platform to perform GPU calculations. This may
be faster than OpenCL. Cuda functionality is not included in the official
releases of FAHBench, but you can compile it from source.

The Cuda platform requires the full Cuda SDK to compile the compute kernels
at runtime. Make sure `nvcc` is in your path and `libcuda` can be found.


