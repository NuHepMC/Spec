#pragma once

#include "HepMC3/GenRunInfo.h"

#include <memory>

namespace NuHepMC {

namespace genruninfo {

class GRIHelper {

  std::vector<HepMC3::GenRunInfo::ToolInfo> ToolInfo;

  std::map<int, std::string> VertexEnumMap;
  std::map<int, std::string> ParticleEnumMap;
  std::map<int, std::string> ModeDefinitions;

  std::map<std::string, std::map<std::string, std::string> > ExtraDefinitions;

  double FluxAverageTotalCrossSection;

public:
  GRIHelper(std::shared_ptr<HepMC3::GenRunInfo> const);

  HepMC3::GenRunInfo::ToolInfo GetGenToolInfo() const;
  std::vector<HepMC3::GenRunInfo::ToolInfo> const &GetToolsInfo() const;
  std::string GetVertexEnumString(int) const;
  std::string GetParticleEnumString(int) const;
  std::string GetModeDefinitionString(int) const;
  std::string GetExtraDefinitionString(std::string const &,
                                       std::string const &) const;

  double GetFluxAverageTotalCrossSection() const;

  std::string to_string() const;
};

} // namespace genruninfo

} // namespace NuHepMC
