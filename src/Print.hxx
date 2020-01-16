#pragma once

#include "HepMCNuEvtTools/GenEventReaderHelper.hxx"
#include "HepMCNuEvtTools/GenRunInfoReaderHelper.hxx"
#include "HepMCNuEvtTools/Kinematics.hxx"
#include "HepMCNuEvtTools/ParticleStackReaderHelper.hxx"

#include <sstream>
#include <string>

inline std::ostream &operator<<(std::ostream &os,
                                HepMC3::FourVector const &fv) {
  return os << "[ " << fv.x() << ", " << fv.y() << ", " << fv.z()
            << ", E: " << fv.e() << " ]";
}

namespace HepMC3Nu {
namespace Print {
inline std::string listing(HepMC3::GenVertex const &vtx) {
  std::stringstream ss("");
  ss << "Id: " << vtx.id() << ", status: " << vtx.status() << std::endl;
  ss << "\tIn: " << std::endl;
  for (auto p : vtx.particles_in()) {
    ss << "\t" << p->id() << "\t" << p->status() << "\t" << p->momentum()
       << "\t" << p->pid() << std::endl;
  }
  ss << "\tOut: " << std::endl;
  for (auto p : vtx.particles_out()) {
    ss << "\t" << p->id() << "\t" << p->status() << "\t" << p->momentum()
       << "\t" << p->pid() << std::endl;
  }
  return ss.str();
}
inline std::string listing(HepMC3::GenEvent const &evt) {

  std::stringstream ss("");

  ss << std::string(80, '*') << std::endl;
  ss << "GenEvent: #" << evt.event_number() << std::endl;
  ss << "Position: " << evt.event_pos() << std::endl;
  auto probe = HepMC3Nu::GetProbe(evt);
  ss << "Probe: " << probe->pid() << ", p: " << probe->momentum() << std::endl;
  ss << "Hard Scattering Mode: " << HepMC3Nu::genevent::GetHardScatterMode(evt)
     << std::endl;
  ss << std::string(40, '-') << std::endl;
  ss << "Vertices: ";
  auto LabFrameVtx = HepMC3Nu::GetLabFrameVertex(evt);
  if (LabFrameVtx) {
    ss << "Lab frame vertex: " << std::endl;
    ss << listing(*LabFrameVtx);
  }
  auto HardScatterVtx = HepMC3Nu::GetHardScatterVertex(evt);
  if (HardScatterVtx) {
    ss << "Hard scatter vertex: " << std::endl;
    ss << listing(*HardScatterVtx);
  }
  ss << std::string(80, '*') << std::endl;

  return ss.str();
}
} // namespace Print
} // namespace HepMC3Nu
