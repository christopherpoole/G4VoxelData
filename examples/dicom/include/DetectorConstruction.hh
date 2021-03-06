//////////////////////////////////////////////////////////////////////////
// G4VoxelData
// ===========
// A general interface for loading voxelised data as geometry in GEANT4.
//
// Author:  Christopher M Poole <mail@christopherpoole.net>
// Source:  http://github.com/christopherpoole/G4VoxelData
//
// License & Copyright
// ===================
// 
// Copyright 2013 Christopher M Poole <mail@christopherpoole.net>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////



#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include <vector>
#include <inttypes.h>


#include "G4VoxelData.hh"

// GEANT4 //
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"


// Simple data structure for setpoints in hounsfiled -> G4Material ramp
class Hounsfield{
  public:
    Hounsfield(int value, G4String material_name, G4double density) {
        this->value = value;
        this->material_name = material_name;
        this->density = density;
    };

  public:
    G4int value;
    G4String material_name;
    G4double density;
};


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4String dir);
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    std::map<int16_t, G4Material*> MakeMaterialsMap(G4int increment);
    G4Material* MakeNewMaterial(G4String base_material_name, G4double density);


  private:
    //SensitiveDetector* detector;
    G4ThreeVector interaction_origin;
    G4int event_id;

    G4Box* world_solid;
    G4LogicalVolume* world_logical;
    G4VPhysicalVolume* world_physical;
    
    // Materials map setpoints
    std::vector<Hounsfield> hounsfield;

    // Directory of DICOM CT to load
    G4String dir;
};
#endif

