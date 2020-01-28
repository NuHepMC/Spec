#pragma once

#include "HepMC3/GenRunInfo.h"

namespace NuHepMC {

namespace genruninfo {

std::shared_ptr<HepMC3::GenRunInfo>
GRIFactory(std::string const &gen_name, std::string const &gen_version,
           std::string const &gen_descrip = "");

void SetFluxAveragedTotalCrossSection(std::shared_ptr<HepMC3::GenRunInfo>,
                                      double);

/// Set the NuHepMC VertexEnumStandard used
/// Use 0.0 to disable
void SetVertexEnumStandard(std::shared_ptr<HepMC3::GenRunInfo>,
                           std::string const &vstr = "0");
void SetExtraVertexEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                                   std::map<int, std::string> const &defns);

/// Set the NuHepMC ParticleEnumStandard used
/// Use 0.0 to disable
void SetParticleEnumStandard(std::shared_ptr<HepMC3::GenRunInfo>,
                             std::string const &vstr = "0");
void SetExtraParticleEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                                     std::map<int, std::string> const &defns);

void SetHardScatterModeDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                                   std::map<int, std::string> const &defns);

void AddGRIKeyValuePairs(std::shared_ptr<HepMC3::GenRunInfo>,
                         std::string const &attr_name,
                         std::map<std::string, std::string> const &defns);

} // namespace genruninfo

} // namespace NuHepMC
