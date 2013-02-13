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


// G4VOXELDATA //
#include "G4VoxelDataStore.hh"

// STL //
#include <vector>

// GEANT4 //
#include "globals.hh"


G4VoxelDataStore* G4VoxelDataStore::instance = 0;
G4bool G4VoxelDataStore::locked = 0;

G4VoxelDataStore* G4VoxelDataStore::GetInstance() {
    static G4VoxelDataStore voxel_data_store;
    if (!instance)
    {
        instance = &voxel_data_store;
    }
    return instance;
}

void G4VoxelDataStore::Register(G4VoxelData* voxel_data) {
    GetInstance()->push_back(voxel_data);
}

void G4VoxelDataStore::DeRegister(G4VoxelData* voxel_data) {
    if (!locked) {
        G4VoxelDataStore* store = GetInstance();
        
        for (iterator current=store->begin(); current!=store->end(); current++) {
            //if (**current == *voxel_data) {
            //    store->erase(current);
            //    break;
            //}
        }
    }
}

void G4VoxelDataStore::Clean() {
    locked = true;  

    size_t i=0;
    G4VoxelDataStore* store = GetInstance();

    for(iterator current=store->begin(); current!=store->end(); current++) {
        if (*current) {
            delete *current;
        }
        i++;
    }

    locked = false;
    store->clear();
}

//    G4VoxelData* GetVoxelData(const G4String& name, G4bool verbose=true) const;

G4VoxelDataStore::~G4VoxelDataStore() {
    Clean();
}

//G4VoxelDataStore() : instance(0), locked(0) {
G4VoxelDataStore::G4VoxelDataStore() {
    reserve(10);
}


