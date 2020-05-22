#include "NuHepMC/GenRunInfoWriterHelper.hxx"

#include "NuHepMC/AttributeUtils.hxx"
#include "NuHepMC/StandardEnums.hxx"
#include "NuHepMC/StringUtils.hxx"

#include "HepMC3/Attribute.h"

namespace NuHepMC {

namespace genruninfo {

void AddIntStringMap(std::shared_ptr<HepMC3::GenRunInfo> gri,
                     std::string const &attr_name,
                     std::map<int, std::string> const &defns) {
  std::vector<int> keys;
  std::vector<std::string> values;
  for (auto kv : defns) {
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  gri->add_attribute(attr_name + ".keys", AsAttribute(keys));
  gri->add_attribute(
      attr_name + ".vals",
      AsAttribute(NuHepMC::strutil::SerializeStringVector(values)));
}

std::shared_ptr<HepMC3::GenRunInfo> GRIFactory(std::string const &gen_name,
                                               std::string const &gen_version,
                                               std::string const &gen_descrip) {
  std::shared_ptr<HepMC3::GenRunInfo> gri =
      std::make_shared<HepMC3::GenRunInfo>();
  gri->tools().push_back({gen_name, gen_version, gen_descrip});
  return gri;
}

void SetFluxAveragedTotalCrossSection(std::shared_ptr<HepMC3::GenRunInfo> gri,
                                      double fatc) {
  gri->add_attribute("FluxAveragedTotalCrossSection", AsAttribute(fatc));
}

/// Set the NuHepMC VertexEnumStandard used
/// Use 0.0 to disable
void SetVertexEnumStandard(std::shared_ptr<HepMC3::GenRunInfo> gri,
                           std::string const &vstr) {
  AddIntStringMap(gri, "VertexEnum",
                  NuHepMC::labels::GetVertexDefinitions(vstr));
}
void SetExtraVertexEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo> gri,
                                   std::map<int, std::string> const &defns) {
  AddIntStringMap(gri, "VertexEnumExtra", defns);
}

/// Set the NuHepMC ParticleEnumStandard used
/// Use 0.0 to disable
void SetParticleEnumStandard(std::shared_ptr<HepMC3::GenRunInfo> gri,
                             std::string const &vstr) {
  AddIntStringMap(gri, "ParticleEnum",
                  NuHepMC::labels::GetParticleDefinitions(vstr));
}
void SetExtraParticleEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo> gri,
                                     std::map<int, std::string> const &defns) {
  AddIntStringMap(gri, "ParticleEnumExtra", defns);
}

void SetHardScatterModeDefinitions(std::shared_ptr<HepMC3::GenRunInfo> gri,
                                   std::map<int, std::string> const &defns) {
  AddIntStringMap(gri, "HardScatterMode", defns);
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
  gri->add_attribute(
      attr_name + ".keys",
      AsAttribute(NuHepMC::strutil::SerializeStringVector(keys)));
  gri->add_attribute(
      attr_name + ".vals",
      AsAttribute(NuHepMC::strutil::SerializeStringVector(values)));
}

} // namespace genruninfo

} // namespace NuHepMC
