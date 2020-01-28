#pragma once

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

namespace NuHepMC {

HepMC3::FourVector GetFourMomentumTransfer(HepMC3::GenEvent const& evt);
double GetQ2(HepMC3::GenEvent const& evt);

}
