SET(implementation_files
  ParticleStackReaderHelper.cxx
  Kinematics.cxx
  Topologies.cxx
  GenRunInfoReaderHelper.cxx
  GenRunInfoWriterHelper.cxx)

SET(header_files
  ParticleStackReaderHelper.hxx
  Kinematics.hxx
  Topologies.hxx
  PidCodes.hxx
  GenRunInfoReaderHelper.hxx
  GenRunInfoWriterHelper.hxx
  ReaderTools
  WriterTools)

add_library(HepMCNuEvtTools SHARED ${implementation_files})

install(TARGETS HepMCNuEvtTools DESTINATION lib)

install(FILES ${header_files} DESTINATION include/HepMCNuEvtTools)
