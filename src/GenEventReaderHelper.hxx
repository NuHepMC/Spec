#pragma once

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

namespace NuHepMC {
namespace genevent {

inline int GetHardScatterMode(HepMC3::GenEvent const &evt) {
  auto attr = evt.attribute<HepMC3::IntAttribute>("HSM");
  return bool(attr) ? attr->value() : 0;
}

} // namespace genevent
} // namespace NuHepMC
