#include "NuEvtTopologies.hxx"

#include "NuEvtHelper.hxx"

namespace HepMC3Nu {

namespace topo {

HepMC3Nu::labels::ProbeSummary GetProbeSummary(HepMC3::GenEvent const &evt) {

  auto ISProbe = GetProbe(evt);

  switch (ISProbe->pid()) {
  case 12: {
    return {HepMC3Nu::labels::Matter::kMatter,
            HepMC3Nu::labels::ProbeSpecies::kNue};
  }
  case -12: {
    return {HepMC3Nu::labels::Matter::kAntiMatter,
            HepMC3Nu::labels::ProbeSpecies::kNue};
  }
  case 14: {
    return {HepMC3Nu::labels::Matter::kMatter,
            HepMC3Nu::labels::ProbeSpecies::kNumu};
  }
  case -14: {
    return {HepMC3Nu::labels::Matter::kAntiMatter,
            HepMC3Nu::labels::ProbeSpecies::kNumu};
  }
  case 11: {
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

  if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kNumu) {
    expected_pids = (ps.matter == HepMC3Nu::labels::Matter::kMatter)
                        ? std::vector<int>{14, 13}
                        : std::vector<int>{-14, -13};
  } else if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kNue) {
    expected_pids = (ps.matter == HepMC3Nu::labels::Matter::kMatter)
                        ? std::vector<int>{12, 11}
                        : std::vector<int>{-12, -11};
    auto FSPart = GetHMFSParticle(evt, expected_pids);

  } else if (ps.spec == HepMC3Nu::labels::ProbeSpecies::kElec) {
    expected_pids.push_back(11);
  } else {
    return {HepMC3Nu::labels::Current::kOther, ps,
            HepMC3Nu::labels::Topology::kOther};
  }

  auto FSPart = GetHMFSParticle(evt, expected_pids);

  int NPip = GetFSParticles(evt, 211).size();
  int NPim = GetFSParticles(evt, -211).size();
  int NPi0 = GetFSParticles(evt, 111).size();

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

} // namespace topo

} // namespace HepMC3Nu
