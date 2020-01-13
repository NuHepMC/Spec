#pragma once

#include <vector>

namespace HepMC3Nu {
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

} // namespace pid
} // namespace HepMC3Nu
