#include "HepMCNuEvtTools/Topologies.hxx"

#include "HepMCNuEvtTools/ParticleStackReaderHelper.hxx"

#include "HepMCNuEvtTools/PidCodes.hxx"

namespace HepMC3Nu {

namespace topo {

HepMC3Nu::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &evt) {

  auto ISProbe = GetProbe(evt);

  switch (ISProbe->pid()) {
  case HepMC3Nu::pid::kNuE: {
    return {HepMC3Nu::labels::Matter::kMatter,
            HepMC3Nu::labels::ProbeSpecies::kNue};
  }
  case HepMC3Nu::pid::kNuEBar: {
    return {HepMC3Nu::labels::Matter::kAntiMatter,
            HepMC3Nu::labels::ProbeSpecies::kNue};
  }
  case HepMC3Nu::pid::kNuMu: {
    return {HepMC3Nu::labels::Matter::kMatter,
            HepMC3Nu::labels::ProbeSpecies::kNuMu};
  }
  case HepMC3Nu::pid::kNuMuBar: {
    return {HepMC3Nu::labels::Matter::kAntiMatter,
            HepMC3Nu::labels::ProbeSpecies::kNuMu};
  }
  case HepMC3Nu::pid::kElec: {
    return {HepMC3Nu::labels::Matter::kMatter,
            HepMC3Nu::labels::ProbeSpecies::kElec};
  }
  default: {
    return {HepMC3Nu::labels::Matter::kOther,
            HepMC3Nu::labels::ProbeSpecies::kOther};
  }
  }
}

HepMC3Nu::labels::EventSummary GetEventSummary(HepMC3::GenEvent const &evt) {
  HepMC3Nu::labels::ProbeSummary ps = GetProbeSummary(evt);

  std::vector<int> expected_pids;

  if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kNuMu) {
    expected_pids =
        (ps.matter == HepMC3Nu::labels::Matter::kMatter)
            ? std::vector<int>{HepMC3Nu::pid::kNuMu, HepMC3Nu::pid::kMuon}
            : std::vector<int>{HepMC3Nu::pid::kNuMuBar, HepMC3Nu::pid::kMuBar};
  } else if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kNue) {
    expected_pids =
        (ps.matter == HepMC3Nu::labels::Matter::kMatter)
            ? std::vector<int>{HepMC3Nu::pid::kNuE, HepMC3Nu::pid::kElec}
            : std::vector<int>{HepMC3Nu::pid::kNuEBar, HepMC3Nu::pid::kPosit};
    auto FSPart = GetHMFSParticle(evt, expected_pids);

  } else if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kElec) {
    expected_pids.push_back(HepMC3Nu::pid::kElec);
  } else {
    return {HepMC3Nu::labels::Current::kOther, ps,
            HepMC3Nu::labels::Topology::kOther};
  }

  auto FSPart = GetHMFSParticle(evt, expected_pids);

  int NPip = GetFSParticles(evt, HepMC3Nu::pid::kPip).size();
  int NPim = GetFSParticles(evt, HepMC3Nu::pid::kPim).size();
  int NPi0 = GetFSParticles(evt, HepMC3Nu::pid::kPi0).size();

  HepMC3Nu::labels::Topology topo = HepMC3Nu::labels::Topology::kOther;
  int NPi = (NPip + NPim + NPi0);
  if (NPi == 0) {
    topo = HepMC3Nu::labels::Topology::k0Pi;
  } else if (NPi == 1) {
    if (NPip) {
      topo = HepMC3Nu::labels::Topology::k1Pip;
    } else if (NPim) {
      topo = HepMC3Nu::labels::Topology::k1Pim;
    } else {
      topo = HepMC3Nu::labels::Topology::k1Pi0;
    }
  } else {
    topo = HepMC3Nu::labels::Topology::kNPi;
  }

  return {FSPart->pid() % 2 ? HepMC3Nu::labels::Current::kCC
                            : HepMC3Nu::labels::Current::kNC,
          ps, topo};
}

bool IsNu(HepMC3::GenEvent const &evt) {
  return HepMC3Nu::ListContains(HepMC3Nu::pid::kMatterNeutrinos,
                                GetProbe(evt)->pid());
}
bool IsNuBar(HepMC3::GenEvent const &evt) {
  return HepMC3Nu::ListContains(HepMC3Nu::pid::kAntiMatterNeutrinos,
                                GetProbe(evt)->pid());
}

bool IsNuMu(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == HepMC3Nu::pid::kNuMu);
}
bool IsNuMuBar(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == HepMC3Nu::pid::kNuMuBar);
}

bool IsNuE(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == HepMC3Nu::pid::kNuE);
}
bool IsNuEBar(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == HepMC3Nu::pid::kNuEBar);
}

bool IsCC(HepMC3::GenEvent const &evt) {
  return (GetEventSummary(evt).curr == HepMC3Nu::labels::Current::kCC);
}
bool IsNC(HepMC3::GenEvent const &evt) {
  return (GetEventSummary(evt).curr == HepMC3Nu::labels::Current::kNC);
}

} // namespace topo

} // namespace HepMC3Nu
