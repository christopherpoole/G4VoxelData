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


#ifndef G4VOXELDATALOGGER_H
#define G4VOXELDATALOGGER_H

// G4VOXELDATA //
#include "G4VoxelData.hh"

// STL //
#include <iostream>
#include <ostream>

// GEANT4 //
#include "globals.hh"
#include "G4UIcommand.hh"


enum G4VoxelDataLoggerLevel {
    MESSAGE,
    WARNING,
    ERROR,
    DEBUG
};


class G4VoxelDataLoggerStream : public std::ostream {
  private:
    class Buffer : public std::stringbuf {
      public:
        Buffer() {
        };
        
        ~Buffer() {
            pubsync();
        };

        int sync() {
            std::cout << buffer << str();
            str("");
            return !std::cout;
        };

      private:
        std::string buffer;
    };

  public:
    G4VoxelDataLoggerStream() : std::ostream(new Buffer()){
    };

    ~G4VoxelDataLoggerStream() {
        delete rdbuf();
    };
};


class G4VoxelDataLogger {
  public:
    G4VoxelDataLogger(G4VoxelDataLoggerLevel level) {
        this->level = level;
    };
    
    ~G4VoxelDataLogger() {};

  public:
    void SetVerbose(G4bool verbose) {
        this->verbose = verbose;
    }; 

    G4bool GetVerbose() {
        return this->verbose;
    };

    void SetLevel(G4VoxelDataLoggerLevel level) {
        this->level = level;
    };

    G4VoxelDataLoggerLevel GetLevel() {
        return this->level;
    };

    void SetPrefix(G4String prefix) {
        this->prefix = prefix;
    };
    
    G4String GetPrefix() {
        return this->prefix;
    };

  public:
    G4bool verbose;
    G4String prefix;

    G4VoxelDataLoggerLevel level;

    G4VoxelDataLoggerStream message;
    G4VoxelDataLoggerStream warning;
    G4VoxelDataLoggerStream error;
    G4VoxelDataLoggerStream debug;
};

#endif // G4VOXELDATALOGGER_H

