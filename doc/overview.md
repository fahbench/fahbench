---
title: Overview
layout: page
image: fah-bench.jpg
---

FAHBench is the official Folding@Home benchmark. Like the Folding@Home
"cores" being executed by hundreds of thousands of donors across the world
to solve hard problems in protein dynamics, FAHBench is built on the
molecular dynamics engine [OpenMM]. FAHBench works on any OpenCL device.
FAHBench is available as a GUI or command-line based application for
Windows and Linux. It is licensed under GPLv2.

[OpenMM]: http://openmm.org

The Task
--------

The benchmark consists of a short molecular dynamics (MD) simulation of
proteins. Molecular dynamics simulates how each atom moves over time. The
score represents how much simulated time your computer can process
in a given unit of 'real' (wallclock) time. Higher is better.

Since there are many thousands of atoms being simulated,
molecular dynamics benefits from the
parallelism offered by commodity GPUs. Instead of rendering many triangles
every second for a video game, these devices can compute the trajectories
of many atoms quickly. A molecular dynamics computation fully exploits the
power of the GPU while still being a relevant, real-world problem, making
it an excellent benchmark.

Traditionally in the MD community, a protein named DHFR in a [box full of
water molecules][solvent] has served as the benchmark of choice.  FAHBench
includes this protein system and [others][workunits] to see how your
hardware deals with different sized simulations. To compare performance
among the different simulation systems, FAHBench reports a [scaled
score][scaled-score] in addition to the raw score.


[opencl-on-cpu]: {{site.url}}/details.html#opencl-on-cpu
[cpu-platform]:  {{site.url}}/details.html#cpu-platform
[precision]:     {{site.url}}/details.html#precision
[solvent]:       {{site.url}}/details.html#solvent
[scaled-score]:  {{site.url}}/details.html#scaled-score
[workunits]:     {{site.url}}/details.html#workunits
