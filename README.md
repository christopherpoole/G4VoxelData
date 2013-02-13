Generic voxelised data loader for GEANT4
========================================

Dependencies
------------
* GEANT4
* GDCM 2.2.1

Compiling/Running the Example
-----------------------------
First edit the detector construction, with the `dir` variable naming a directory with some DICOM CT files in it; this is currently hardcoded and will segfault without it.

    cd example/
    mkdir build
    cd build/
    cmake ..
    make
    cd ..
    ./build/VoxelDataExample
