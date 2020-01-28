#include "NuHepMC/Topologies.hxx"

#include "NuHepMC/ParticleStackReaderHelper.hxx"

#include "NuHepMC/PidTools.hxx"

namespace NuHepMC {

namespace topo {

NuHepMC::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &evt) {

  auto ISProbe = GetProbe(evt);

  switch (ISProbe->pid()) {
  case NuHepMC::pid::kNuE: {
    return {NuHepMC::labels::Matter::kMatter,
            NuHepMC::labels::ProbeSpecies::kNue};
  }
  case NuHepMC::pid::kNuEBar: {
    return {NuHepMC::labels::Matter::kAntiMatter,
            NuHepMC::labels::ProbeSpecies::kNue};
  }
  case NuHepMC::pid::kNuMu: {
    return {NuHepMC::labels::Matter::kMatter,
            NuHepMC::labels::ProbeSpecies::kNuMu};
  }
  case NuHepMC::pid::kNuMuBar: {
    return {NuHepMC::labels::Matter::kAntiMatter,
            NuHepMC::labels::ProbeSpecies::kNuMu};
  }
  case NuHepMC::pid::kElec: {
    return {NuHepMC::labels::Matter::kMatter,
            NuHepMC::labels::ProbeSpecies::kElec};
  }
  default: {
    return {NuHepMC::labels::Matter::kOther,
            NuHepMC::labels::ProbeSpecies::kOther};
  }
  }
}

NuHepMC::labels::EventSummary GetEventSummary(HepMC3::GenEvent const &evt) {
  NuHepMC::labels::ProbeSummary ps = GetProbeSummary(evt);

  std::vector<int> expected_pids;

  if (ps.spec == NuHepMC::labels::ProbeSpecies::kNuMu) {
    expected_pids =
        (ps.matter == NuHepMC::labels::Matter::kMatter)
            ? std::vector<int>{NuHepMC::pid::kNuMu, NuHepMC::pid::kMuon}
            : std::vector<int>{NuHepMC::pid::kNuMuBar, NuHepMC::pid::kMuBar};
  } else if (ps.spec == NuHepMC::labels::ProbeSpecies::kNue) {
    expected_pids =
        (ps.matter == NuHepMC::labels::Matter::kMatter)
            ? std::vector<int>{NuHepMC::pid::kNuE, NuHepMC::pid::kElec}
            : std::vector<int>{NuHepMC::pid::kNuEBar, NuHepMC::pid::kPosit};
    auto FSPart = GetHMFSParticle(evt, expected_pids);

  } else if (ps.spec == NuHepMC::labels::ProbeSpecies::kElec) {
    expected_pids.push_back(NuHepMC::pid::kElec);
  } else {
    return {NuHepMC::labels::Current::kOther, ps,
            NuHepMC::labels::Topology::kOther};
  }

  auto FSPart = GetHMFSParticle(evt, expected_pids);

  int NPip = GetFSParticles(evt, NuHepMC::pid::kPip).size();
  int NPim = GetFSParticles(evt, NuHepMC::pid::kPim).size();
  int NPi0 = GetFSParticles(evt, NuHepMC::pid::kPi0).size();

  NuHepMC::labels::Topology topo = NuHepMC::labels::Topology::kOther;
  int NPi = (NPip + NPim + NPi0);
  if (NPi == 0) {
    topo = NuHepMC::labels::Topology::k0Pi;
  } else if (NPi == 1) {
    if (NPip) {
      topo = NuHepMC::labels::Topology::k1Pip;
    } else if (NPim) {
      topo = NuHepMC::labels::Topology::k1Pim;
    } else {
      topo = NuHepMC::labels::Topology::k1Pi0;
    }
  } else {
    topo = NuHepMC::labels::Topology::kNPi;
  }

  return {FSPart->pid() % 2 ? NuHepMC::labels::Current::kCC
                            : NuHepMC::labels::Current::kNC,
          ps, topo};
}

bool IsNu(HepMC3::GenEvent const &evt) {
  return NuHepMC::ListContains(NuHepMC::pid::kMatterNeutrinos,
                                GetProbe(evt)->pid());
}
bool IsNuBar(HepMC3::GenEvent const &evt) {
  return NuHepMC::ListContains(NuHepMC::pid::kAntiMatterNeutrinos,
                                GetProbe(evt)->pid());
}

bool IsNuMu(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == NuHepMC::pid::kNuMu);
}
bool IsNuMuBar(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == NuHepMC::pid::kNuMuBar);
}

bool IsNuE(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == NuHepMC::pid::kNuE);
}
bool IsNuEBar(HepMC3::GenEvent const &evt) {
  return (GetProbe(evt)->pid() == NuHepMC::pid::kNuEBar);
}

bool IsCC(HepMC3::GenEvent const &evt) {
  return (GetEventSummary(evt).curr == NuHepMC::labels::Current::kCC);
}
bool IsNC(HepMC3::GenEvent const &evt) {
  return (GetEventSummary(evt).curr == NuHepMC::labels::Current::kNC);
}

} // namespace topo

} // namespace NuHepMC
