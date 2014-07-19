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

#include "DicomDataIO.hh"

// BOOST/Python //
#include "boost/python.hpp"
#include "pyublas/numpy.hpp"


using namespace boost::python;


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

    class_<DicomDataIO, DicomDataIO*,
        bases<G4VoxelDataIO> >
        ("DicomDataIO", "Read DICOM data into G4VoxelData.")
        .def("SetSort", &DicomDataIO::SetSort)
        .def("SetModality", &DicomDataIO::SetModality)
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

