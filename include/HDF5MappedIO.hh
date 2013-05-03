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


#ifndef HDF5MAPPEDIO_H
#define HDF5MAPPEDIO_H

// G4VOXELDATA //
#include "G4VoxelArrayMappedIO.hh"

// HDF5 //
#include "H5Cpp.h"

// GEANT4 //
#include "globals.hh"


template <typename T>
class HDF5MappedIO : public G4VoxelArrayMappedIO<T> {
  public:
        HDF5MappedIO<T>() {
            this->buffer_width = 2;

            this->buffer_size[0] = 2*this->buffer_width + 1;
            this->buffer_size[1] = 2*this->buffer_width + 1;
            this->buffer_size[2] = 2*this->buffer_width + 1;
        };
  
  public:
    void Read(G4String filename, G4String dataset_name) {
        file = H5::H5File(filename.c_str(), H5F_ACC_RDONLY);
        dataset = file.openDataSet(dataset_name.c_str());
        dataspace = dataset.getSpace();

        int ndims_file = dataspace.getSimpleExtentNdims();
        
        hsize_t chunk_shape[ndims_file];

        H5::DSetCreatPropList cparms = dataset.getCreatePlist();
        if (H5D_CHUNKED == cparms.getLayout())  {
            //int chunk_ndims = cparms.getChunk(32, chunk_shape);
            int chunk_ndims = cparms.getChunk(32, buffer_size);
        }
    };

    T GetValue(unsigned int x, unsigned int y, unsigned int z) {
        unsigned int ndims = dataspace.getSimpleExtentNdims();

        hsize_t offset[3];   // hyperslab offset in the file
        offset[0] = (x / this->buffer_width) * this->buffer_width;
        offset[1] = (y / this->buffer_width) * this->buffer_width;
        offset[2] = (z / this->buffer_width) * this->buffer_width;


        dataspace.selectHyperslab(H5S_SELECT_SET, this->buffer_size, offset);

        memspace = H5::DataSpace(3, this->buffer_size);

        hsize_t      offset_out[3];       // hyperslab offset in memory
        hsize_t      count_out[3];        // size of the hyperslab in memory
        offset_out[0] = 0;
        offset_out[1] = 0;
        offset_out[2] = 0;
        memspace.selectHyperslab( H5S_SELECT_SET, this->buffer_size, offset_out );

        T data_out[this->buffer_size[0]][this->buffer_size[1]][this->buffer_size[2]];
        dataset.read(data_out, H5::PredType::NATIVE_INT, memspace, dataspace );
        
        return data_out[x - offset[0]][y - offset[1]][z - offset[2]];
    };

    //virtual void Write(G4String, G4MappedVoxelArray*) {
    //    G4Exception("G4VoxelData::Write", "Writing data not implemented.",
    //            FatalException, "");
    //};

  private:
    H5::H5File file;
    H5::DataSet dataset; 
    H5::DataSpace dataspace;
    H5::DataSpace memspace;

    int buffer_width;
    hsize_t buffer_size[3];
};

#endif // HDF5MAPPEDIO_H

