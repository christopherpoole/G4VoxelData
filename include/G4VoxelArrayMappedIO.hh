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


#ifndef G4VOXELARRAYMAPPEDIO_H
#define G4VOXELARRAYMAPPEDIO_H

// G4VOXELDATA //
#include "G4VoxelData.hh"
#include "G4VoxelArray.hh"

// GEANT4 //
#include "globals.hh"


class G4VoxelArrayMappedIO {
  public:
    G4VoxelArrayMappedIO() {};
    ~G4VoxelArrayMappedIO() {};

  public:
    virtual G4VoxelArray* Read(G4String) {
        G4Exception("G4VoxelData::Read", "Reading data not implemented.",
                FatalException, "");

        return NULL;
    };

    virtual void Write(G4String, G4VoxelArray*) {
        G4Exception("G4VoxelData::Write", "Writing data not implemented.",
                FatalException, "");
    };
};

#endif // G4VOXELARRAYMAPPEDIO_H

