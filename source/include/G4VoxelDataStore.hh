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


#ifndef G4VOXELDATASTORE_H
#define G4VOXELDATASTORE_H

// G4VOXELDATA //
#include "G4VoxelData.hh"

class G4VoxelDataStore : public std::vector<G4VoxelData*>
{
  public:
    virtual ~G4VoxelDataStore();
    
    static G4VoxelDataStore* GetInstance();
    static void Register(G4VoxelData* voxel_data);
    static void DeRegister(G4VoxelData* voxel_data);
    static void Clean();

//    G4VoxelData* GetVoxelData(const G4String& name, G4bool verbose=true) const;

  protected:
    //G4VoxelDataStore() : instance(0), locked(0) {
    G4VoxelDataStore();

  private:
    static G4VoxelDataStore* instance;
    static G4bool locked;
};

#endif // G4VOXELDATASTORE_H

