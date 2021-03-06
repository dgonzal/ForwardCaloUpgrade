#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(fcalor)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#



#execute_process(COMMAND "cd" $CMSSW_BASE	 
#		COMMAND "scram" tool tag geant4core INCLUDE 
#		OUTPUT_VARIABLE GEANT4_INC)
#message(STATUS ${GEANT4_INC})


include_directories(/cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/geant4/10.00.p01-cms/include/Geant4)
include_directories(/cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/hepmc/2.06.07-cms/include)
include_directories(/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.17-cms/include )

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available Vis drivers by default
# You can set WITH_GEANT4_VIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" OFF)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
   find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})


#FIND_LIBRARY(XERCES_LIBRARY xerces-c /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/xerces-c/2.8.0/lib)
#message(STATUS ${XERCES_DIR})



#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(fcalor fcalor.cc ${sources} ${headers})
target_link_libraries(fcalor ${Geant4_LIBRARIES})

  

