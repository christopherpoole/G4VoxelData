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


#ifndef NUMPYDATAIO_H
#define NUMPYDATAIO_H

// G4VoxelData //
#include "G4VoxelData.hh"
#include "G4VoxelDataIO.hh"

// STL //
#include <vector>

// CNPY //
#include "cnpy.h"

// GEANT4 //
#include "globals.hh"


class NumpyDataIO : public G4VoxelDataIO {
  public:
    G4VoxelData* Read(G4String filename) {

        logger->message << "Opening " << filename << std::endl;
        cnpy::NpyArray array = cnpy::npy_load(filename.c_str());

        unsigned int ndims = array.shape.size();
        std::vector<unsigned int> shape = array.shape;

        if (ndims == 2) {
            logger->warning << "Adding extra dimension to 2D dataset." << std::endl;
            ndims = 3;
            shape.push_back(1);
        }

        unsigned int size = array.word_size;
        for (unsigned int i=0; i<ndims; i++) size *= shape[i]; 

        std::vector<double> spacing(ndims);
        std::fill(spacing.begin(), spacing.end(), 1);
        std::vector<double> origin(ndims);
        std::fill(origin.begin(), origin.end(), 0);

        std::vector<char>* buffer = new std::vector<char>(array.data, array.data + size);
        
        return new G4VoxelData(buffer, size, ndims, shape, spacing, origin, UNKNOWN, ROW_MAJOR);
    };

    using G4VoxelDataIO::Write;
    template <typename T>
    void Write(G4String filename, G4VoxelData* data) {
        const unsigned int shape[] = {data->shape[0], data->shape[1], data->shape[2]};
        cnpy::npy_save(filename, reinterpret_cast<T*>(&data->array->front()), shape, data->ndims, "w", "F");
    }
};

#endif // NUMPYDATAIO_H

