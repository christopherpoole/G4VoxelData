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


#ifndef TXTDATAIO_H
#define TXTDATAIO_H

// G4VoxelData //
#include "G4VoxelData.hh"
#include "G4VoxelDataIO.hh"

// STL //
#include <vector>
#include <sstream>
#include <fstream>

// GEANT4 //
#include "globals.hh"


class TxtDataIO : public G4VoxelDataIO {
  public:
    G4VoxelData* Read(G4String filename) {
        std::ifstream lines;
        lines.open(filename, std::ios_base::in);
        std::string line;

        unsigned int ndims;
        //unsigned int word_size;
        std::vector<unsigned int> shape;
        std::vector<double> spacing;
        std::vector<double> origin;

        // Read header metadata
        while (std::getline(lines, line)) {
            std::istringstream l(line);

            std::string property;
            l >> property;

            if (property == "ndims") {
                l >> ndims;
            } else if ((property == "shape") && (ndims > 0)) {
                unsigned int s;
                for (unsigned int i=0; i<ndims; i++) {
                    l >> s;
                    shape.push_back(s);
                }
            } else if ((property == "origin") && (ndims > 0)) {
                unsigned int o;
                for (unsigned int i=0; i<ndims; i++) {
                    l >> o;
                    origin.push_back(o);
                }

            } else if (property == "end_header") {
                break;
            }
        }

        unsigned int size = 1;
        for (unsigned int i=0; i<ndims; i++) size *= shape[i];

        // Populate data vector
        double val;
        std::vector<double>* data = new std::vector<double>;
        while (std::getline(lines, line)) {
            std::istringstream l(line);
            while (l >> val) {
                data->push_back(val);
            }
        }
        
        std::vector<char>* buffer = reinterpret_cast<std::vector<char>*>(data);
        return new G4VoxelData(buffer, size, ndims, shape, spacing, origin, UNKNOWN, ROW_MAJOR);
    };
};

#endif // TXTDATAIO_H

