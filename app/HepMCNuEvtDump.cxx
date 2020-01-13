#include <iostream>

#include "HepMCNuEvtTools/ReaderTools"

#include "HepMC3/Print.h"
#include "HepMC3/ReaderRootTree.h"

using namespace HepMC3Nu;

int main(int argc, char const *argv[]) {

  HepMC3::ReaderRootTree in(argv[1]);

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
  }

  in.close();
}
