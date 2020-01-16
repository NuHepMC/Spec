#pragma once

#include "HepMCNuEvtTools/PidTools.hxx"
#include "HepMCNuEvtTools/StandardEnums.hxx"

#include <iostream>
#include <utility>

namespace HepMC3Nu {

// This namespace defines *expressly* non-exhaustive labels that might be
// useful internally.
namespace labels {

enum class Current { kCC, kNC, kOther = 999 };

enum class Matter { kMatter, kAntiMatter, kOther = 999 };

enum class ProbeSpecies {
  kNuMu = HepMC3Nu::pid::kNuMu,
  kNue = HepMC3Nu::pid::kNuE,
  kElec = HepMC3Nu::pid::kElec,
  kOther = 999
};

enum class Topology { k0Pi, k1Pip, k1Pim, k1Pi0, kNPi, kOther = 999 };

struct ProbeSummary {
  Matter matter;
  ProbeSpecies spec;
};

struct EventSummary {
  Current curr;
  ProbeSummary prbsum;
  Topology topo;
};

} // namespace labels

} // namespace HepMC3Nu

inline std::ostream &operator<<(std::ostream &os, HepMC3Nu::labels::Current c) {
  switch (c) {
  case HepMC3Nu::labels::Current::kCC:
    return os << "kCC";
  case HepMC3Nu::labels::Current::kNC:
    return os << "kNC";
  default:
    return os << "UnknownCurrent";
  }
}

inline std::ostream &operator<<(std::ostream &os, HepMC3Nu::labels::Matter m) {
  switch (m) {
  case HepMC3Nu::labels::Matter::kMatter:
    return os << "kMatter";
  case HepMC3Nu::labels::Matter::kAntiMatter:
    return os << "kAntiMatter";
  default:
    return os << "UnknownMatterType";
  }
}

inline std::ostream &operator<<(std::ostream &os,
                                HepMC3Nu::labels::Topology t) {
  switch (t) {
  case HepMC3Nu::labels::Topology::k0Pi:
    return os << "k0Pi";
  case HepMC3Nu::labels::Topology::k1Pip:
    return os << "k1Pip";
  case HepMC3Nu::labels::Topology::k1Pim:
    return os << "k1Pim";
  case HepMC3Nu::labels::Topology::k1Pi0:
    return os << "k1Pi0";
  case HepMC3Nu::labels::Topology::kNPi:
    return os << "kNPi";
  default:
    return os << "UnknownTopology";
  }
}
