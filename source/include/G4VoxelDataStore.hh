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


#ifndef G4VOXELDATASTORE_H
#define G4VOXELDATASTORE_H

#include <vector>

// G4VOXELDATA //
#include "G4VoxelData.hh"

template <typename T>
class G4VoxelDataStore : public std::vector<T>
{
  public:
    virtual ~G4VoxelDataStore()
    {
        Clean();   
    };
    
    static G4VoxelDataStore<T>* GetInstance()
    {
        static G4VoxelDataStore<T> voxel_data_store;
        if (!instance)
        {
            instance = &voxel_data_store;
        }
        return instance;
    };
    
    static void Register(T voxel_data)
    {
        GetInstance()->push_back(voxel_data);
    };

    //static void DeRegister(G4VoxelData* voxel_data)
    static void DeRegister(T)
    {
        if (!locked) {
            G4VoxelDataStore* store = GetInstance();
            
            for (typename std::vector<T>::iterator current=store->begin(); current!=store->end(); current++) {
                //if (**current == *voxel_data) {
                //    store->erase(current);
                //    break;
                //}
            }
        }
    };

    static void Clean()
    {
        //locked = true;  

        size_t i=0;
        G4VoxelDataStore* store = GetInstance();

        for(typename std::vector<T>::iterator current=store->begin();
            current!=store->end(); current++) {
            if (*current) {
                delete *current;
            }
            i++;
        }

        //locked = false;
        store->clear();
    };

  protected:
    G4VoxelDataStore()
    {
        std::vector<T>::reserve(10);    
    };

  private:
    static G4VoxelDataStore<T>* instance;
    static bool locked;
};

template <typename T> G4VoxelDataStore<T>* G4VoxelDataStore<T>::instance = 0;
template <typename T> bool G4VoxelDataStore<T>::locked = 0;

#endif // G4VOXELDATASTORE_H

