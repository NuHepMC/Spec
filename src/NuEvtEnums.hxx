#pragma once

#include <utility>
#include <iostream>

namespace HepMC3Nu {

// This namespace defines *expressly* non-exhaustive labels that might be
// useful internally.
namespace labels {

enum class Current { kCC, kNC, kOther };

enum class Matter { kMatter, kAntiMatter, kOther };

enum class ProbeSpecies { kNumu, kNue, kElec, kOther };

enum class Topology { k0Pi, k1Pip, k1Pim, k1Pi0, kNPi, kOther };

enum class State { kIS, kFS, kOther };

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

inline std::ostream &operator<<(std::ostream &os, HepMC3Nu::labels::State s) {
  switch (s) {
  case HepMC3Nu::labels::State::kIS:
    return os << "kIS";
  case HepMC3Nu::labels::State::kFS:
    return os << "kFS";
  default:
    return os << "UnknownState";
  }
}
