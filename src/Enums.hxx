#pragma once

#include "NuHepMC/PidTools.hxx"
#include "NuHepMC/StandardEnums.hxx"

#include <iostream>
#include <utility>

namespace NuHepMC {

// This namespace defines *expressly* non-exhaustive labels that might be
// useful internally.
namespace labels {

enum class Current { kCC, kNC, kOther = 999 };

enum class Matter { kMatter, kAntiMatter, kOther = 999 };

enum class ProbeSpecies {
  kNuMu = NuHepMC::pid::kNuMu,
  kNue = NuHepMC::pid::kNuE,
  kElec = NuHepMC::pid::kElec,
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

} // namespace NuHepMC

inline std::ostream &operator<<(std::ostream &os, NuHepMC::labels::Current c) {
  switch (c) {
  case NuHepMC::labels::Current::kCC:
    return os << "kCC";
  case NuHepMC::labels::Current::kNC:
    return os << "kNC";
  default:
    return os << "UnknownCurrent";
  }
}

inline std::ostream &operator<<(std::ostream &os, NuHepMC::labels::Matter m) {
  switch (m) {
  case NuHepMC::labels::Matter::kMatter:
    return os << "kMatter";
  case NuHepMC::labels::Matter::kAntiMatter:
    return os << "kAntiMatter";
  default:
    return os << "UnknownMatterType";
  }
}

inline std::ostream &operator<<(std::ostream &os,
                                NuHepMC::labels::Topology t) {
  switch (t) {
  case NuHepMC::labels::Topology::k0Pi:
    return os << "k0Pi";
  case NuHepMC::labels::Topology::k1Pip:
    return os << "k1Pip";
  case NuHepMC::labels::Topology::k1Pim:
    return os << "k1Pim";
  case NuHepMC::labels::Topology::k1Pi0:
    return os << "k1Pi0";
  case NuHepMC::labels::Topology::kNPi:
    return os << "kNPi";
  default:
    return os << "UnknownTopology";
  }
}
