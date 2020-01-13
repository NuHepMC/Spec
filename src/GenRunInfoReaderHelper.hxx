#pragma once

#include "HepMC3/GenRunInfo.h"

namespace HepMC3Nu {

namespace genruninfo {

class GRIHelper {

  std::shared_ptr<HepMC3::GenRunInfo const> fGRI;

  std::pair<int, int> VertexEnumVersion;
  std::map<int, std::string> VertexEnumMap;

  std::pair<int, int> ParticleEnumVersion;
  std::map<int, std::string> ParticleEnumMap;

  std::map<int, std::string> ModeDefinitions;

  // Lazy loading as in general we don't know the names before they are
  // requested
  mutable std::map<std::string, std::map<std::string, std::string> >
      ExtraDefinitions;

public:
  GRIHelper(std::shared_ptr<HepMC3::GenRunInfo> const);

  HepMC3::GenRunInfo::ToolInfo GetGenToolInfo() const;
  std::vector<HepMC3::GenRunInfo::ToolInfo> GetToolsInfo() const;
  std::string GetVertexEnumString(int) const;
  std::string GetParticleEnumString(int) const;
  std::string GetModeDefinitionString(int) const;
  std::string GetExtraDefinitionString(std::string const &,
                                       std::string const &) const;
};

} // namespace genruninfo

} // namespace HepMC3Nu
