#include <iostream>

#include "HepMCNuEvtTools/WriterTools"

#include "HepMC3/Print.h"
#include "HepMC3/WriterRootTree.h"

using namespace HepMC3Nu;
using namespace HepMC3Nu::genruninfo;

int main(int argc, char const *argv[]) {

  auto gri = GRIFactory("TESTGen", "1.2.3", "Fake description");
  SetParticleEnumStandard(gri, "1.0");
  SetVertexEnumStandard(gri, "1.0");

  SetFluxAveragedTotalCrossSection(gri, 1E-38);

  std::map<int, string> Modestrs = {
      {1, "CCQE"}, {2, "MEC"}, {3, "RES"}, {4, "DIS"}, {5, "COH"}};

  SetHardScatterModeDefinitions(gri, Modestrs);

  AddGRIKeyValuePairs(gri, "GenModelSettings",
                      {{"MaQE", "1"}, {"MaRES", "0.95"}});

  AddGRIKeyValuePairs(gri, "StagesRun",
                      {{"ISI", "0"}, {"HardScatter", "1"}, {"FSI", "1"}});

  WriterRootTree out(argv[1], gri);

  HepMC3::GenEvent evt(HepMC3::Units::MEV, HepMC3::Units::CM);

  out.write_event(evt);

  out.close();
}
