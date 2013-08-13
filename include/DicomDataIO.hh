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


#ifndef DICOMDATAIO_H
#define DICOMDATAIO_H

// G4VoxelData //
#include "G4VoxelData.hh"
#include "G4VoxelDataIO.hh"

// STL //
#include <vector>
#include <string>

// Grassroots DICOM Library //
#include "gdcmDirectory.h"
#include "gdcmImageReader.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmRescaler.h"

// GEANT4 //
#include "globals.hh"

class DicomDataIO : public G4VoxelDataIO {
  public:
    DicomDataIO() {
        this->sort = true;
        this->modality = "CT";
        this->acquisition_number = -1;

        this->override_slope = false;
        this->slope = 1;
        this->override_intercept = false;
        this->intercept = 0;
    };    
  
    void SetSort(bool sort) {
        this->sort = sort;
    };

    void SetModality(G4String modality) {
        this->modality = modality;
    };
    
    void SetAcquisitionNumber(int number) {
        this->acquisition_number = number;
    };

    void SetSlope(double slope) {
        override_slope = true;
        this->slope = slope;
    };

    bool IsOverrideSlope() {
        return override_slope;
    };

    void ResetOverrideSlope() {
        override_slope = false;
    };

    void SetIntercept(double intercept) {
        override_intercept = true;
        this->intercept = intercept;
    };

    bool IsOverrideIntercept() {
        return override_intercept;
    };

    void ResetOverrideIntercept() {
        override_intercept = false;
    };

  public:
    G4VoxelData* ReadDirectory(G4String directory)
    {
        logger->SetPrefix("DicomDataIO::ReadDirectory:");
        logger->message << "Reading files in " << directory << G4endl;

        gdcm::Directory dir;
        dir.Load((const char*) directory.c_str());
        std::vector<std::string> input_filenames = dir.GetFilenames();

        if (input_filenames.size() == 0) {
            //logger.error << "Specified directory is empty." << G4endl;
        } else {
            //logger.message << "Found " << input_filenames.size() << " files in " << directory << G4endl;
        }

        // Lookup the modality of all images in the directory, we will choose
        // only those matching `modality` as specified by the user.
        gdcm::Scanner scanner;

        gdcm::Tag const modality_tag = gdcm::Tag(0x08, 0x60);
        scanner.AddTag(modality_tag);

        gdcm::Tag const acquisition_tag = gdcm::Tag(0x20, 0x12);
        scanner.AddTag(acquisition_tag);

        scanner.Scan(input_filenames);
        std::vector<std::string> filtered_filenames = 
            scanner.GetAllFilenamesFromTagToValue(modality_tag,
                                                  (const char*) modality.c_str());

        if (filtered_filenames.size() == 0) {
           //logger.error << "No files of modality " << modality << " in " << directory << G4endl; 
        } else {
            //logger.message << "Found " << filtered_filenames.size() << " " << modality << " files." << G4endl;;
        }

        if (acquisition_number > 0) {
            scanner.Scan(filtered_filenames);
            std::string aq_number = gdcm::to_string(acquisition_number);
            filtered_filenames = 
                scanner.GetAllFilenamesFromTagToValue(acquisition_tag,
                        (const char*) aq_number.c_str());

            if (filtered_filenames.size() == 0) {
                //logger.error << "No files of acquisition number " << acquisition_number << " in directory." << G4endl;
            } else {
                 //logger.message << "Found " << filtered_filenames.size() << " files in acquisition " << acquisition_number << G4endl;
            }
        }
        
        // Sort the files along the z-axis for stacking as a 3D array.
        std::vector<std::string> filenames;
        if (sort == true) {
            gdcm::IPPSorter sorter;
            sorter.SetComputeZSpacing(false);
            sorter.Sort(filtered_filenames);
            filenames = sorter.GetFilenames();

            //if (filenames.size() == 0)
                //logger.error << "Files could not be sorted, check acquisition number" << G4endl;
        } else {
            filenames = filtered_filenames;
        }
        // Populate G4VoxelData with stacked slices.
        G4VoxelData* voxel_data = Read(filenames[0].c_str());

        double first_position = voxel_data->origin[2];
        double last_position;

        for (unsigned int i=1; i<filenames.size(); i++) {
            G4VoxelData* vd = Read(filenames[i].c_str());
            voxel_data->array->insert(voxel_data->array->end(),
                                      vd->array->begin(), vd->array->end());
            voxel_data->length += vd->length;
            voxel_data->shape[2] += vd->shape[2];
            
            if (i == filenames.size()-1)
                last_position = vd->origin[2];
        }

        voxel_data->ndims = 3;

        // Coerce the origin to the centre of the dataset.
        voxel_data->origin[0] = voxel_data->origin[0]
                              + voxel_data->shape[0]*voxel_data->spacing[0]/2;
        voxel_data->origin[1] = voxel_data->origin[1]
                              + voxel_data->shape[1]*voxel_data->spacing[1]/2;
        voxel_data->origin[2] = first_position
                              + (last_position - first_position)/2;

        voxel_data->order = ROW_MAJOR;

        return voxel_data;
    };

