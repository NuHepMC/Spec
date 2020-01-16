#pragma once

#include "HepMCNuEvtTools/PidTools.hxx"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

namespace HepMC3Nu {
namespace genevent {

inline HepMC3::GenParticlePtr
MakeNuclearParticle(size_t Z, size_t A,
                    HepMC3::FourVector p = HepMC3::FourVector::ZERO_VECTOR()) {
  auto nucl = std::make_shared<HepMC3::GenParticle>(
      p, HepMC3Nu::pid::GetNuclearPID(Z, A));
  if (!p.m2()) { // No mass, set NIST isotope mass
    p.set_e(HepMC3Nu::pid::GetIsotopeMass(Z, A));
  } else {
    nucl->set_generated_mass(p.m());
  }

  return nucl;
}

inline void SetHardScatterMode(HepMC3::GenEvent &evt, int mode) {
  evt.add_attribute("HSM", std::make_shared<HepMC3::IntAttribute>(mode));
}

} // namespace genevent
} // namespace HepMC3Nu
