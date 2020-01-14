#include <iostream>

#include "HepMCNuEvtTools/ReaderTools"

#include "HepMC3/Print.h"

using namespace HepMC3Nu;
using namespace HepMC3Nu::genruninfo;

int main(int argc, char const *argv[]) {

  ReaderRootTree in(argv[1]);

  std::unique_ptr<GRIHelper> grih = std::make_unique<GRIHelper>(in.run_info());

  size_t it = 0;
  while (!in.failed()) {

    HepMC3::GenEvent evt;

    in.read_event(evt);

    if (in.failed()) {
      break;
    }

    std::cout << "[INFO]: Read Event " << it++ << std::endl;
    HepMC3::Print::listing(evt);

    std::cout << "Enu: " << GetProbe(evt)->momentum().e() << std::endl;
    std::cout << "Elep: " << GetFSProbe(evt)->momentum().e() << std::endl;
    std::cout << "Q2: " << GetQ2(evt) << std::endl;

    if (grih) {
      std::cout
          << "Mode: "
          << evt.attribute<HepMC3::IntAttribute>("HardScatterMode")->value()
          << " = "
          << grih->GetModeDefinitionString(
                 evt.attribute<HepMC3::IntAttribute>("HardScatterMode")
                     ->value())
          << std::endl;
    }
  }

  in.close();
}
