#pragma once

#include "NuEvtEnums.hxx"

namespace HepMC3Nu {

namespace topo {

HepMC3Nu::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &);
HepMC3Nu::labels::EventSummary GetEventSummary(HepMC3::GenEvent const &);

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
