#pragma once

#include "HepMC3/GenRunInfo.h"

namespace HepMC3Nu {

namespace genruninfo {

std::shared_ptr<HepMC3::GenRunInfo>
GRIFactory(std::string const &gen_name, std::string const &gen_version,
           std::string const &gen_descrip = "");

/// Set the HepMCNuEvtTools VertexEnumStandard used
/// Use 0.0 to disable
void SetVertexEnumStandard(std::shared_ptr<HepMC3::GenRunInfo>, int v = 0,
                           int s = 0);
void AddVertexEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                              std::map<int, std::string> const &defns);

/// Set the HepMCNuEvtTools ParticleEnumStandard used
/// Use 0.0 to disable
void SetParticleEnumStandard(std::shared_ptr<HepMC3::GenRunInfo>, int v = 0,
                             int s = 0);
void AddParticleEnumDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                                std::map<int, std::string> const &defns);

void SetHardScatterModeDefinitions(std::shared_ptr<HepMC3::GenRunInfo>,
                                   std::map<int, std::string> const &defns);

void AddGRIKeyValuePairs(std::shared_ptr<HepMC3::GenRunInfo>,
                         std::string const &attr_name,
                         std::map<std::string, std::string> const &defns);

} // namespace genruninfo

} // namespace HepMC3Nu
