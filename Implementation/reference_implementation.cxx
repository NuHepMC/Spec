#include "HepMC3/GenRunInfo.h"

#include "HepMC3/Attribute.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"

#include "HepMC3/WriterAscii.h"

std::shared_ptr<HepMC3::GenRunInfo> BuildGenRunInfo() {
  // G.R.1
  std::shared_ptr<HepMC3::GenRunInfo> gri =
      std::make_shared<HepMC3::GenRunInfo>();

  // G.R.2
  gri->add_attribute("NuHepMC.Version.Major",
                     std::make_shared<HepMC3::IntAttribute>(0));
  gri->add_attribute("NuHepMC.Version.Minor",
                     std::make_shared<HepMC3::IntAttribute>(1));
  gri->add_attribute("NuHepMC.Version.Patch",
                     std::make_shared<HepMC3::IntAttribute>(0));

  // G.R.3
  gri->tools().emplace_back(
      HepMC3::GenRunInfo::ToolInfo{"MyGen", "0.0.1", "My Favorite Generator"});

  // G.R.4
  std::map<int, std::pair<std::string, std::string>> processes = {
      {200, {"CCQE", "The Moon CCQE Model -- PRD 1234 (1990)"}}, // G.S.2
      {300, {"MEC", "My Shiny MEC Model -- PRL 1 (1950)"}},      // G.S.2
      {500, {"Single PiPlus Productions", "Plussy"}},
      {600, {"DIS", "Badabing Badaboom"}},
  };

  std::vector<int> process_ids;
  for (auto const &p : processes) {
    process_ids.push_back(p.first);
  }
  gri->add_attribute("NuHepMC.ProcessIDs",
                     std::make_shared<HepMC3::VectorIntAttribute>(process_ids));

  for (auto const &p : processes) {
    process_ids.push_back(p.first);
  }

  for (auto const &p : processes) {
    gri->add_attribute(
        "NuHepMC.Process[" + std::to_string(p.first) + "].Name",
        std::make_shared<HepMC3::StringAttribute>(p.second.first));
    gri->add_attribute(
        "NuHepMC.Process[" + std::to_string(p.first) + "].Description",
        std::make_shared<HepMC3::StringAttribute>(p.second.second));
  }

  // G.S.1
  gri->add_attribute("NuHepMC.Provenance.NEvents",
                     std::make_shared<HepMC3::IntAttribute>(3));
  gri->add_attribute("NuHepMC.Provenance.RNGState",
                     std::make_shared<HepMC3::IntAttribute>(1337));
  gri->add_attribute("NuHepMC.Provenance.Config.processes_enabled",
                     std::make_shared<HepMC3::VectorIntAttribute>(
                         std::vector<int>{200, 300, 500, 600}));

  gri->add_attribute(
      "NuHepMC.Provenance.Config.Options",
      std::make_shared<HepMC3::VectorStringAttribute>(
          std::vector<std::string>{"opt1", "opt5", "somethingelse"}));

  // G.R.5
  gri->set_weight_names({
      "CV",
  });

  // G.C.1
  gri->add_attribute(
      "NuHepMC.Conventions",
      std::make_shared<HepMC3::VectorStringAttribute>(
          std::vector<std::string>{"G.C.1", "G.C.2", "G.C.3", "G.C.4", "E.C.1"}));

  return gri;
}

int main() {

  auto gri = BuildGenRunInfo();
  // E.R.3
  HepMC3::GenEvent evt(gri, HepMC3::Units::MEV, HepMC3::Units::MM);

  auto writer = std::make_shared<HepMC3::WriterAscii>("example.hepmc3");

  // E.C.1
  const double cm2_to_pb = 1E36;
  evt.weight("CV") = 10E-38 * cm2_to_pb; // pb
  // E.R.1
  evt.set_event_number(1);
  // E.R.2
  evt.add_attribute("ProcID", std::make_shared<HepMC3::IntAttribute>(200));
  // E.R.4
  evt.add_attribute("LabPos", std::make_shared<HepMC3::VectorDoubleAttribute>(
                                  std::vector<double>{0, 0, 0}));
  writer->write_event(evt);

  evt.set_event_number(2);
  evt.add_attribute("ProcID", std::make_shared<HepMC3::IntAttribute>(300));
  writer->write_event(evt);

  evt.set_event_number(3);
  evt.add_attribute("ProcID", std::make_shared<HepMC3::IntAttribute>(500));
  writer->write_event(evt);

  writer->close();
}