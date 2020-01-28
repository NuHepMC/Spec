#include <iostream>

#include "NuHepMC/ReaderTools"

#include "HepMC3/Print.h"

using namespace NuHepMC;
using namespace NuHepMC::genruninfo;

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

    std::cout << NuHepMC::Print::listing(evt) << std::endl;
  }

  in.close();
}
