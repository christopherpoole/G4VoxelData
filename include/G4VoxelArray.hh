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
class G4VoxelArrayBase {
  public:
    G4ThreeVector GetVoxelSize() {
        return G4ThreeVector(spacing[0]/2., spacing[1]/2., spacing[2]/2.);
    };

    G4ThreeVector GetVolumeShape() {
        if (this->cropped) {
            return G4ThreeVector(cropped_shape[0], cropped_shape[1], cropped_shape[2]);
        }
        return G4ThreeVector(shape[0], shape[1], shape[2]);
    };

    unsigned int GetIndex(unsigned int index) {
        return index;
    };

    unsigned int GetIndex(unsigned int x, unsigned int y, unsigned int z) {
        unsigned int index;
        
        if (this->order == ROW_MAJOR) {
            index = x + (this->shape[0] * y) + (this->shape[0] * this->shape[1] * z); 
        } else if (this->order == COLUMN_MAJOR) {
            index = z + (this->shape[2] * y) + (this->shape[2] * this->shape[1] * x); 
        }
        return index;
    };

    void Crop(unsigned int x1, unsigned int x2,
              unsigned int y1, unsigned int y2,
              unsigned int z1, unsigned int z2) {

        this->cropped = true;

        crop_limits.clear();
        crop_limits.push_back(x1);
        crop_limits.push_back(x2);
        crop_limits.push_back(y1);
        crop_limits.push_back(y2);
        crop_limits.push_back(z1);
        crop_limits.push_back(z2);
    
        cropped_shape.push_back(x2 - x1);
        cropped_shape.push_back(y2 - y1);
        cropped_shape.push_back(z2 - z1);
    }

    void Crop(bool cropped) {
        if (cropped && crop_limits.size() != 6) {
            G4cerr << "Cropping limits must be set, Defaulting to array shape." << G4endl;

            ClearCrop();
            cropped_shape = shape;
        }

       this->cropped = cropped; 
    };

    bool IsCropped() {
        return this->cropped;
    }

    void ClearCrop() {
        cropped_shape.clear();
        crop_limits.push_back(0);
        crop_limits.push_back(shape[0]);
        crop_limits.push_back(0);
        crop_limits.push_back(shape[1]);
        crop_limits.push_back(0);
        crop_limits.push_back(shape[2]);
    }
    
    std::vector<unsigned int> GetCropLimit() {
        return this->crop_limits;
    }; 

    unsigned int GetLength() {
        return this->length;
    };

    unsigned int GetDimensions() {
        return this->ndims;
    };

    void SetDimensions(unsigned int ndims) {
        this->ndims = ndims;
    };

    std::vector<unsigned int> GetShape() {
        if (this->cropped) {
            return this->cropped_shape; 
        }
        return this->shape;
    };

    void SetShape(std::vector<unsigned int> shape) {
        this->shape = shape;
    }

    std::vector<double> GetSpacing() {
        return this->spacing;
    };

    void SetSpacing(std::vector<double> spacing) {
        this->spacing = spacing;
    };

  protected:
    unsigned int length;
    unsigned int ndims;
    
    Order order;

    std::vector<unsigned int> shape;
    std::vector<double> spacing;

    bool cropped;
    std::vector<unsigned int> crop_limits;
    std::vector<unsigned int> cropped_shape;
};


template <typename T>
class G4VoxelArray : public G4VoxelArrayBase<T> {
  public:
    G4VoxelArray(G4VoxelData* data) {
        this->length = data->length;
        this->ndims = data->ndims;
        this->shape = data->shape;
        this->spacing = data->spacing;
        this->order = data->order;

        this->cropped = false;

        this->array = reinterpret_cast<std::vector<T>*>(data->array);
    };

    ~G4VoxelArray() {};

    using G4VoxelArrayBase<T>::GetIndex; 
    
    virtual T GetValue(unsigned int x) {
        unsigned int index = GetIndex(x);
        return (*array)[index]; 
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

  public:
    std::vector<T>* array;
};


template <typename T>
class G4VoxelArray<std::complex<T> > : public G4VoxelArrayBase<T> {
  public:
    G4VoxelArray(G4VoxelData* data) {
        this->length = data->length;
        this->ndims = data->ndims;
        this->shape = data->shape;
        this->spacing = data->spacing;
        this->order = data->order;

        this->cropped = false;

        this->array = reinterpret_cast<std::vector<std::complex<T> >*>(data->array);
    };

    ~G4VoxelArray() {};

    using G4VoxelArrayBase<T>::GetIndex; 
    
    T GetValue(unsigned int x) {
        unsigned int index = GetIndex(x);
        return ((*array)[index]).real(); 
    };

    T GetRoundedValue(unsigned int x, std::complex<T> rounder)
    {
        T val = GetValue(x);
        T rounder_ = rounder.real();

        if (val < 0) {
            val = floor((val - rounder_/2)/rounder_)*rounder_;
        } else {
            val = floor((val + rounder_/2)/rounder_)*rounder_;
        }

        return val;
    };
    
    T GetRoundedValue(unsigned int x, std::complex<T> lower, std::complex<T> upper,
            std::complex<T> rounder)
    {
        T val = GetRoundedValue(x, rounder);
    
        if (val < lower.real()) val = lower.real();
        if (val > upper.real()) val = upper.real();

        return val; 
    };

  public:
    std::vector<std::complex<T> >* array;
};


#endif // G4VOXELARRAY_H

