---
title: Custom work units
layout: page
---

The parameters of a simulation are specified with three files from a
standard Folding@Home work unit (WU):
 
 - system.xml
 - integrator.xml
 - state.xml

as well as a FAHBench-specific file named `wu.json`. This file encodes the
number of steps to perform and other meta-information. 

Workunits are installed to
`[install dir]/workunits/` on Windows and 
`[prefix]/share/fahbench/workunits` on Linux.
The workunit files are installed in subdirectories `[wuname]/{*.xml, *.json}`.
To make your own workunit, put the four required files in an analogous subdirectory.

`wu.json` fields
----------------

 - `codename` should match the directory name of the installed workunit
 - `protein.name` can give a "full" name for a particular protein or workunit
 - `protein.description` can give a brief description of the system
 - `step_chunk` sets how many integration steps to do before refreshing the
   current performance estimate and % completed. A higher value will make the
   UI less responsive. Too low of a value may impact performance. Simpler
   systems should increase this value, while difficult systems may reduce it.
   Default: 10
 - Other fields are ignored.


Source tree
-----------

The workunits are stored compressed in the source tree. They will be extracted
at build time via CMake "external project". Developers can add WU's hosted
elsewhere via this mechanism.

