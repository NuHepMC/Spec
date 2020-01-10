#pragma once

#include <utility>

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
