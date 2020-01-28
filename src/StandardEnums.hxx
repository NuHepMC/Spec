#pragma once

#include <iostream>
#include <map>
#include <string>

namespace NuHepMC {

// This namespace defines enumerations used in the specification
namespace labels {

enum class ParticleState {
  kInitialState = 0,
  kIntermediate,
  kFinalState,
  kReinteracted,
  kDecayed,
  kOther = 999
};

inline std::map<int, std::string>
GetParticleDefinitions(std::string const &vstring) {
  if (vstring == "1.0") {
    return {
        {0, "InitialState"}, {1, "Intermediate"}, {2, "FinalState"},
        {3, "Reinteracted"}, {4, "Decayed"},
    };
  } else {
    return {};
  }
}

enum class VertexState {
  kLabFrame = 0,
  kISInteraction,
  kHardScatter,
  kFSInteraction,
  kOther = 999
};

inline std::map<int, std::string>
GetVertexDefinitions(std::string const &vstring) {
  if (vstring == "1.0") {
    return {
        {0, "LabFrame"},
        {1, "ISInteraction"},
        {2, "HardScatter"},
        {3, "FSInteraction"},
    };
  } else {
    return {};
  }
}

template <typename E> int e2i(E const &e) { return static_cast<int>(e); }

} // namespace labels

} // namespace NuHepMC

inline std::ostream &operator<<(std::ostream &os,
                                NuHepMC::labels::ParticleState s) {
  switch (s) {
  case NuHepMC::labels::ParticleState::kInitialState:
    return os << "kInitialState";
  case NuHepMC::labels::ParticleState::kIntermediate:
    return os << "kIntermediate";
  case NuHepMC::labels::ParticleState::kFinalState:
    return os << "kFinalState";
  case NuHepMC::labels::ParticleState::kReinteracted:
    return os << "kReinteracted";
  case NuHepMC::labels::ParticleState::kDecayed:
    return os << "kDecayed";
  default:
    return os << "UnknownParticleState";
  }
}

inline std::ostream &operator<<(std::ostream &os,
                                NuHepMC::labels::VertexState s) {
  switch (s) {
  case NuHepMC::labels::VertexState::kLabFrame:
    return os << "kLabFrame";
  case NuHepMC::labels::VertexState::kISInteraction:
    return os << "kISInteraction";
  case NuHepMC::labels::VertexState::kHardScatter:
    return os << "kHardScatter";
  case NuHepMC::labels::VertexState::kFSInteraction:
    return os << "kFSInteraction";
  default:
    return os << "UnknownVertexState";
  }
}
