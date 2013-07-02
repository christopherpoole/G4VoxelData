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


#include "TxtDataIO.hh"
#include "G4VoxelArray.hh"


int main(int, char** argv)
{
    TxtDataIO* reader = new TxtDataIO();

    G4VoxelData* data = reader->Read(argv[1]);
    G4VoxelArray<double>* array = new G4VoxelArray<double>(data);

    G4cout << "Value at (1, 2): " << array->GetValue(1, 2) << G4endl;
    G4cout << "Max: " << array->GetMaxValue() << G4endl;
    G4cout << "Min: " << array->GetMinValue() << G4endl;

    return 0;
}


