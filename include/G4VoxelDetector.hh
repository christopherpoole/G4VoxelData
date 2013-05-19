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


#ifndef _SensitiveDETECTOR_HH
#define	_SensitiveDETECTOR_HH

// G4VoxelData //
#include "G4VoxelData.hh"
#include "G4VoxelArray.hh"

// STL //
#include <vector>

// GEANT4 //
#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"


class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;
class DetectorConstruction;


template <typename T>
class G4VoxelDetector : public G4VSensitiveDetector {
  public:
    G4VoxelDetector(G4String name, std::vector<unsigned int> shape,
            std::vector<double> spacing) : G4VSensitiveDetector(name) {
        debug = false;

        this->shape = shape;
        this->spacing = spacing;

        energy_histogram = new G4VoxelArray<T>(shape, spacing);     // Total energy
        energysq_histogram = new G4VoxelArray<T>(shape, spacing);   // Sum of squares
        counts_histogram = new G4VoxelArray<T>(shape, spacing);     // Total hits
    };

    virtual ~G4VoxelDetector() {
        
    };

    void Initialize(G4HCofThisEvent*) {};
    void EndOfEvent(G4HCofThisEvent*) {};
    void clear() {};
    void PrintAll() {};

    void SetDebug(G4bool debug) {
        this->debug = debug;
    };

    G4VoxelArray<T>* GetEnergyHistogram() {
        return this->energy_histogram;
    }

    G4VoxelArray<T>* GetEnergySqHistogram() {
        return this->energysq_histogram;
    }

    G4VoxelArray<T>* GetCountsHistogram() {
        return this->counts_histogram;
    }

    G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) {
        const G4Track* aTrack = aStep->GetTrack();
        const G4String name = aTrack->GetDefinition()->GetParticleName();

        G4double energy_deposit = aStep->GetTotalEnergyDeposit();

        if(energy_deposit == 0) {
            if (debug) G4cout << "No enegy to deposit." << G4endl << G4endl;
            return false;
        }

        //G4double voxel_mass = aTrack->GetMaterial()->GetDensity() * volume;
        G4ThreeVector world_position = aTrack->GetPosition();
        G4ThreeVector position = aStep->GetPreStepPoint()->GetTouchableHandle()->
            GetHistory()->GetTopTransform().TransformPoint(world_position);

        int x_index = std::floor((position.x() + (shape[0]/2. * spacing[0])) / spacing[0]);
        int y_index = std::floor((position.y() + (shape[1]/2. * spacing[1])) / spacing[1]);
        int z_index = std::floor((position.z() + (shape[2]/2. * spacing[2])) / spacing[2]);

        if (debug) {
            G4cout << "Solid name:       " << aTrack->GetVolume()->GetLogicalVolume()->GetName() << G4endl;
            G4cout << "Total energy:     " << aTrack->GetTotalEnergy() << G4endl;
            G4cout << "Enegy to deposit: " << energy_deposit << " MeV" << G4endl;
            G4cout << "Voxel material:   " << aTrack->GetMaterial()->GetName() << G4endl;
            //G4cout << "Voxel mass:       " << voxel_mass << G4endl;
            //G4cout << "Voxel volume:     " << volume << G4endl;
            G4cout << "Position: "
                   << position.x() << " "
                   << position.y() << " "
                   << position.z() << " " << G4endl;
            G4cout << "Histogram index: "
                   << x_index << " "
                   << y_index << " "
                   << z_index << " " << G4endl;
            if (x_index != std::abs(x_index)) {
                G4cout << "Error: X index" << G4endl;
            }
            if (y_index != std::abs(y_index)) {
                G4cout << "Error: Y index" << G4endl;
            }
            if (z_index != std::abs(z_index)) {
                G4cout << "Error: Z index" << G4endl;
            }
            G4cout << G4endl; // blank line
        }
        
        energy_histogram->IncrementValue(energy_deposit, x_index, y_index, z_index);
        energysq_histogram->IncrementValue(pow(energy_deposit, 2), x_index, y_index, z_index);
        counts_histogram->IncrementValue(aTrack->GetWeight(), x_index, y_index, z_index);

        return true;
    };

public:

    G4VoxelArray<T>* energy_histogram;
    G4VoxelArray<T>* energysq_histogram;
    G4VoxelArray<T>* counts_histogram;

    G4bool debug;
    G4double volume;
    
    std::vector<unsigned int> shape;
    std::vector<double> spacing;
};


#endif	/* _SensitiveDETECTOR_HH */

