#include "NuHepMC/GenRunInfoReaderHelper.hxx"

#include "NuHepMC/StandardEnums.hxx"
#include "NuHepMC/StringUtils.hxx"

#include <algorithm>

namespace NuHepMC {

namespace genruninfo {

void ReadIntStringMap(std::shared_ptr<HepMC3::GenRunInfo> const gri,
                      std::string const &attr_name,
                      std::map<int, std::string> &m) {

  std::shared_ptr<HepMC3::VectorIntAttribute> keys =
      gri->attribute<HepMC3::VectorIntAttribute>(attr_name + ".keys");
  std::shared_ptr<HepMC3::StringAttribute> vals =
      gri->attribute<HepMC3::StringAttribute>(attr_name + ".vals");

  if (keys && vals) {
    std::vector<std::string> valsv =
        NuHepMC::strutil::UnSerializeStringVector(vals->value());
    for (size_t i = 0; i < keys->value().size(); ++i) {
      m[keys->value()[i]] = valsv[i];
    }
  }
}

void ReadStringStringMap(std::shared_ptr<HepMC3::GenRunInfo> const gri,
                         std::string const &attr_name,
                         std::map<std::string, std::string> &m) {

  std::shared_ptr<HepMC3::StringAttribute> keys =
      gri->attribute<HepMC3::StringAttribute>(attr_name + ".keys");
  std::shared_ptr<HepMC3::StringAttribute> vals =
      gri->attribute<HepMC3::StringAttribute>(attr_name + ".vals");

  if (keys && vals) {
    std::vector<std::string> keysv =
        NuHepMC::strutil::UnSerializeStringVector(keys->value());
    std::vector<std::string> valsv =
        NuHepMC::strutil::UnSerializeStringVector(vals->value());
    for (size_t i = 0; i < keysv.size(); ++i) {
      m[keysv[i]] = valsv[i];
    }
  }
}

GRIHelper::GRIHelper(std::shared_ptr<HepMC3::GenRunInfo> const gri)
    : FluxAverageTotalCrossSection(0) {
  if (!gri) {
    std::cout << "[ERROR]: Instantiated GRIHelper with a nullptr" << std::endl;
    throw;
  }

  std::vector<std::string> const standard_names = {
      "VertexEnum", "VertexEnumExtra", "ParticleEnum", "ParticleEnumExtra",
      "HardScatterMode"};

  ReadIntStringMap(gri, "VertexEnum", VertexEnumMap);
  ReadIntStringMap(gri, "VertexEnumExtra", VertexEnumMap);

  ReadIntStringMap(gri, "ParticleEnum", ParticleEnumMap);
  ReadIntStringMap(gri, "ParticleEnumExtra", ParticleEnumMap);

  ReadIntStringMap(gri, "HardScatterMode", ModeDefinitions);

  std::vector<std::string> attr_names = gri->attribute_names();
  for (auto aname : attr_names) {
    if (aname.find(".keys") == (aname.length() - 5)) {
      std::string attr_name = aname.substr(0, aname.length() - 5);
      if (std::find(standard_names.begin(), standard_names.end(), attr_name) ==
          standard_names.end()) { // Not a pre-known key
        std::cout << "Found extra info: " << attr_name << std::endl;

        if (std::find(attr_names.begin(), attr_names.end(),
                      attr_name + ".vals") != attr_names.end()) {
          ReadStringStringMap(gri, attr_name, ExtraDefinitions[attr_name]);
        }
      }
    }
  }

  std::shared_ptr<HepMC3::DoubleAttribute> FATC_att =
      gri->attribute<HepMC3::DoubleAttribute>("FluxAveragedTotalCrossSection");
  if (FATC_att) {
    FluxAverageTotalCrossSection = FATC_att->value();
  }

  ToolInfo = gri->tools();
}

HepMC3::GenRunInfo::ToolInfo GRIHelper::GetGenToolInfo() const {
  return GetToolsInfo().size() ? GetToolsInfo().front()
                               : HepMC3::GenRunInfo::ToolInfo();
}
std::vector<HepMC3::GenRunInfo::ToolInfo> const &
GRIHelper::GetToolsInfo() const {
  return ToolInfo;
}
std::string GRIHelper::GetVertexEnumString(int vs) const {
  if (VertexEnumMap.count(vs)) {
    return VertexEnumMap.at(vs);
  }
  return "UnknownVertexStatus";
}
std::string GRIHelper::GetParticleEnumString(int ps) const {
  if (ParticleEnumMap.count(ps)) {
    return ParticleEnumMap.at(ps);
  }
  return "UnknownParticleStatus";
}
std::string GRIHelper::GetModeDefinitionString(int m) const {
  if (ModeDefinitions.count(m)) {
    return ModeDefinitions.at(m);
  }
  return "UnknownMode";
}
std::string GRIHelper::GetExtraDefinitionString(std::string const &attr_name,
                                                std::string const &key) const {

  // If you can find it, return it.
  if (ExtraDefinitions.count(attr_name) &&
      ExtraDefinitions.at(attr_name).count(key)) {
    return ExtraDefinitions.at(attr_name).at(key);
  }

  return "UnknownGeneratorDefinition";
}

double GRIHelper::GetFluxAverageTotalCrossSection() const {
  return FluxAverageTotalCrossSection;
}

std::string GRIHelper::to_string() const {
  std::stringstream ss("");
  ss << std::string(80, '*') << std::endl;
  ss << "GenRunInfo: " << std::endl;
  ss << "\tGenerator: " << GetGenToolInfo().name << std::endl
     << "\tVersion: " << GetGenToolInfo().version << std::endl;
  if (GetGenToolInfo().description.size()) {
    ss << "\tDescription: " << GetGenToolInfo().description << std::endl;
  }
  ss << "\tFlux averaged total cross section: " << FluxAverageTotalCrossSection
     << std::endl;
  ss << std::string(80, '*') << std::endl;
  if (VertexEnumMap.size()) {
    ss << "Vertex status definitions: " << std::endl;
    for (auto kv : VertexEnumMap) {
      ss << "\t" << kv.first << ": " << kv.second << std::endl;
    }
    ss << std::string(40, '-') << std::endl;
  }
  if (ParticleEnumMap.size()) {
    ss << "Particle status definitions: " << std::endl;
    for (auto kv : ParticleEnumMap) {
      ss << "\t" << kv.first << ": " << kv.second << std::endl;
    }
    ss << std::string(40, '-') << std::endl;
  }
  if (ModeDefinitions.size()) {
    ss << "Hard scattering mode definitions: " << std::endl;
    for (auto kv : ModeDefinitions) {
      ss << "\t" << kv.first << ": " << kv.second << std::endl;
    }
    ss << std::string(40, '-') << std::endl;
  }
  if (ExtraDefinitions.size()) {
    ss << "Generator-specific metadata:" << std::endl;
    for (auto const &ed : ExtraDefinitions) {
      ss << "\t" << ed.first << ": " << std::endl;
      for (auto const &d : ed.second) {
        ss << "\t\t" << d.first << ": " << d.second << std::endl;
      }
      ss << "\t" << std::string(30, '-') << std::endl;
    }
  }
  ss << std::string(80, '*') << std::endl;

  return ss.str();
}

} // namespace genruninfo

} // namespace NuHepMC
