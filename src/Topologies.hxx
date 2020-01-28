#pragma once

#include "NuHepMC/Enums.hxx"

#include "HepMC3/GenEvent.h"

namespace NuHepMC {

namespace topo {

NuHepMC::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &);
NuHepMC::labels::EventSummary GetEventSummary(HepMC3::GenEvent const &);

/// These methods are fine for high-level access to event properties where
/// optimization is not needed, but are inefficient lookups when you want to
/// query multiple event properties procedurally.

bool IsNu(HepMC3::GenEvent const &);
bool IsNuBar(HepMC3::GenEvent const &);

bool IsNuMu(HepMC3::GenEvent const &);
bool IsNuMuBar(HepMC3::GenEvent const &);

bool IsNuE(HepMC3::GenEvent const &);
bool IsNuEBar(HepMC3::GenEvent const &);

bool IsCC(HepMC3::GenEvent const &);
bool IsNC(HepMC3::GenEvent const &);

} // namespace topo

} // namespace NuHepMC
