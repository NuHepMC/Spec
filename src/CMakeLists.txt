SET(implementation_files
  NuEvtHelper.cxx
  NuEvtKinematics.cxx
  NuEvtTopologies.cxx)

SET(header_files
  NuEvtHelper.cxx
  NuEvtKinematics.cxx
  NuEvtTopologies.cxx
  NuEvtTools)

add_library(HepMCNuEvtTools SHARED ${implementation_files})

install(TARGETS HepMCNuEvtTools DESTINATION lib)

install(FILES ${header_files} DESTINATION include/HepMCNuEvtTools)
