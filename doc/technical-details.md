---
title: Technical Details
layout: page
---

FAHBench is the official Folding@Home benchmark. Like the Folding@Home
"cores" being executed by hundres of thousands of donors across the world
to solve hard problems in protein dynamics, FAHBench is built on the
molecular dynamics engine [OpenMM]. FAHBench works on any OpenCL device.
FAHBench is available as a GUI or command-line based application for
Windows and Linux. It is licensed under GPLv2.

[OpenMM]: http://openmm.org

Overview
--------

The benchmark runs a short molecular dynamics (MD) simulation on various
systems. The score is the number of MD steps completed per unit time.

The score depends strongly on the size of the system being simulated.
Traditionally, explicit solvent DHFR (~20k atoms) is the benchmarking
system of choice. FAHBench includes other systems ("workunits") for
benchmarking. A **scaled score** attempts to compare results on
different-sized systems to DHFR. Please take this scaled score with a grain
of salt.

Details
-------

### Units

The score is quantified with the peculiar unit of "nanoseconds per day".
This gives the ratio of simulated time to real-life time.

The theoretical scaling of molecular dynamics is `N log N` (where N is the
number of atoms). This factor is used to provide a "scaled score"
comparable to a DHFR-sized system. In practice, scaling is rarely `N log
N`.

### The CPU Platform

OpenMM supports CPU calculations as well, albeit slower than MD engines
that focus on CPU calculations. CPU workunits on Folding@Home are performed
with [Gromacs], a more performant CPU code.

The pre-compiled Windows releases do not include an optimized `fftw3`
library because it's difficult to compile on Windows. This means the CPU
platform will run very slowly on Windows.

[Gromacs]: http://gromacs.org

