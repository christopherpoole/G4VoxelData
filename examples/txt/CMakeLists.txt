####################################################
# GEANT4 DICOM Task Force : G4VoxelData Proposal 
#
# File:      CMakeLists.txt
#
# Author:    Christopher M Poole,
# Email:     mail@christopherpoole.net
#
# Date:      14th March, 2013
####################################################

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(GEANT4_VOXELDATA)

# GEANT4 core
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/../../include)

# User code
file(GLOB headers ${PROJECT_SOURCE_DIR}/../../include)

add_executable(TxtExample TxtExample.cc ${headers})

target_link_libraries(TxtExample ${Geant4_LIBRARIES})

