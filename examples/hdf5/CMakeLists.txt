####################################################
# GEANT4 DICOM Task Force : G4VoxelData Proposal 
#
# File:      CMakeLists.txt
#
# Author:    Christopher M Poole,
# Email:     mail@christopherpoole.net
#
# Date:      19th March, 2012
####################################################

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(GEANT4_VOXELDATA)

# GEANT4 core
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../include)

# User code
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/../../include)

add_executable(HDF5Example HDF5Example.cc ${sources} ${headers})

target_link_libraries(HDF5Example ${Geant4_LIBRARIES})

target_link_libraries(HDF5Example hdf5 hdf5_cpp)

