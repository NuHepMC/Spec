#pragma once

#include "NuHepMC/ParticleStackReaderHelper.hxx"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

namespace NuHepMC {
namespace genevent {

inline int GetHardScatterMode(HepMC3::GenEvent const &evt) {
  auto attr = evt.attribute<HepMC3::IntAttribute>("HSM");
  return bool(attr) ? attr->value() : 0;
}

inline int GetTargetNucleusPDG(HepMC3::GenEvent const &evt) {
  HepMC3::ConstGenVertexPtr LFV = GetLabFrameVertex(evt);

  for (auto const &pi : LFV->particles_in()) {
    if (NuHepMC::pid::IsNuclearPID(pi->pid())) {
      return pi->pid();
    }
  }
  return 0;
}

} // namespace genevent
} // namespace NuHepMC