    G4VoxelData* Read(G4String filename) {
        if (this->verbose)
            G4cout << "Reading: " << filename << G4endl;

        gdcm::ImageReader* reader = new gdcm::ImageReader();
        reader->SetFileName((const char*) filename.c_str());

        try {
            reader->Read();
        } catch (...) {
            G4Exception("DicomData::Read", "cannot read data.", FatalException, "");
        }

        gdcm::Image* image = &reader->GetImage();
        gdcm::DataSet* file = &reader->GetFile().GetDataSet();
        
        unsigned int ndims = (unsigned int) image->GetNumberOfDimensions();
        unsigned int buffer_length = (unsigned int) image->GetBufferLength();

        std::vector<unsigned int> shape(image->GetDimensions(),
                image->GetDimensions() + sizeof(unsigned int)*ndims);
        std::vector<double> spacing(image->GetSpacing(),
                image->GetSpacing() + sizeof(double) + ndims) ;

        // Add slice thickness as z-spacing
        gdcm::Tag slice_thickness_tag = gdcm::Tag(0x0018, 0x0050);
        if (file->FindDataElement(slice_thickness_tag)) {
            std::stringstream strm;
            file->GetDataElement(slice_thickness_tag).GetValue().Print(strm);
            strm >> spacing[2];
        }

        std::vector<double> origin(image->GetOrigin(),
                image->GetOrigin() + sizeof(double) + ndims);

        gdcm::PixelFormat pixel_format = image->GetPixelFormat();

        char* buffer_in = new char[buffer_length];
        char* buffer_out = new char[buffer_length];
        image->GetBuffer(buffer_in);

        if (!override_slope)
            slope = image->GetSlope();

        if (!override_intercept)
            intercept = image->GetIntercept();

        gdcm::Rescaler rescaler = gdcm::Rescaler();
        rescaler.SetIntercept(intercept);
        rescaler.SetSlope(slope);
        rescaler.SetPixelFormat(pixel_format);
        rescaler.SetMinMaxForPixelType(((gdcm::PixelFormat)INT16).GetMin(),
                                       ((gdcm::PixelFormat)INT16).GetMax());
        rescaler.Rescale(buffer_out, buffer_in, buffer_length);        

        std::vector<char>* buffer =
            new std::vector<char>(buffer_out, buffer_out + buffer_length);

        // Clean up buffers
        delete reader;
        delete buffer_in;
        delete buffer_out;

        return new G4VoxelData(buffer, buffer_length/sizeof(int16_t), ndims, shape, spacing, origin, INT16);
    };
  
 public:
    bool sort;
    G4String modality;
    int acquisition_number;

    bool override_slope;
    double slope;
    bool override_intercept;
    double intercept;
};

#endif // DICOMDATAIO_H

