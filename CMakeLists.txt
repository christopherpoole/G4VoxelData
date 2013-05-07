cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(G4VoxelData)

include(FindPkgConfig)

# GEANT4
find_package(Geant4 REQUIRED ui_all vis_all)
# find_package(Geant4 REQUIRED)

# GDCM
set(G4VOXELDATA_DICOM_LIBRARIES)
set(G4VOXELDATA_DICOM_USE_FILE)
option(WITH_DICOM "Find GDCM for loading DICOM CT" ON)
if (WITH_DICOM)
    find_package(GDCM REQUIRED)
    set(G4VOXELDATA_DICOM_LIBRARIES ${GDCM_LIBRARIES})
    set(G4VOXELDATA_DICOM_USE_FILE ${GDCM_USE_FILE})
endif()

# Python Numpy
set(G4VOXELDATA_NUMPY_LIBRARIES)
set(G4VOXELDATA_NUMPY_USE_FILE)
option(WITH_NUMPY "Find cnpy for loading Python Numpy arrays" ON)
if (WITH_NUMPY)
    set(G4VOXELDATA_NUMPY_LIBRARIES cnpy)
endif()

include_directories(include/)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(G4VOXELDATA_INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include/G4VoxelData)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/g4voxeldata-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/g4voxeldata-config.cmake" @ONLY IMMEDIATE)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/g4voxeldata-config.cmake" DESTINATION "lib/cmake/G4VoxelData")

install(FILES ${headers} DESTINATION include/G4VoxelData)

