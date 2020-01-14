#include <iostream>

#include "HepMCNuEvtTools/ReaderTools"

#include "HepMC3/Attribute.h"
#include "HepMC3/Print.h"

using namespace HepMC3Nu;
using namespace HepMC3Nu::genruninfo;

int main(int argc, char const *argv[]) {

  ReaderRootTree in(argv[1]);

  std::unique_ptr<GRIHelper> grih = std::make_unique<GRIHelper>(in.run_info());
  HepMC3::GenEvent evt;
  std::cout << grih->to_string() << std::endl;

  in.close();
}
