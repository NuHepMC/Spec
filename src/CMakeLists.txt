SET(implementation_files
  ParticleStackReaderHelper.cxx
  Kinematics.cxx
  Topologies.cxx
  GenRunInfoReaderHelper.cxx
  GenRunInfoWriterHelper.cxx
  atomic_mass_nist.cxx)

SET(header_files
  atomic_mass_nist.hxx
  ParticleStackReaderHelper.hxx
  Kinematics.hxx
  Topologies.hxx
  PidTools.hxx
  GenRunInfoReaderHelper.hxx
  GenRunInfoWriterHelper.hxx
  Enums.hxx
  StandardEnums.hxx
  ReaderRootTree.hxx
  WriterRootTree.hxx
  GenEventReaderHelper.hxx
  GenEventWriterHelper.hxx
  ReaderTools
  WriterTools)

add_library(HepMCNuEvtTools SHARED ${implementation_files})

install(TARGETS HepMCNuEvtTools DESTINATION lib)

install(FILES ${header_files} DESTINATION include/HepMCNuEvtTools)
