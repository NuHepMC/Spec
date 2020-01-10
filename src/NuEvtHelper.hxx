#pragma once

#include "NuEvtEnums.hxx"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

namespace HepMC3Nu {

std::vector<HepMC3::ConstGenParticlePtr> GetParticles(HepMC3::GenEvent const &,
                                                      int pid, labels::State);

inline std::vector<HepMC3::ConstGenParticlePtr>
GetISParticles(HepMC3::GenEvent const &evt, int pid) {
  return GetParticles(evt, pid, labels::State::kIS);
}

inline std::vector<HepMC3::ConstGenParticlePtr>
GetFSParticles(HepMC3::GenEvent const &evt, int pid) {
  return GetParticles(evt, pid, labels::State::kFS);
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::GenEvent const &, std::vector<int> const &pids,
             labels::State);

inline std::vector<HepMC3::ConstGenParticlePtr>
GetISParticles(HepMC3::GenEvent const &evt, std::vector<int> const &pids) {
  return GetParticles(evt, pids, labels::State::kIS);
}

inline std::vector<HepMC3::ConstGenParticlePtr>
GetFSParticles(HepMC3::GenEvent const &evt, std::vector<int> const &pids) {
  return GetParticles(evt, pids, labels::State::kFS);
}

template <typename pidspec>
inline HepMC3::ConstGenParticlePtr
GetHMParticle(HepMC3::GenEvent const &evt, pidspec pid, labels::State st) {

  HepMC3::ConstGenParticlePtr hm(nullptr);
  double max_p3mod2 = 0;
  for (auto part : GetParticles(evt, pid, st)) {
    double p3mod2 = part->momentum().p3mod2();
    if (p3mod2 > max_p3mod2) {
      hm = part;
      max_p3mod2 = p3mod2;
    }
  }

  return hm;
}

template <typename pidspec>
inline HepMC3::ConstGenParticlePtr GetHMISParticle(HepMC3::GenEvent const &evt,
                                                   pidspec pid) {
  return GetHMParticle(evt, pid, labels::State::kIS);
}

template <typename pidspec>
inline HepMC3::ConstGenParticlePtr GetHMFSParticle(HepMC3::GenEvent const &evt,
                                                   pidspec pid) {
  return GetHMParticle(evt, pid, labels::State::kFS);
}

HepMC3::ConstGenParticlePtr GetProbe(HepMC3::GenEvent const &evt);
HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt, int pid);
HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt);

} // namespace HepMC3Nu
