Generic voxelised data loader for GEANT4
========================================

Dependencies
------------
* GEANT4
* GDCM 2.2.1

Compiling/Running the Example
-----------------------------
Make sure you have some DICOM CT data in a directory:

    cd example/
    mkdir build
    cd build/
    cmake ..
    make
    cd ..
    ./build/VoxelDataExample /path/to/dicom_ct/

BeamOn is called from `macro/vis.mac`, when running, you will get some output from the sensitive detector printing the name of the current material. Note that the material ramp in this example is arbitrary and not suitable for use in anything real (yet).
