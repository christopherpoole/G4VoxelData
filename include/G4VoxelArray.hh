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


#ifndef G4VOXELARRAY_H
#define G4VOXELARRAY_H

// G4VOXELDATA //
#include "G4VoxelData.hh"

// STL //
#include <vector>

// GEANT4 //
#include "globals.hh"


template <typename T>
class G4VoxelArray {
  public:
    G4VoxelArray(G4VoxelData* data) {
        this->length = data->length / sizeof(T);
        this->ndims = data->ndims;
        this->shape = data->shape;
        this->spacing = data->spacing;

        this->array = reinterpret_cast<std::vector<T>*>(data->array);
    };

    ~G4VoxelArray() {};

    T GetValue(unsigned int x) {
        return (*array)[x]; 
    };

    T GetRoundedValue(unsigned int x, T rounder)
    {
        T val = GetValue(x);

        if (val < 0) {
            val = floor((val - rounder/2)/rounder)*rounder;
        } else {
            val = floor((val + rounder/2)/rounder)*rounder;
        }

        return val;
    };
    
    T GetRoundedValue(unsigned int x, T lower, T upper, T rounder)
    {
        T val = GetRoundedValue(x, rounder);
    
        if (val < lower) val = lower;
        if (val > upper) val = upper;

        return val; 
    };

    T GetValue(unsigned int x, unsigned int y) {
        unsigned int index = shape[1] * y + x;
        return array[index];
    };

    G4ThreeVector GetVoxelSize() {
        return G4ThreeVector(spacing[0]/2., spacing[1]/2., spacing[2]/2.);
    };

    G4ThreeVector GetVolumeShape() {
        return G4ThreeVector(shape[0], shape[1], shape[2]);
    };

  public:
    unsigned int length;
    unsigned int ndims;
    std::vector<unsigned int> shape;
    std::vector<double> spacing;
    std::vector<T>* array;
};

#endif // G4VOXELARRAY_H

