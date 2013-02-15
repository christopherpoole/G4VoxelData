# A general interface for loading voxelised data as geometry in GEANT4.
Presently there is not an easy and generic way of loading voxelised datasets into GEANT4 as geometry. 
G4VoxelData aims to provide a consistent interface to various voxel data libraries (GDCM for DICOM for example) and load this data as nested parameterised volumes.
The library is headers only so no compilation or installation step is required by the user.

## Dependencies
* [GEANT4](http://www.geant4.org/) (only tested with 4.9.6 to date)
* [GDCM 2.2.1](http://gdcm.sourceforge.net/wiki/index.php/Main_Page) (for DICOM)

## Usage (User)
The fundamental type is `G4VoxelData` which is basically a container holding a `char*` of data long with some metadata including the actual type of the data (uint16 for example), the total length, shape and voxel spacing.
We populate `G4VoxelData` using an IO inheriting from `G4VoxelDataIO` which will provided a reader and/or writer for the target voxel data format; `DicomDataIO` will reader DICOM CT data using GDCM.

Given the inferred or known type of the data, voxelwise access (rather than bytewise for a `char*`) is available with `G4VoxelArray<sometype>(g4voxeldata)`.
Here we can access a value in the voxel data with `G4VoxelArray::GetValue(index)` for the raw value of type `sometype`, or we can use `G4VoxelArray::GetRoundedValue(index, `n`)` to access the raw value rounded to the nearest `n`; this is useful if our materials mapping between the values in the array at `G4Materials` is only defined in increments of `n`.
For basic usage a `std::map<sometype, G4Material*>` is required and must be created by the user.
In the example usage we have an array of Hounsfiled units (DICOM CT) which we wish to map to G4Materials of various density.

To place the data within a detector we use nested parameterised volumes, in pretty much the same way as described in GEANT4 example `extended/runAndEvent/RE02`.
Using `G4VoxelDataParameterisation<sometype>(g4voxelarray, materialsmap)`, the parameterisation is initialised; to place it within the geometry `parameterisation->Construct(position, rotation)` will find the geometry located at `G4ThreeVector position` and rotated by `G4RotationMatrix rotation`.

## Usage (Developer)
Developers can create their own voxel data readers or writers by inheriting from `G4VoxelDataIO`; the included DICOM reader `DicomDataIO` serves as an example usage.
The derived class must implement at least `G4VoxelDataIO::Read` and/or `G4VoxelDataIO::Write`.

## Compiling/Running the Example
For the DICOM example, all CT slices in a folder are sorted and loaded as a nested parameterised volume along with a user defined `std::map<int, G4Material*>`.
In a detector construction it looks like this:

    #include "DicomDataIO.hh"
    #include "G4VoxelDataParameterisation.hh"
    
    ...

    DicomDataIO* reader = new DicomDataIO(); 
    G4VoxelData* data = reader->ReadDirectory(dir);

    // We can peek at the data type with data->type, however at some point
    // we will have to nominate exactly what the type of the data is. For
    // standard DICOM CT as in this example we are using int16's.
    G4VoxelArray<int16_t>* array = new G4VoxelArray<int16_t>(data);

    // Make a mapping between the data in array and G4Materials
    // at increaments of 25 HU.
    G4int increment = 25;
    std::map<int16_t, G4Material*> materials = MakeMaterialsMap(increment);

    G4VoxelDataParameterisation<int16_t>* voxeldata_param =
        new G4VoxelDataParameterisation<int16_t>(array, materials, world_physical );
    voxeldata_param->Construct(G4ThreeVector(), new G4RotationMatrix());

Here the `MakeMaterialsMap` function is user defined and interpolates a full map of materials/densities at 25 HU intervals given a `std::vector` of change points such as:

    // This is not a good materials ramp, don't use it!
    hounsfield.push_back(Hounsfield(-1050, "G4_AIR", 0.001 ));
    hounsfield.push_back(Hounsfield(-950,"G4_AIR", 0.044));
    hounsfield.push_back(Hounsfield(-700,"G4_LUNG_ICRP", 0.302));
    hounsfield.push_back(Hounsfield(125,"G4_TISSUE_SOFT_ICRP", 1.101));
    hounsfield.push_back(Hounsfield(2500,"G4_BONE_CORTICAL_ICRP", 2.088));

Make sure you have some DICOM CT data in a directory:

    cd example/
    mkdir build
    cd build/
    cmake ..
    make
    cd ..
    ./build/VoxelDataExample /path/to/dicom_ct/

BeamOn is called from `macro/vis.mac`, when running, you will get some output from the sensitive detector printing the name of the current material. Note that the material ramp in this example is arbitrary and not suitable for use in anything real (yet).
