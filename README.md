NuHepMC Specification Version 0.9.0
----------------------------------

# Abstract

In this specification we present an additional set of *Requirements*, *Conventions*, and *Suggestions* for simulated interactions stored in [HepMC3](https://doi.org/10.1016/j.cpc.2020.107310) format. These are designed to be used in specific context of neutrino event generators and associated simulation and analysis toolchains. By doing so, we hope to lower the barrier for interfacing with the output of a range of interaction simulations.

# Table of Contents

* [Introduction](#introduction)
* [The NuHepMC Specification](#nuhepmc)
  * [A Note on the Usage of HepMC3 C++ Class Names](#a-note-on-the-usage-of-hepmc3-c++-class-names)
  * [Generator Run Metadata](#generator-run-metadata)
    * [Requirements](#gr1-valid-genruninfo)
    * [Conventions](#gc1-signalling-followed-conventions)
    * [Suggestions](#gs1-run-configuration)
  * [Event Metadata](#event-metadata)
    * [Requirements](#er1-event-number)
    * [Conventions](#ec1-process-ids)
    * [Suggestions](#es1-beam-description-(beam-simulation))
  * [Vertex Metadata](#vertex-metadata)
    * [Requirements](#vr1-vertex-status-codes)
    * [Conventions](#vc1-bound-nucleon-separation-vertex)
  * [Particle Metadata](#particle-metadata)
    * [Requirements](#pr1-particle-status-codes)
    * [Conventions](#pc1-struck-nucleon-status)
* [Contributing and NuHepMC Revisions](#contributing-and-nuhepmc-revisions)
* [Examples](#examples)
* [Contact](#contact)

# Introduction

In the accelerator neutrino HEP subfield, currently, each event generator, experimental simulation, and analysis framework maintains their own proprietary set of interfaces and format converters. By developing an extensible and extendable set of conventions on top of the versatile and mature HepMC3 we aim to cater to analysers and simulation framework developers who want to handle interactions in a tool-agnostic way, while also not putting any limits on the information that individual event generators want to write out, enabling lossless, bidirectional format conversion with existing proprietary formats.

The specification is broken down into *Requirements*, *Conventions*, and *Suggestions* (RCs) for each of the following HepMC3 components.

* G: Generator Run Metadata
* E: Event Metadata
* V: Vertex Information
* P: Particle Information

Where RCs are enumerated like \<Component\>.\<Category\>.\<Index\>, _i.e._ the _sixth_ _Convention_ for _Event_ information should be referred to as [E.C.5](#ec5-lab-time).

Following named conventions is optional. If conventions prove useful and are considered stable, they may become requirements in future versions. Suggestions cover information that is useful to make available to users, but has not always been included in proprietary formats or may not be simple to include in a first implementation. [G.C.1](#gc1-signalling-followed-conventions) provides a convention on signalling that certain Conventions or Suggestions have been followed.  

See [NuHepMC/ReferenceImplementation](https://github.com/NuHepMC/ReferenceImplementation) for reference implementations of an NuHepMC event writer and [NuHepMC/Validator](https://github.com/NuHepMC/Validator) specification validator.

# NuHepMC

## A Note on the Usage of HepMC3 C++ Class Names

Throughout this standard, references are made to various HepMC3 types, _e.g._ `HepMC3::GenRunInfo`, which are equivalent to the namespace-qualified C++ typename in the [HepMC3 reference implementation](https://gitlab.cern.ch/hepmc/HepMC3). These are used purely as a convenient handle for abstract data objects and NuHepMC should not be considered specific to the reference implementation. Anywhere that the RCs are ambiguous outside of this context should be considered a bug in NuHepMC.

## Generator Run Metadata

The generator run metadata describes the overall setup of the event generator, _i.e._, information that is not unique to a specific event. The NuHepMC specifications for this metadata are as follows:

### Requirements

#### G.R.1 Valid GenRunInfo

All NuHepMC vectors must contain a `HepMC3::GenRunInfo` instance.

#### G.R.2 NuHepMC Version

A NuHepMC `HepMC3::GenRunInfo` instance must contain the following attributes that specify the version of NuHepMC that is implemented

* type: `HepMC3::IntAttribute`, name: `"NuHepMC.Version.Major"`
* type: `HepMC3::IntAttribute`, name: `"NuHepMC.Version.Minor"`
* type: `HepMC3::IntAttribute`, name: `"NuHepMC.Version.Patch"`

This document describes *version 0.9.0* of NuHepMC

#### G.R.3 Generator Identification

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::GenRunInfo::ToolInfo` for each 'tool' involved in the production of the vector thus far. The `ToolInfo` instance must contain non-empty name and version fields.

#### G.R.4 Process Metadata

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.ProcessIds"` listing all physics process Ids as integers.

For each valid process Id, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ProcessInfo\[\<Id\>\].Name"` 
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ProcessInfo\[\<Id\>\].Description"` 

where `\<Id\>` enumerates all process Ids present in `"NuHepMC.ProcessIds"`. (See also [E.R.1](#er1-process-id))

#### G.R.5 Vertex Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.VertexStatusIds"` declaring any generator-specific vertex status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each declared vertex status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.VertexStatusInfo\[\<Id\>\].Name"` 
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.VertexStatusInfo\[\<Id\>\].Description"` 

where `\<Id\>` enumerates all status codes present in `"NuHepMC.VertexStatusIds"`. (See also [V.R.1](#vr1-vertex-status-codes))

#### G.R.6 Particle Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.ParticleStatusIds"` declaring any generator-specific status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each valid particle status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ParticleStatusInfo\[\<Id\>\].Name"` 
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ParticleStatusInfo\[\<Id\>\].Description"` 

where `\<Id\>` enumerates all status codes present in `"NuHepMC.ParticleStatusIds"`. (See also [P.R.1](#pr1-particle-status-codes))

#### G.R.7 Event Weights

For weights that will be calculated for every event, HepMC3 provides an interface for storing the weight names only once in the `HepMC3::GenRunInfo` instance. At least one event weight, named `"CV"` must be declared on the `HepMC3::GenRunInfo` instance, and filled for every event. 

This weight may be 1 or constant for every event in a generator run (in the case of an _unweighted_ event vector). This weight must always be included by a user when producing correctly-normalized predictions from a NuHepMC vector and must not be assumed to be always 1. The exact form of this weight, whether it is the only information required to properly normalize a prediction, or if additional information is required is an implementation detail. If [G.C.2](#gc2-vector-exposure-standalone) and [G.C.4](#gc4-flux-averaged-total-cross-section) are adopted, then the combination of the `"CV"` weight, the flux-averaged total cross section, and the total vector exposure can be unambiguously used to convert an event rate into a cross-section prediction.

This requirement may become more prescriptive in future versions, but it may currently present a barrier to adoption if we require a strict form of the cross-section normalizing factor.

#### G.R.8 Non-standard Particle Numbers (PDG MC Codes)

Essentially all event generators in HEP use a standard set of integer codes
to identify particle species. This numbering scheme is maintained by
the Particle Data Group (PDG) and is regularly updated in their Review
of Particle Physics: [Monte Carlo Particle Numbering Scheme](https://pdg.lbl.gov/2023/mcdata/mc_particle_id_contents.html).

We expect that neutrino event generators may need to use codes for non-standard
particle species (_i.e._, those without an existing PDG code) for a variety of
applications. This could include simulating exotic physics processes involving
new particles as well as implementing bookkeeping methods involving
generator-specific quasiparticles.

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.AdditionalParticleNumbers"` declaring any particle codes used
that are not defined in the current PDG numbering scheme. Including any of the
standard codes in this list is permitted but not required. The standard
particle codes must not be reused to mean something different than in the PDG
specification.

For each additional particle number, the `HepMC3::GenRunInfo` instance must also contain an attribute giving a unique name to the represented particle
species:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.AdditionalParticleNumber\[\<PDG\>\].Name"` 

where `\<PDG\>` enumerates all particle numbers present in `"NuHepMC.AdditionalParticleNumbers"`.

### Conventions

#### G.C.1 Signalling Followed Conventions

To signal to a user that an implementation follows a named convention from this specification, a `HepMC3::VectorStringAttribute` should be added to the `HepMC3::GenRunInfo` instance named `"NuHepMC.Conventions"` containing the names of the conventions adhered to.

#### G.C.2 Vector Exposure (Standalone)

Each vector should contain a description of the exposure of the generator run. When running a standalone event simulation this will often correspond to the number of events requested, which may differ from the number of events output in cases where events that are not written out must contribute to the total cross section calculation.

* type: `HepMC3::LongAttribute`, name: `"NuHepMC.Exposure.NEvents"`

Implementations should not adhere to both [G.C.2](#gc2-vector-exposure-(standalone)) and [G.C.3](#gc3-vector-exposure-experimental) simultaneously.

#### G.C.3 Vector Exposure (Experimental)

Each vector should contain a description of the exposure of the generator run. When simulating with some experimental exposure, often "protons on target" (POT), the exposure should be described. Two attributes are reserved for signalling the exposure simulated to users. One or both can be provided.

* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.POT" `
* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.Livetime"`

Implementations should not adhere to both [G.C.2](#gc2-vector-exposure-(standalone)) and [G.C.3](#gc3-vector-exposure-experimental) simultaneously.

#### G.C.4 Cross Section Units and Target Scaling

There are a variety of units typically used to report both measured and
predicted cross sections in HEP. FFor neutrino cross sections specifically, `10^-38 cm^2` per nucleon is common, but not ubiquitous. We want to provide the flexibility to signal different conventions, but provide a sensible recommended default while preserving the flexibility for an implementation to signal a different choice. One or both of the following `HepMC3::StringAttribute`s may be included on the `HepMC3::GenRunInfo` to fully qualify cross section units conventions used within a vector.

* `"NuHepMC.Units.CrossSection.Unit"`. Possible values of the attribute are not restricted, but we reserve the meanings of the following for ease of compatibility with existing conventions:
  * `"pb"`: Picobarns or 10^-36 cm^2. This is our recommended default.
  * `"cm2"`: Using bare cm^2 in this option, without any power-of-ten scaling, is not recommended due to numerical precision concerns. The natural scale of neutrino--nucleon cross sections is approximately 10^-38, which is very close to the minimum representable IEEE 754 single-precision floating point number.
  * `"10-38 cm2"`: This choice is the most frequent in the neutrino literature.
* `"NuHepMC.Units.CrossSection.TargetScale"`. Possible values of the attribute are not restricted, but we reserve the meanings of the following for ease of compatibility with existing conventions:
  * `"PerTargetMolecule"`: Often used for hydrocarbon- and water-target measurements.
  * `"PerTargetAtom"`:  Our recommended default. Choosing "atom" rather than "nucleus" in this context removes ambiguity when considering neutrino interactions with atomic electrons.
  * `"PerTargetNucleon"`: Another common choice in the literature.

It is ultimately up to the user to parse these attributes and decide whether any additional per-event scalings are needed for their purposes. If these attributes are not present, then the cross section will be assumed to be in picobarns per target particle. We _strongly_ recommend that implementations use this default.

#### G.C.5 Flux-averaged Total Cross Section

When running a standalone simulation, with a requested number of simulated events, additional information about the simulated total cross section is required to properly normalize the output event rate to a cross-section prediction. The quantity required is the flux-averaged total cross section, which is calculated from the ratio of the integral of incoming flux times the cross section to the integral of the flux.

This quantity is not always known at the beginning of a generator run. The simulation of events builds up an approximation of the total cross-section as the generator runs, for storing a running 'best estimate' of the total cross-section see [E.C.4](#ec4-estimated-cross-section).

If known, the flux-averaged total cross section should be stored as a `HepMC3::DoubleAttribute` on the `HepMC3::GenRunInfo` named `"NuHepMC.FluxAveragedTotalCrossSection"`.

Optionally, the uncertainty in the flux-averaged total cross section may be stored as a `HepMC3::DoubleAttribute` on the `HepMC3::GenRunInfo` named `"NuHepMC.FluxAveragedTotalCrossSectionUncertainty"`.

See [G.C.4](#gc4-cross-section-units-and-target-scaling) for a convention on the units of this value.

#### G.C.6 Citation Metadata

Modelling components implemented based on published work should always be fully cited. The `HepMC3::GenRunInfo` should contain at least one `HepMC3::VectorStringAttribute` for each relevant modelling component, named according to the pattern `"NuHepMC.Citations.\<Comp\>.\<Type\>"`. 

Valid substitutions for the `\<Comp\>` and `\<Type\>` fields are not restricted by this standard. We suggest using `\<Comp\>=Generator` for specifying the main citation for the interaction generator and `\<Comp\>=Process\[\<Id\>\]` for individual processes. For common reference formats in the HEP field, we suggest some common values for the `\<Type\>` field: 

* `"NuHepMC.Process\[\<Id\>\].InspireHEP"` might contain one or more unique InspireHep identifiers (texkeys).
* `"NuHepMC.Process\[\<Id\>\].arXiv"` might contain one or more unique arXiv identifiers (eprint numbers).
* `"NuHepMC.Process\[\<Id\>\].DOI"` might contain one or more unique Digital Object Identifiers.

If you don't cite your contributing theorists, and make it easy for users of your simulation to cite them as well, then don't come crying to us when they cannot find a job to stay in the field.

We hope that automatic bibliography generation tools using this metadata will be built.

#### G.C.7 Beam Energy Distribution Description

Each vector should contain a description of the beam particle flux used to simulate the output event vector. For many truth studies and experimental simulations where the detector is not physically close to the source, a simple beam energy distribution is enough to describe the particle beam. The two types of energy distribution covered by this convention are mono-energetic beams and those with distributions described by a histogram. The type should be signalled via a `HepMC3::StringAttribute` named `"NuHepMC.Beam\[\<PDG\>\].Type"` with value `"MonoEnergetic"` or `"Histogram"` stored on the `HepMC3::GenRunInfo`. For both types, relevant units can be signalled via two attributes:

* `"NuHepMC.Beam\[\<PDG\>\].EnergyUnit"`. Possible values of the attribute are not restricted, but we reserve the meanings of `"MEV"` and `"GEV"`. This attribute should always exist and be not empty.
* `"NuHepMC.Beam\[\<PDG\>\].RateUnit"`. Possible values of the attribute are not restricted, but we reserve the meaning of `"Arbitrary"` to signal that the normalization of the distribution was not known or used by the simulation. If this attribute is not used then the normalization will be assumed arbitrary.

For the case of a `"MonoEnergetic"`-type distribution, all beam particles in the vector must have identical energy. The attribute `"NuHepMC.Beam\[\<PDG\>\].MonoEnergetic.Energy"` can be used to signal the beam energy in the lab frame, but the usage of this attribute is optional as the energy can be determined from the first (or any) event in the vector.

For the case of a `"Histogram"`-type distribution, the histogram should be encoded into two `HepMC3::VectorDoubleAttribute` per beam species on the `HepMC3::GenRunInfo`:

* `"NuHepMC.Beam\[\<PDG\>\].Histogram.BinEdges"`
* `"NuHepMC.Beam\[\<PDG\>\].Histogram.BinContent"`

where `\[\<PDG\>\]` enumerates the PDG particle numbers of all beam particles present in the event vector. _N.B._ the number of entries in the `"BinEdges"` vector should always be one more than the number of entries in the `"BinContent"` vector.

For a suggestion on how to encode useful information about more realistic neutrino beam descriptions, see [E.S.1](#es1-beam-description-beam-simulation).

### Suggestions

#### G.S.1 Run Configuration

It is recommended that a NuHepMC  instance contains all information required to reproduce the events in the vector. This may be stored in attributes with names beginning with `"NuHepMC.Provenance"`. The information required will necessarily be generator-specific, but we suggest two attributes that would be helpful to downstream users:

* type: `HepMC3::LongAttribute`, name: `"NuHepMC.Provenance.NEvents"`
* type: Implementation defined, name: `"NuHepMC.Provenance.RNGState"`
  * This might be a single seed number used for RNG initialization, or a more complicated description of the initialized RNG state.

#### G.S.2 Non-standard Particle Number Descriptions

For each additional particle number declared in the `"NuHepMC.AdditionalParticleNumbers"` attribute, according to [G.R.8](#gr8-non-standard-particle-numbers-pdg-mc-codes), the `HepMC3::GenRunInfo` instance may contain an attribute giving a description of the particle:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.AdditionalParticleNumber\[\<PDG\>\].Description"` 

#### G.S.3 Complete Status Metadata

While [G.R.5](#gr5-vertex-status-metadata) and [G.R.6](#gr6-particle-status-metadata) explicitly do not require implementations to emit metadata for standard status codes defined in the HepMC3 standard, it is suggested that the complete list of status codes used by an implementation are included in the `"NuHepMC.VertexStatusInfo"` and `"NuHepMC.ParticleStatusInfo"` attributes.

## Event Metadata

The event metadata is used to store information about the event as a whole. The
NuHepMC specifications for this metadata are as follows:

### Requirements

#### E.R.1 Event Number

Each `HepMC3::GenEvent` must have a unique, non-negative event number within a given vector.

#### E.R.2 Process Id

The process Id that produced the `HepMC3::GenEvent` must be recorded in a `HepMC3::IntAttribute` named `"ProcId"`. The metadata for this process Id must be stored according to [G.R.4](#gr4-process-metadata).

#### E.R.3 Units

Energy and position units must be explicitly set in the `HepMC3::GenEvent`.

#### E.R.4 Lab Position

The position of the event in the lab frame must be added as a `HepMC3::VectorDoubleAttribute`, named `"LabPos"`, with the same units as used when implementing [E.R.3](#er3-units). See [E.C.6](#ec6-lab-time) for how to optionally store time in this attribute.

If the simulation did not involve a macroscopic geometry, then this attribute should be set to `\[0, 0, 0\]`.

#### E.R.5 Vertices

An event must have at least one `HepMC3::GenVertex`, and must have one and only one with a _Primary interaction vertex_ status code. No `HepMC3::GenVertex` may have a _Not defined_ status code. (See [V.R.1](#vr1-vertex-status-codes) for additional details).

#### E.R.6 Beam and Target Particles

An event must contain exactly one particle with the _Incoming beam particle_ status code (see [P.R.1](#pr1-particle-status-codes)) and one particle with the _Target particle_ status code. Convention suggests that the more massive particle should usually be considered as the target. This will often be a nucleus or free nucleon particle. See [P.C.1](#pc1-struck-nucleon-status) for an optional convention for marking the kinematics of a constituent bound nucleon if they are definite.

#### E.R.7 Event Completeness

All simulated incoming and outgoing physical particles must be written to the event. The storage of intermediate particles is considered an implementation detail.

### Conventions

#### E.C.1 Process Ids

It is not appropriate to mandate a specific set of interaction processes, and assign them Ids in this standard. Different models make different choices and it is not possible to foresee modelling developments that would require new processes or sub-processes to be defined in the future. Instead, the below ranges of Ids should be used for high-level categorisation of processes. Even if an implementation uses the convention in the table below, it must still adhere to [G.R.4](#gr4-process-metadata).

| Identifier | Process                      |
| ---------- | -------                      |
| 100-199    | Coherent Nuclear scattering  |
| 200-299    | Quasielastic                 |
| 300-399    | Meson Exchange Current       |
| 400-499    | Resonance production         |
| 500-599    | Shallow inelastic scattering |
| 600-699    | Deep inelastic scattering    |
| 700-999    | Other process types          |

Charged current (CC) processes should have identifiers in the X00-X49 block and neutral current (NC) in the X50-X99 block. 

These ranges are subject to change in a future version of this specification.

#### E.C.2 Total Cross Section

The total cross-section for the incoming beam particle, with its specific energy, to interact with the target particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named `"TotXS"`.

#### E.C.3 Process Cross Section

The cross-section for the relevant process Id for the incoming beam particle, with its specific energy, to interact with the target particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named "ProcXS".

#### E.C.4 Estimated Cross Section

Some simulations build up an estimate of the cross section as they run, this makes implementing [G.C.5](#gc5-flux-averaged-total-cross-section) impractical. Instead, the builtin attribute `HepMC3::GenCrossSection`, accessed via `GenEvent::cross_section`, should be used to store the current estimate of the total cross section. A user can then use the best estimate on the last generated event to correctly scale an event rate to a cross-section prediction.

If choosing to use [E.C.4](#ec4-estimated-cross-section), the length of the `cross_sections` and `cross_section_errors` data members must be the same length as the number of weights defined in the header. These should be filled with the current estimate of the total cross section for each variation based on all events through the current event. Additionally, the `HepMC3::GenCrossSection` needs to have the data members `accepted_events` and `attempted_events` filled.

#### E.C.5 Lab Time

If the "LabPos" attribute vector contains three entries then it is considered to be just contain the spatial position, if it contains four entries then the fourth entry is considered the time of the event in seconds.

### Suggestions

#### E.S.1 Beam Description (Beam Simulation)

For more complex beam simulations can not adequately be described by a single energy or energy histogram (see [G.C.6](#gc6-citation-metadata)), it is suggested that the full parent decay history is included in the `HepMC3::GenEvent`. A full set of conventions for the description of beam particle production and parent particle decay chains (for the case of neutrino beams) is currently outside the scope of this specification, but generator implementations can signal that they adhere to this suggestion to notify users that some or all of the beam particle production information is included in the event.

## Vertex Information

The vertices in a HepMC3 event are used to connect groups of incoming and
outgoing particles. For the vertex information, there is only one requirement
in the present version of the NuHepMC standard.

### Requirements

#### V.R.1 Vertex Status Codes

We extend the HepMC3 definition of `HepMC3::GenVertex::status` to include the concept of a primary vertex, corresponding to the _primary_ process (_i.e._ the one labelled by [E.C.1](#ec1-process-ids)), and initial and final state interaction (ISI and FSI) summary vertices. Implementations are free to define specific vertex status codes to refer to individual ISI and FSI processes and output as much information as they require, but a single summary vertex may be useful if the full process history is too detailed or not relevant to users.

| Status Code | Meaning                               | Usage                     |
| ----------- | ------------------------------------- | ------------------------- |
| 0           | Not defined                           | Do not use                |
| 1           | Primary vertex                        | Recommended for all cases |
| 2           | ISI Summary vertex                    | Recommended for all cases |
| 3           | FSI Summary vertex                    | Recommended for all cases |
| 5-10        | Reserved for future NuHepMC standards | Do not use                |
| 11-999      | Generator-dependent                   | For generator usage       |

Any secondary vertex included within a NuHepMC event may have a status in the range 11-999, where [G.R.5](#gr5-vertex-status-metadata) requires that all generator-specific status codes must be fully described by attributes on the `HepMC3::GenRunInfo`. 

### Conventions

#### V.C.1 Bound Nucleon Separation Vertex

When an interaction with a nucleon bound within a nucleus with definite kinematics is simulated, a `HepMC3::GenVertex` corresponding to the separation of the struck nucleon and the nuclear remnant may be included and assigned status code 11. If this convention is signalled via the mechanism described in [G.C.1](gc1-signalling-followed-conventions), then status code 11 need not be included in the implementation of [G.R.5](#gr5-vertex-status-metadata).

## Particle Information

In the current version of the NuHepMC standard, there is only a single
requirement for the particle information.

### Requirements

#### P.R.1 Particle Status Codes

We extend the HepMC3 definition of `HepMC3::GenParticle::status` slightly to include the concept of a target particle, which for neutrino scattering will usually be a target nucleus.

| Status Code | Description                           | Usage                                                   |
| ----------- | ------------------------------------- | ------------------------------------------------------- |
| 0           | Not defined                           | Not meaningful                                          |
| 1           | Undecayed physical particle           | Recommended for all cases                               |
| 2           | Decayed physical particle             | Recommended for all cases                               |
| 3           | Documentation line                    | Often used to indicate in/out particles in hard process |
| 4           | Incoming beam particle                | Recommended for all cases                               |
| 5-10        | Reserved for future HepMC3 standards  | Do not use                                              |
| 11          | Target particle                       | Recommended for all cases                               |
| 12-20       | Reserved for future NuHepMC standards | Do not use                                              |
| 21-200      | Generator-dependent                   | For generator usage                                     |
| 201-        | Simulation-dependent                  | For simulation software usage                           |

Note especially that any incoming real particle must have a status of 4 or 11, and any outgoing real particle must have a status of 1. This allows users to know at a glance which simulated particles must be considered 'observable' and which are 'internal' details of the calculation. Special care must be taken when including the effects of initial-state and final-state interactions.

Any internal particle included within a NuHepMC event may have a status in the range 21-200, where [G.R.6](#gr6-particle-status-metadata) requires that all generator-specific status codes must be fully described by attributes on the `HepMC3::GenRunInfo`. 

### Conventions

#### P.C.1 Struck Nucleon Status

When an interaction with a nucleon, with definite kinematics, bound within a nucleus is simulated, the internal `HepMC3::GenParticle` corresponding to the stuck nucleon should have status code 21. If this convention is signalled via the mechanism described in [G.C.1](gc1-signalling-followed-conventions), then status code 21 need not be included in the implementation of [G.R.6](#gr6-particle-status-metadata).

# Contributing and NuHepMC Revisions

# Examples

## Annotated Event Listings

### Achilles

Version:
NuHepMC is the native output format

### NEUT

Version:
Converted with:

### GENIE

Version:
NuHepMC is the native output format or Converted with:

### NuWro

Version:
Converted with:

## Worked Event Projection Examples

### Calculating Lepton-side Q2

### Total Proton Kinetic Energy

## Normalizing to a Cross Section

### Using G.C.4

# Contact

S. Gardiner: `gardiner[at]fnal[dot]gov`
J. Isaacson: `isaacson[at]fnal[dot]gov`
L. Pickering: `luke.pickering[at]rhul[dot]ac[dot]uk`
