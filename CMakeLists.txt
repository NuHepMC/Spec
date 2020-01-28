cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project(NuHepMC)

include(ExternalProject)

set (NuHepMC_VERSION_MAJOR 0)
set (NuHepMC_VERSION_MINOR 1)
set (NuHepMC_VERSION_REVISION 0)

set (NuHepMC_VERSION_STRING "v${NuHepMC_VERSION_MAJOR}r${NuHepMC_VERSION_MINOR}p${NuHepMC_VERSION_REVISION}")

#Set this to TRUE to enable build debugging messages
set(BUILD_DEBUG_MSGS TRUE)
set(VERBOSE TRUE)
set(USE_DK2NU TRUE)
set(USE_FHICL TRUE)

include(${CMAKE_SOURCE_DIR}/cmake/cmessage.cmake)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE DEBUG)
elseif(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

################################################################################
#                            Check Dependencies
################################################################################

SET(EXTRA_CXX_FLAGS)
SET(INCDIRS)
SET(EXTRA_LINK_DIRS)
SET(EXTRA_LIBS)

include(cmake/parseConfigApp.cmake)

##################################  ROOT  ######################################
include(${CMAKE_SOURCE_DIR}/cmake/ROOTSetup.cmake)

include_directories(${CMAKE_SOURCE_DIR})

###############################  HepMC  #####################################
include(${CMAKE_SOURCE_DIR}/cmake/HepMCSetup.cmake)

################################## COMPILER ####################################
include(${CMAKE_SOURCE_DIR}/cmake/c++CompilerSetup.cmake)

add_subdirectory(src)
add_subdirectory(app)

configure_file(cmake/setup.sh.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/setup.sh" DESTINATION
  ${CMAKE_INSTALL_PREFIX})

configure_file(cmake/NuHepMC-config.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/NuHepMC-config" @ONLY)
install(PROGRAMS
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/NuHepMC-config" DESTINATION
  ${CMAKE_INSTALL_PREFIX}/bin)