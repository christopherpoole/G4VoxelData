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


#ifndef NIFTIMAPPEDIO_H
#define NIFTIMAPPEDIO_H

// G4VOXELDATA //
#include "G4VoxelArray.hh"

// NIFTI //
#include "H5Cpp.h"

// GEANT4 //
#include "globals.hh"


template <typename T>
class NIFTIMappedIO : public G4VoxelArray<T> {
  public:
    using G4VoxelArray<T>::Init;
    using G4VoxelArray<T>::GetIndex;
    using G4VoxelArray<T>::UnpackIndices;
    
    NIFTIMappedIO<T>() {
    };
  
  public:
    void Read(G4String filename, G4String dataset_name) {
        Init();
    };

    T GetValue(unsigned int index) {
        return GetValue(indices);
    };

    //virtual void Write(G4String, G4MappedVoxelArray*) {
    //    G4Exception("G4VoxelData::Write", "Writing data not implemented.",
    //            FatalException, "");
    //};

  private:
};

#endif // NIFTIMAPPEDIO_H

