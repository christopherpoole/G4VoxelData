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
#include "G4VoxelData.hh"


G4VoxelData::G4VoxelData(std::vector<char>* array,
        unsigned int length, unsigned int ndims, unsigned int* shape,
        double* spacing, double* origin, DataType type)
{
    this->array = array;
    this->length = length;
    this->ndims = ndims;
    this->shape = shape;
    this->spacing = spacing;
    this->origin = origin;
    this->type = type;

    // Register the current instance in the store for
    // automatic garbage collection.
    G4VoxelDataStore<G4VoxelData*>::GetInstance()->Register(this);
}

G4VoxelData::~G4VoxelData() {
    delete array;
}

