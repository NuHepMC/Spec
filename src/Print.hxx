#pragma once

#include "NuHepMC/GenEventReaderHelper.hxx"
#include "NuHepMC/GenRunInfoReaderHelper.hxx"
#include "NuHepMC/Kinematics.hxx"
#include "NuHepMC/ParticleStackReaderHelper.hxx"

#include <sstream>
#include <string>

inline std::ostream &operator<<(std::ostream &os,
                                HepMC3::FourVector const &fv) {
  return os << "[ " << fv.x() << ", " << fv.y() << ", " << fv.z()
            << ", E: " << fv.e() << " ]";
}

namespace NuHepMC {
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
  auto probe = NuHepMC::GetProbe(evt);
  ss << "Probe: " << probe->pid() << ", p: " << probe->momentum() << std::endl;
  ss << "Hard Scattering Mode: " << NuHepMC::genevent::GetHardScatterMode(evt)
     << std::endl;
  ss << std::string(40, '-') << std::endl;
  ss << "Vertices: ";
  auto LabFrameVtx = NuHepMC::GetLabFrameVertex(evt);
  if (LabFrameVtx) {
    ss << "Lab frame vertex: " << std::endl;
    ss << listing(*LabFrameVtx);
  }
  auto HardScatterVtx = NuHepMC::GetHardScatterVertex(evt);
  if (HardScatterVtx) {
    ss << "Hard scatter vertex: " << std::endl;
    ss << listing(*HardScatterVtx);
  }
  ss << std::string(80, '*') << std::endl;

  return ss.str();
}
} // namespace Print
} // namespace NuHepMC
