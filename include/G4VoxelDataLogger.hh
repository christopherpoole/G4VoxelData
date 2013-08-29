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
#include <string>
#include <map>

// GEANT4 //
#include "globals.hh"
#include "G4UIcommand.hh"


enum LoggerLevel {
    MESSAGE,
    WARNING,
    ERROR,
    DEBUG
};


class LoggerStream : public std::ostream {
  private:
    class Buffer : public std::stringbuf {
      public:
        Buffer() {
            name = "";
            active = true;
        };

        Buffer(const std::string& buf) : buffer(buf) {
        };

        ~Buffer() {
            pubsync();
        };

        int sync() {
            if (this->active == true) {
                std::cout << name << buffer << str();
            }
            str("");
            return !std::cout;
        };

        void SetName(std::string name) {
            this->name = name + ": ";
        };

        void SetActive(bool active) {
            this->active = active;
        };

      private:
        std::string buffer;
        std::string name;
        bool active;
    };

  public:
    LoggerStream() : std::ostream(new Buffer()){
    };

    ~LoggerStream() {
        delete rdbuf();
    };

    void SetActive(bool active) {
        ((Buffer*) rdbuf())->SetActive(active);
    };

    void SetName(std::string name) {
        ((Buffer*) rdbuf())->SetName(name);
    };
};


class Logger {
  public:
    Logger() {};
    ~Logger() {};

  public:
    void SetVerbose(G4bool verbose) {
        this->verbose = verbose;
    }; 

    G4bool GetVerbose() {
        return this->verbose;
    };

    void SetLevel(LoggerLevel level) {
        this->level = level;
        UpdateLoggerLevels();
    };

    LoggerLevel GetLevel() {
        return this->level;
    };

  protected:
    void AddLoggerStream(std::string name, LoggerLevel level, LoggerStream *stream) {
        stream->SetName(name);
        loggers[level] = stream;
    };  

  private:
    void UpdateLoggerLevels() {
        std::map<LoggerLevel, LoggerStream* >::iterator logger;
        for (logger = loggers.begin(); logger != loggers.end(); ++logger) {
            if (logger->first <= this->level) {
                (logger->second)->SetActive(true);
            } else {
                (logger->second)->SetActive(false);
            }
        }
    };

  public:
    G4bool verbose;
    LoggerLevel level;

    std::vector<std::string> logger_names;
    std::map<LoggerLevel, LoggerStream* > loggers;
};


class G4VoxelDataLogger : public Logger
{
  public:
    G4VoxelDataLogger(LoggerLevel level) {
        AddLoggerStream("MESSAGE", MESSAGE, &message);
        AddLoggerStream("WARNING", WARNING, &warning);
        AddLoggerStream("ERROR", ERROR, &error);
        AddLoggerStream("DEBUG", DEBUG, &debug);

        SetLevel(level);
    };

  public:
    LoggerStream message;
    LoggerStream warning;
    LoggerStream error;
    LoggerStream debug;

};

#endif // G4VOXELDATALOGGER_H

