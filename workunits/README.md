Work Units (WUs)
================

This directory contains several sample work-units to run benchmarking.

Each WU must have:
    - system.xml
    - state.xml
    - integrator.xml
    - wu.json

The workunits are stored compressed. They will be extracted at build time
via CMake "external project". You can add WU's hosted elsewhere via this
mechanism.
