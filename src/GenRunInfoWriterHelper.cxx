#include "GenRunInfoWriterHelper.hxx"

#include "HepMC3/Attribute.h"

namespace HepMC3Nu {

namespace genruninfo {

std::shared_ptr<HepMC3::GenRunInfo> GRIFactory(std::string const &gen_name,
                                               std::string const &gen_version,
                                               std::string const &gen_descrip) {
  std::shared_ptr<HepMC3::GenRunInfo> gri =
      std::make_shared<HepMC3::GenRunInfo>();
  gri->tools().push_back({gen_name, gen_version, gen_descrip});
  return gri;
}

/// Set the HepMCNuEvtTools VertexEnumStandard used
/// Use 0.0 to disable
void SetVertexEnumStandard(std::shared_ptr<HepMC3::GenRunInfo> gri, int v = 0,
                           int s = 0) {
  gri->add_attribute("VertexEnumStandard",
                     std::make_shared<HepMC3::StringAttribute>(
                         std::to_string(v) + "." + std::to_string(s)));
}
void AddVertexEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo> gri,
                              std::map<int, std::string> const &defns) {
  std::vector<int> keys;
  std::vector<std::string> values;
  for (auto kv : defns) {
    if(kv.first < 10){
      throw "Not allowed to define new Vertex enums < 10.";
    }
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  gri->add_attribute("VertexEnumExtra_keys",
                     std::make_shared<HepMC3::VectorIntAttribute>(keys));
  gri->add_attribute("VertexEnumExtra_values",
                     std::make_shared<HepMC3::VectorStringAttribute>(values));
}

/// Set the HepMCNuEvtTools ParticleEnumStandard used
/// Use 0.0 to disable
void SetParticleEnumStandard(std::shared_ptr<HepMC3::GenRunInfo> gri, int v = 0,
                             int s = 0) {
  gri->add_attribute("ParticleEnumStandard",
                     std::make_shared<HepMC3::StringAttribute>(
                         std::to_string(v) + "." + std::to_string(s)));
}
void AddParticleEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo> gri,
                                std::map<int, std::string> const &defns) {
  std::vector<int> keys;
  std::vector<std::string> values;
  for (auto kv : defns) {
        if(kv.first < 10){
      throw "Not allowed to define new Particle enums < 10.";
    }
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  gri->add_attribute("ParticleEnumExtra_keys",
                     std::make_shared<HepMC3::VectorIntAttribute>(keys));
  gri->add_attribute("ParticleEnumExtra_values",
                     std::make_shared<HepMC3::VectorStringAttribute>(values));
}

void SetHardScatterModeDefinitions(std::shared_ptr<HepMC3::GenRunInfo> &gri,
                                   std::map<int, std::string> const &defns) {
  std::vector<int> keys;
  std::vector<std::string> values;
  for (auto kv : defns) {
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  gri->add_attribute("HardScatterMode_keys",
                     std::make_shared<HepMC3::VectorIntAttribute>(keys));
  gri->add_attribute("HardScatterMode_values",
                     std::make_shared<HepMC3::VectorStringAttribute>(values));
}

void AddGRIKeyValuePairs(std::shared_ptr<HepMC3::GenRunInfo> gri,
                         std::string const &attr_name,
                         std::map<std::string, std::string> const &defns) {
  std::vector<std::string> keys;
  std::vector<std::string> values;
  for (auto kv : defns) {
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  gri->add_attribute(attr_name + "_keys",
                     std::make_shared<HepMC3::VectorStringAttribute>(keys));
  gri->add_attribute(attr_name + "_values",
                     std::make_shared<HepMC3::VectorStringAttribute>(values));
}

} // namespace genruninfo

} // namespace HepMC3Nu
