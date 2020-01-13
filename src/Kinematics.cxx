#include "HepMCNuEvtTools/Kinematics.hxx"
#include "HepMCNuEvtTools/ParticleStackReaderHelper.hxx"

#include <iostream>

namespace HepMC3Nu {

HepMC3::FourVector GetFourMomentumTransfer(HepMC3::GenEvent const &evt) {
  auto ISProbe = GetProbe(evt);
  if (!ISProbe) {
    return HepMC3::FourVector::ZERO_VECTOR();
  }

  auto FSProbe = GetFSProbe(evt, ISProbe->pid());
  if (!FSProbe) {
    return HepMC3::FourVector::ZERO_VECTOR();
  }

  return (ISProbe->momentum() - FSProbe->momentum());
}

double GetQ2(HepMC3::GenEvent const &evt) {
  return -GetFourMomentumTransfer(evt).m2();
}

} // namespace HepMC3Nu
