#include "HepMCNuEvtTools/GenRunInfoReaderHelper.hxx"

#include "HepMCNuEvtTools/StandardEnums.hxx"

namespace HepMC3Nu {

namespace genruninfo {

GRIHelper::GRIHelper(std::shared_ptr<HepMC3::GenRunInfo> const gri) {
  fGRI = gri;

  std::shared_ptr<HepMC3::StringAttribute> VES =
      fGRI->attribute<HepMC3::StringAttribute>("VertexEnumStandard");
  if (VES) {
    VertexEnumMap = HepMC3Nu::labels::GetVertexDefinitions(VES->value());
  }

  std::shared_ptr<HepMC3::VectorIntAttribute> VEE_k =
      fGRI->attribute<HepMC3::VectorIntAttribute>("VertexEnumExtra_keys");
  std::shared_ptr<HepMC3::VectorStringAttribute> VEE_v =
      fGRI->attribute<HepMC3::VectorStringAttribute>("VertexEnumExtra_values");

  if (VEE_k && VEE_v) {
    for (size_t i = 0; i < VEE_k->value().size(); ++i) {
      VertexEnumMap[VEE_k->value()[i]] = VEE_v->value()[i];
    }
  }

  std::shared_ptr<HepMC3::StringAttribute> PES =
      fGRI->attribute<HepMC3::StringAttribute>("ParticleEnumStandard");
  if (PES) {
    ParticleEnumMap = HepMC3Nu::labels::GetParticleDefinitions(PES->value());
  }

  std::shared_ptr<HepMC3::VectorIntAttribute> PEE_k =
      fGRI->attribute<HepMC3::VectorIntAttribute>("ParticleEnumExtra_keys");
  std::shared_ptr<HepMC3::VectorStringAttribute> PEE_v =
      fGRI->attribute<HepMC3::VectorStringAttribute>(
          "ParticleEnumExtra_values");

  if (PEE_k && PEE_v) {
    for (size_t i = 0; i < PEE_k->value().size(); ++i) {
      ParticleEnumMap[PEE_k->value()[i]] = PEE_v->value()[i];
    }
  }

  std::shared_ptr<HepMC3::VectorIntAttribute> HSM_k =
      fGRI->attribute<HepMC3::VectorIntAttribute>("HardScatterMode_keys");
  std::shared_ptr<HepMC3::VectorStringAttribute> HSM_v =
      fGRI->attribute<HepMC3::VectorStringAttribute>("HardScatterMode_values");

  if (HSM_k && HSM_v) {
    for (size_t i = 0; i < HSM_k->value().size(); ++i) {
      ModeDefinitions[HSM_k->value()[i]] = HSM_v->value()[i];
    }
  }
}

HepMC3::GenRunInfo::ToolInfo GRIHelper::GetGenToolInfo() const {
  return GetToolsInfo().front();
}
std::vector<HepMC3::GenRunInfo::ToolInfo> GRIHelper::GetToolsInfo() const {
  return fGRI->tools();
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
  //If we don't have it, try and lazy load it
  if (!ExtraDefinitions.count(attr_name)) {
    std::shared_ptr<HepMC3::VectorStringAttribute> Ex_k =
        fGRI->attribute<HepMC3::VectorStringAttribute>(attr_name + "_keys");
    std::shared_ptr<HepMC3::VectorStringAttribute> Ex_v =
        fGRI->attribute<HepMC3::VectorStringAttribute>(attr_name + "_values");

    if (Ex_k && Ex_v) {
      for (size_t i = 0; i < Ex_k->value().size(); ++i) {
        ExtraDefinitions[attr_name][Ex_k->value()[i]] = Ex_v->value()[i];
      }
    } else {
      return "UnknownGeneratorDefinition";
    }
  }

  //If you can find it, return it.
  if (ExtraDefinitions[attr_name].count(key)) {
    return ExtraDefinitions[attr_name].at(key);
  }

  return "UnknownGeneratorDefinition";
}

} // namespace genruninfo

} // namespace HepMC3Nu
