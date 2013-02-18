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


#include <iostream>

// USER //
#include "DetectorConstruction.hh"

// GEANT4 //
#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4UserLimits.hh"
#include "G4UIcommand.hh"


// G4VoxelData //
#include "NumpyDataIO.hh"
#include "G4VoxelDataParameterisation.hh"


DetectorConstruction::DetectorConstruction(G4String filename)
{
    this->filename = filename;
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");
    G4Material* water = nist_manager->FindOrBuildMaterial("G4_WATER");

    world_solid = new G4Box("world_solid", 200*cm, 200*cm, 200*cm);
    world_logical = new G4LogicalVolume(world_solid, air, "world_logical", 0, 0, 0);
    world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical,
            "world_physical", 0, false, 0);
    world_logical->SetVisAttributes(G4VisAttributes::Invisible);

    NumpyDataIO* reader = new NumpyDataIO(); 
    G4VoxelData* data = reader->Read(filename);

    G4VoxelArray<std::complex<uint8_t> >* array =
        new G4VoxelArray<std::complex<uint8_t> >(data);
    array->spacing.push_back(1);
    array->shape.push_back(1);
    array->ndims += 1;

    std::map<uint8_t, G4Material*> materials;
    for (int i=0; i<256; i++) {
        materials[i] = air;
    }
    G4VoxelDataParameterisation<std::complex<uint8_t>, uint8_t>* voxeldata_param =
        new G4VoxelDataParameterisation<std::complex<uint8_t>, uint8_t>(array, materials,
                                                                world_physical);
    voxeldata_param->Construct(G4ThreeVector(), new G4RotationMatrix());

    return world_physical;
}
