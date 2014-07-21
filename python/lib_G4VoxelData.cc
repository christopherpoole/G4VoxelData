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

// G4VoxelData //
#include "G4VoxelData.hh"
#include "G4VoxelArray.hh"
#include "G4VoxelDataIO.hh"
#include "G4VoxelDataParameterisation.hh"

#include "DicomDataIO.hh"

// BOOST/Python //
#include "boost/python.hpp"
#include "pyublas/numpy.hpp"


using namespace boost::python;


template <typename T>
void expose(std::string type_name)
{
    std::string voxel_array_base = "G4VoxelArrayBase_" + type_name;
    std::string voxel_array = "G4VoxelArray_" + type_name;
    std::string voxel_data_param = "G4VoxelDataParameterisation_" + type_name;

    class_<G4VoxelArrayBase<T>, G4VoxelArrayBase<T>*>
        (voxel_array_base.c_str(), "The G4VoxelArray base class.")
        .def("GetData", &G4VoxelArrayBase<T>::GetData, return_internal_reference<>())
        .def("GetVoxelSize", &G4VoxelArrayBase<T>::GetVoxelSize)
        .def("GetVolumeShape", &G4VoxelArrayBase<T>::GetVolumeShape)
        .def("GetOrigin", &G4VoxelArrayBase<T>::GetOrigin)
        .def("CropX", &G4VoxelArrayBase<T>::CropX)
        .def("CropY", &G4VoxelArrayBase<T>::CropY)
        .def("CropZ", &G4VoxelArrayBase<T>::CropZ)
        //.def("Crop", &G4VoxelArrayBase<uint16_t>::Crop)
        .def("IsCropped", &G4VoxelArrayBase<T>::IsCropped)
        .def("ClearCop", &G4VoxelArrayBase<T>::ClearCrop)
        .def("GetLength", &G4VoxelArrayBase<T>::GetLength)
        .def("GetDimensions", &G4VoxelArrayBase<T>::GetDimensions)
        ;
      
    class_<G4VoxelArray<T>, G4VoxelArray<T>*,
        bases<G4VoxelArrayBase<T> > >
        (voxel_array.c_str(), "G4VoxelArray composed of G4VoxelData with a type.")
        .def("SetData", &G4VoxelArray<T>::SetData)
        .def("Merge", &G4VoxelArray<T>::Merge)
        .def("max", &G4VoxelArray<T>::GetMaxValue)
        .def("min", &G4VoxelArray<T>::GetMinValue)
        ;

    class_<G4VoxelDataParameterisation<T>, G4VoxelDataParameterisation<T>*>
        (voxel_data_param.c_str(), "G4VoxelDataParameterisation for placing geometry.")
        .def("GetNumberOfMaterials", &G4VoxelDataParameterisation<T>::GetNumberOfMaterials)
        .def("GetLogicalVolume", &G4VoxelDataParameterisation<T>::GetLogicalVolume,
                return_internal_reference<>())
        .def("SetVisibility", &G4VoxelDataParameterisation<T>::SetVisibility)
        .def("ShowPlanes", &G4VoxelDataParameterisation<T>::ShowPlanes)
        .def("ShowMidPlanes", &G4VoxelDataParameterisation<T>::ShowMidPlanes)
        ;
}


BOOST_PYTHON_MODULE(libG4VoxelData)
{
    class_<G4VoxelData, G4VoxelData*>
        ("G4VoxelData", "The root G4VoxelData object.")
        ;

    class_<G4VoxelDataIO, G4VoxelDataIO*>
        ("G4VoxelDataDataIO", "Read DICOM data into G4VoxelData.")
        .def("SetVerbose", &G4VoxelDataIO::SetVerbose)
        .def("GetVerbose", &G4VoxelDataIO::GetVerbose)
        ;

    expose< int16_t>( "int16");
    expose<uint16_t>("uint16");

    class_<DicomDataIO, DicomDataIO*,
        bases<G4VoxelDataIO> >
        ("DicomDataIO", "Read DICOM data into G4VoxelData.")
        .def("SetSort", &DicomDataIO::SetSort)
        .def("SetModality", &DicomDataIO::SetModality)
        .def("SetAcquisitionNumber", &DicomDataIO::SetAcquisitionNumber)
        .def("SetSlope", &DicomDataIO::SetSlope)
        .def("IsOverrideSlope", &DicomDataIO::IsOverrideSlope)
        .def("ResetOverrideSlope", &DicomDataIO::ResetOverrideSlope)
        .def("SetIntercept", &DicomDataIO::SetIntercept)
        .def("IsOverrideIntercept", &DicomDataIO::IsOverrideIntercept)
        .def("ResetOverrideIntercept", &DicomDataIO::ResetOverrideIntercept)
        
        .def("ReadDirectory", &DicomDataIO::_ReadDirectory, return_internal_reference<>())
        .def("Read", &DicomDataIO::_Read, return_internal_reference<>())
        ;
}

