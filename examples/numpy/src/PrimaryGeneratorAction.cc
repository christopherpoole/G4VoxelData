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


// USER //
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

// GEANT4 //
#include "globals.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    particle_gun = new G4ParticleGun();

    G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particle_table->FindParticle("e-");
  
    particle_gun->SetParticleDefinition(particle);
    particle_gun->SetParticlePosition(G4ThreeVector(0, -12*mm, 0*m));
    particle_gun->SetParticleEnergy(1.*MeV);
    particle_gun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));

    detector_construction = (DetectorConstruction*) (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particle_gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    particle_gun->GeneratePrimaryVertex(event);

    //detector_construction->SetEventID(event->GetEventID());

    //if (event->GetEventID() % 10000 == 0)
    //    G4cout << event->GetEventID() << G4endl;
}


