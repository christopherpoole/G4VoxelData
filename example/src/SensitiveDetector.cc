//////////////////////////////////////////////////////////////////////////
// License & Copyright
// ===================
// 
// Copyright 2012 Christopher M Poole <mail@christopherpoole.net>
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


#include "globals.hh"

#include "SensitiveDetector.hh"
#include "DetectorConstruction.hh"


#include "G4ProcessType.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"

#include <vector>


SensitiveDetector::SensitiveDetector(const G4String& name) : G4VSensitiveDetector(name) {
    debug = true;
    detector_construction = (DetectorConstruction*) (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
}

SensitiveDetector::~SensitiveDetector() {
}

void SensitiveDetector::Initialize(G4HCofThisEvent*) {
}

//G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* touchable) {
G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    const G4Track* aTrack = aStep->GetTrack();
    const G4String name = aTrack->GetDefinition()->GetParticleName();

//    G4double energy_deposit = aStep->GetTotalEnergyDeposit();

//    if(energy_deposit == 0) {
//        if (debug) G4cout << "No enegy to deposit." << G4endl << G4endl;
//        return false;
//    }

//    G4double voxel_density = aTrack->GetMaterial()->GetDensity();

//    G4ThreeVector position = aTrack->GetPosition();

//    if (debug) {
//        G4cout << "Solid name:       " << aTrack->GetVolume()->GetLogicalVolume()->GetName() << G4endl;
//        G4cout << "Total energy:     " << aTrack->GetTotalEnergy() << G4endl;
//        G4cout << "Enegy to deposit: " << energy_deposit << " MeV" << G4endl;
        G4cout << "Voxel material:   " << aTrack->GetMaterial()->GetName() << G4endl;
//        G4cout << "Voxel density:    " << voxel_density << G4endl;
//        G4cout << "Position: "
//               << position.x() << " "
//               << position.y() << " "
 //              << position.z() << " " << G4endl;
//    }

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
}

void SensitiveDetector::clear() {
}

void SensitiveDetector::PrintAll() {
}
