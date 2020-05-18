#pragma once

#include <algorithm>
#include <vector>

#include "NuHepMC/atomic_mass_nist.hxx"

namespace NuHepMC {
namespace pid {

static int const kNuE = 12;
static int const kNuMu = 14;
static int const kNuTau = 16;

static int const kNuEBar = -12;
static int const kNuMuBar = -14;
static int const kNuTauBar = -16;

static int const kElec = 11;
static int const kMuon = 13;

static int const kPosit = -11;
static int const kMuBar = -13;

static int const kProton = 2212;
static int const kNeutron = 2112;

static int const kPip = 211;
static int const kPim = -211;
static int const kPi0 = 111;

static std::vector<int> const kNeutralLeptons{kNuE,    kNuMu,    kNuTau,
                                              kNuEBar, kNuMuBar, kNuTauBar};
static std::vector<int> const kMatterNeutrinos{kNuE, kNuMu, kNuTau};
static std::vector<int> const kAntiMatterNeutrinos{kNuEBar, kNuMuBar,
                                                   kNuTauBar};

static std::vector<int> const kChargedLeptons{kElec, kMuon, kPosit, kMuBar};
static std::vector<int> const kMatterLeptons{kElec, kMuon};
static std::vector<int> const kAntiMatterLeptons{kPosit, kMuBar};

static std::vector<int> const kPions{kPip, kPim, kPi0};
static std::vector<int> const kChargedPions{kPip, kPim};

inline bool IsLepton(int pid) {
  return ((std::find(kNeutralLeptons.begin(), kNeutralLeptons.end(), pid) !=
           kNeutralLeptons.end()) ||
          (std::find(kChargedLeptons.begin(), kChargedLeptons.end(), pid) !=
           kChargedLeptons.end()));
}

inline bool IsNuclearPID(int pid) {
  return (pid > 1000000000) && (pid < 2000000000);
}
inline int GetNuclearPID(int Z, int A) {
  return 1000000000 + A * 10 + Z * 10000;
}
inline int GetIsotopeMass(int Z, int A) {
  return nist::GetIsotopeMassEnergy(Z, A);
}

} // namespace pid
} // namespace NuHepMC
