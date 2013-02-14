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
#include "SensitiveDetector.hh"

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
#include "DicomDataIO.hh"
#include "G4VoxelDataParameterisation.hh"


DetectorConstruction::DetectorConstruction(G4String dir)
{
    // Setpoints we will interpolate between for
    // our materials ramp.
    hounsfield.push_back(Hounsfield(-1050, "G4_AIR", 0.001 ));
    hounsfield.push_back(Hounsfield(-950,"G4_AIR", 0.044));
    hounsfield.push_back(Hounsfield(-700,"G4_LUNG_ICRP", 0.302));
    hounsfield.push_back(Hounsfield(125,"G4_TISSUE_SOFT_ICRP", 1.101));
    hounsfield.push_back(Hounsfield(2500,"G4_BONE_CORTICAL_ICRP", 2.088));

    this->dir = dir;
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");

    world_solid = new G4Box("world_solid", 200*cm, 200*cm, 200*cm);
    world_logical = new G4LogicalVolume(world_solid, air, "world_logical", 0, 0, 0);
    world_physical = new G4PVPlacement(0, G4ThreeVector(), world_logical, "world_physical", 0, false, 0);
    world_logical->SetVisAttributes(G4VisAttributes::Invisible);

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
//    new G4PVParameterised("PatientVoxelPhys", voxel_logical, voxeldata_placement->x_logical,
//                          kUndefined, data->shape[2], voxeldata_param);

    SensitiveDetector* detector = new SensitiveDetector("target_detector");

    G4SDManager* sd_manager = G4SDManager::GetSDMpointer();
    sd_manager->AddNewDetector(detector);
    voxeldata_param->GetLogicalVolume()->SetSensitiveDetector(detector);

    return world_physical;
}


std::map<int16_t, G4Material*> DetectorConstruction::MakeMaterialsMap(G4int increment) {
    // Our materials map or ramp
    std::map<int16_t, G4Material*> ramp;
    
    // Calculate intermediate points in each segment
    for (unsigned int i=0; i <hounsfield.size()-1; i++) { 
        G4double hounsfield_rise = hounsfield[i+1].density - hounsfield[i].density;
        G4double density_run = hounsfield[i+1].value - hounsfield[i].value;
        G4double gradient = hounsfield_rise / density_run;

        // Add each increment in the current segment to the ramp  
        int count = 0;
        for (int hf=hounsfield[i].value; hf<hounsfield[i+1].value; hf+=increment) {
            G4double density = count*increment*gradient + hounsfield[i].density;
            ramp[hf] = MakeNewMaterial(hounsfield[i].material_name, density);            
            count++;
        }
    } 
    // Add the last setpoint to the ramp
    ramp[hounsfield.back().value] = MakeNewMaterial(hounsfield.back().material_name, 
                                                    hounsfield.back().density);
    return ramp;
}


G4Material* DetectorConstruction::MakeNewMaterial(G4String base_material_name, G4double density) {
    G4NistManager* nist_manager = G4NistManager::Instance();
    G4String new_name = base_material_name + G4UIcommand::ConvertToString(density);

    return nist_manager->BuildMaterialWithNewDensity(new_name, base_material_name, density*g/cm3);
}   
