Work Units (WUs)
================

Each WU must have:
 - system.xml
 - state.xml
 - integrator.xml
 - wu.json

Workunits are installed uncompressed to
`[prefix]/share/fahbench/workunits/[wuname]/{*.xml, *.json}`.
To install your own workunit, dump the four required files
there.

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

The workunits are stored compressed. They will be extracted at build time
via CMake "external project". You can add WU's hosted elsewhere via this
mechanism.

