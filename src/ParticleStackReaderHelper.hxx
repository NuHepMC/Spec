#pragma once

#include "HepMCNuEvtTools/Enums.hxx"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

namespace HepMC3Nu {

std::vector<HepMC3::ConstGenParticlePtr> GetParticles(HepMC3::GenEvent const &,
                                                      int pid, labels::ParticleState);

inline std::vector<HepMC3::ConstGenParticlePtr>
GetISParticles(HepMC3::GenEvent const &evt, int pid) {
  return GetParticles(evt, pid, labels::ParticleState::kInitialState);
}

inline std::vector<HepMC3::ConstGenParticlePtr>
GetFSParticles(HepMC3::GenEvent const &evt, int pid) {
  return GetParticles(evt, pid, labels::ParticleState::kFinalState);
}

std::vector<HepMC3::ConstGenParticlePtr>
GetParticles(HepMC3::GenEvent const &, std::vector<int> const &pids,
             labels::ParticleState);

inline std::vector<HepMC3::ConstGenParticlePtr>
GetISParticles(HepMC3::GenEvent const &evt, std::vector<int> const &pids) {
  return GetParticles(evt, pids, labels::ParticleState::kInitialState);
}

inline std::vector<HepMC3::ConstGenParticlePtr>
GetFSParticles(HepMC3::GenEvent const &evt, std::vector<int> const &pids) {
  return GetParticles(evt, pids, labels::ParticleState::kFinalState);
}

template <typename pidspec>
inline HepMC3::ConstGenParticlePtr
GetHMParticle(HepMC3::GenEvent const &evt, pidspec pid, labels::ParticleState st) {

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
  return GetHMParticle(evt, pid, labels::ParticleState::kInitialState);
}

template <typename pidspec>
inline HepMC3::ConstGenParticlePtr GetHMFSParticle(HepMC3::GenEvent const &evt,
                                                   pidspec pid) {
  return GetHMParticle(evt, pid, labels::ParticleState::kFinalState);
}

HepMC3::ConstGenParticlePtr GetProbe(HepMC3::GenEvent const &evt);
HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt, int pid);
HepMC3::ConstGenParticlePtr GetFSProbe(HepMC3::GenEvent const &evt);

template<typename List>
inline bool ListContains(List const &L, typename List::value_type const & v){
  return (std::find(L.begin(), L.end(), v) != L.end());
}

} // namespace HepMC3Nu

inline std::ostream &operator<<(std::ostream &os,
                                HepMC3::FourVector const &fv) {
  return os << "[ " << fv.x() << ", " << fv.y() << ", " << fv.z()
            << ", E: " << fv.e() << " ]";
}
