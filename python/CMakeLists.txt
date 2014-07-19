cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(G4VoxelData)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(PythonLibs REQUIRED)
find_package(Boost REQUIRED COMPONENTS python)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/../include)

find_package(GDCM REQUIRED)
include(${GDCM_USE_FILE})

# User code
file(GLOB headers ${PROJECT_SOURCE_DIR}/../include/*.hh)

SET(LIBRARY_OUTPUT_PATH ".")
add_library(G4VoxelData "SHARED" lib_G4VoxelData.cc ${sources} ${headers})

target_link_libraries(G4VoxelData ${Geant4_LIBRARIES})
target_link_libraries(G4VoxelData ${Boost_LIBRARIES})
target_link_libraries(G4VoxelData ${Python_LIBRARIES})
target_link_libraries(G4VoxelData ${GDCM_LIBRARIES} gdcmMSFF)

