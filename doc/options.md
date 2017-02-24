---
title: Configuration and Options
layout: page
image: cogwheel32.png
footer: "Icon made by Freepik from www.flaticon.com "
---


 - **Device** - This lists all available OpenCL devices. If you don't see
   your GPU listed, make sure you have updated graphics drivers.
   You might see your CPU listed as an OpenCL device. Folding@Home
   never runs in this configuration, but you can try it with FAHBench.
   Be aware that it [might not work][opencl-on-cpu].

 - **Compute** - This lets you select whether to use `OpenCL` (like
   Folding@Home) to benchmark your GPU or `CPU` to run a calculation on
   your CPU ([not like][cpu-platform] Folding@Home).

 - **Precision** - Computers have to round numbers after a certain point
   (e.g. when tracking the position or velocity of an atom). `single`
   precision uses more rounding for less accurate but higher speed results.
   `double` uses less rounding, which is more accurate, but slower.
   Folding@Home almost always uses single precision.
   [Details][precision]

 - **WU** (Work Unit) - This bit of Folding@Home terminology refers to a
   particular protein system. In FAHBench, each workunit is given a
   codename which is displayed. The dropdown menu shows a full name of the
   protein and a description. WUs determine whether the simulation is in
   [explicit or implicit][solvent] solvent.

 - **Accuracy Check** - If checked, FAHBench will run a reference
   calculation to make sure your device is reporting accurate values.
   Folding@Home performs this check periodically.

 - **NaN Check** - If set to a number greater than 0, FAHBench will make
   sure the simulation has not exploded every this-many steps. If it is set
   to `0 - Disabled`, the check is still run at the end of a benchmarking
   run.

 - **Run length** - This is the approximate time the benchmarking will run.
   Set to a higher value for a more precise performance estimate or to
   stress test a device. Note that the total benchmarking run includes some
   set up calculations which may last several minutes in addition to the
   length of the benchmarking run.

Results
-------

 - **Score** - The ratio of simulated time to real-world time.
 
 - **Scaled Score** - The above ratio [adjusted][scaled-score] to the size
   of the protein system.

 - **Atoms** - The number of atoms, i.e. the size of the protein system.


[opencl-on-cpu]: {{site.url}}/details.html#opencl-on-cpu
[cpu-platform]:  {{site.url}}/details.html#cpu-platform
[precision]:     {{site.url}}/details.html#precision
[solvent]:       {{site.url}}/details.html#solvent
[scaled-score]:  {{site.url}}/details.html#scaled-score

