#pragma once

#include "NuEvtEnums.hxx"

#include "HepMC3/GenEvent.h"

namespace HepMC3Nu {

namespace topo {

HepMC3Nu::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &);
HepMC3Nu::labels::EventSummary GetEventSummary(HepMC3::GenEvent const &);

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

} // namespace HepMC3Nu
