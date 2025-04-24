NuHepMC Specification Version 0.9.0
----------------------------------

# Abstract

In this specification we present an additional set of *Requirements*, *Conventions*, and *Suggestions* for simulated interactions stored in [HepMC3](https://doi.org/10.1016/j.cpc.2020.107310) format. These are designed to be used in specific context of neutrino event generators and associated simulation and analysis toolchains. By doing so, we hope to lower the barrier for interfacing with the output of a range of interaction simulations.

# Table of Contents

* [Introduction](#introduction)
* [The NuHepMC Specification](#nuhepmc)
  * [A Note on the Usage of HepMC3 C++ Class Names](#a-note-on-the-usage-of-hepmc3-c-class-names)
  * [Generator Run Metadata](#generator-run-metadata)
    * [Requirements](#gr1-valid-genruninfo)
    * [Conventions](#gc1-signalling-followed-conventions)
    * [Suggestions](#gs1-run-configuration)
  * [Event Metadata](#event-metadata)
    * [Requirements](#er1-event-number)
    * [Conventions](#ec1-process-ids)
    * [Suggestions](#es1-beam-description-(beam-simulation))
  * [Vertex Information](#vertex-information)
    * [Requirements](#vr1-vertex-status-codes)
    * [Conventions](#vc1-bound-nucleon-separation-vertex)
  * [Particle Information](#particle-information)
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

Where RCs are enumerated like \<Component\>.\<Category\>.\<Index\>, _i.e._ the _fifth_ _Convention_ for _Event_ information should be referred to as [E.C.5](#ec5-lab-time).

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

This document describes *version 1.0.0* of NuHepMC

#### G.R.3 Generator Identification

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::GenRunInfo::ToolInfo` for each 'tool' involved in the production of the vector thus far. The `ToolInfo` instance must contain non-empty name and version fields.

#### G.R.4 Process Metadata

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.ProcessIDs"` listing all physics process Ids as integers.

For each valid process Id, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ProcessInfo[<ID>].Name"`
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ProcessInfo[<ID>].Description"`

where `<ID>` enumerates all process IDs present in `"NuHepMC.ProcessIDs"`. (See also [E.C.1](#ec1-process-id))

#### G.R.5 Vertex Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.VertexStatusIDs"` declaring any generator-specific vertex status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each declared vertex status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.VertexStatusInfo[<ID>].Name"`
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.VertexStatusInfo[<ID>].Description"`

where `<ID>` enumerates all status codes present in `"NuHepMC.VertexStatusIDs"`. (See also [V.R.1](#vr1-vertex-status-codes))

#### G.R.6 Particle Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.ParticleStatusIDs"` declaring any generator-specific status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each valid particle status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ParticleStatusInfo[<ID>].Name"`
* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ParticleStatusInfo[<ID>].Description"`

where `<ID>` enumerates all status codes present in `"NuHepMC.ParticleStatusIDs"`. (See also [P.R.1](#pr1-particle-status-codes))

#### G.R.7 Event Weights

For weights that will be calculated for every event, HepMC3 provides an interface for storing the weight names only once in the `HepMC3::GenRunInfo` instance. At least one event weight, named `"CV"` must be declared on the `HepMC3::GenRunInfo` instance, and filled for every event.

This weight may be 1 or constant for every event in a generator run (in the case of an _unweighted_ event vector). This weight must always be included by a user when producing correctly-normalized predictions from a NuHepMC vector and must not be assumed to be always 1. The exact form of this weight and whether it is the only information required to properly normalize a prediction are considered implementation details.

#### G.R.8 Non-standard Particle Numbers (PDG MC Codes)

Essentially all event generators in `HEP` use a standard set of integer codes
to identify particle species. This numbering scheme is maintained by
the Particle Data Group (PDG) and is regularly updated in their Review
of Particle Physics: [Monte Carlo Particle Numbering Scheme](https://pdg.lbl.gov/2023/mcdata/mc_particle_id_contents.html).

We expect that neutrino event generators may need to use codes for non-standard
particle species (_i.e._, those without an existing PDG code) for a variety of
applications. This could include simulating exotic physics processes involving
new particles as well as implementing bookkeeping methods involving
generator-specific pseudoparticles.

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named `"NuHepMC.AdditionalParticleNumbers"` declaring any particle codes used
that are not defined in the current PDG numbering scheme. Including any of the
standard codes in this list is permitted but not required. The standard
particle codes must not be reused to mean something different than in the PDG
specification.

For each additional particle number, the `HepMC3::GenRunInfo` instance must also contain an attribute giving a unique name to the represented particle
species:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.AdditionalParticleNumber[<PDG>].Name"`

where `<PDG>` enumerates all particle numbers present in `"NuHepMC.AdditionalParticleNumbers"`.

See also [G.C.8](#gc8-non-standard-particle-number-descriptions) for a suggested way of storing descrptions of these special particle species.

### Conventions

#### G.C.1 Signalling Followed Conventions

To signal to a user that an implementation follows a named convention from this specification, a `HepMC3::VectorStringAttribute` should be added to the `HepMC3::GenRunInfo` instance named `"NuHepMC.Conventions"` containing the names of the conventions adhered to.

#### G.C.2 Vector Exposure (Standalone)

Each vector should contain a description of the exposure of the generator run. When running a standalone event simulation this will often correspond to the number of events requested, which may differ from the number of events output in cases where events that are not written out must contribute to the total cross section calculation.

* type: `HepMC3::LongAttribute`, name: `"NuHepMC.Exposure.NEvents"`

Implementations should not adhere to both [G.C.2](#gc2-vector-exposure-(standalone)) and [G.C.3](#gc3-vector-exposure-experimental) simultaneously.

*Note:* It is expected that this number is specific to the generator and can correspond to the number of accepted events, the number of trials, etc.

#### G.C.3 Vector Exposure (Experimental)

Each vector should contain a description of the exposure of the generator run. When simulating with some experimental exposure, often represented for accelerator neutrino experiments in units of "protons on target" (POT),
the exposure should be described. Two attributes are reserved for signaling the exposure used to users. One or both can be provided.

* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.POT" `
* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.Livetime"`

Implementations should not adhere to both [G.C.2](#gc2-vector-exposure-(standalone)) and [G.C.3](#gc3-vector-exposure-experimental) simultaneously.

#### G.C.4 Cross Section Units and Target Scaling

There are a variety of units typically used to report both measured and
predicted cross sections in HEP. For neutrino cross sections specifically, `10^-38 cm^2` per nucleon is common, but not ubiquitous. We want to provide a sensible recommended default while preserving the flexibility for an implementation to signal a different choice. One or both of the following `HepMC3::StringAttribute`s may be included on the `HepMC3::GenRunInfo` to fully qualify cross section units conventions used within a vector.

* `"NuHepMC.Units.CrossSection.Unit"`. Possible values of the attribute are not restricted, but we reserve the meanings of the following:
  * `"pb"`: Picobarns or 10^{-36} cm^2.
  * `"10-38 cm2"`: The choice of 10^{-38} cm^2 is the most frequent in the neutrino literature.
  * `"cm2"`: Using bare cm^2 in this option, without any power-of-ten scaling, is not recommended due to numerical precision concerns. The natural scale of neutrino--nucleon cross sections is approximately 10^{-38}, which is very close to the minimum representable IEEE 754 single-precision floating point number.
* `"NuHepMC.Units.CrossSection.TargetScale"`. Possible values of the attribute are not restricted, but we reserve the meanings of the following for ease of compatibility with existing conventions:
  * `"PerTarget"`:  Our recommendation. This corresponds to the cross section for interacting with the target element, compound, or material in the MCEG run.
  * `"PerTargetNucleon"`: A common choice in the literature.

If this convention is signalled, the chosen units should be assumed to apply to cross section information according to [G.C.5](#gc5-flux-averaged-total-cross-section), [E.C.4](), [E.C.2](#ec2-total-cross-section), and [E.C.3](). _n.b._ The exact interpretation of the `TargetScale` value depends on which cross-section signalling convention is used. For further discussion of the schemes detailed above, see Appendix A of the [NuHepMC paper](https://arxiv.org/abs/2310.13211).

It is ultimately up to the user to parse these attributes and decide whether any additional scaling is needed for their purposes. If these attributes are not present, then the cross section should be assumed to be in picobarns per target. This default value of picobarns is chosen to remain consistent with the assumptions of other tools that read and write HepMC3 files outside the neutrino community, such as [Rivet](https://gitlab.com/hepcedar/rivet/). Rivet is a similar tool to Nuisance, but developed by the collider community. Being consistent between all these tools allows for more interoperability and overall can help to reduce maintenance burdens within the HEP community.

#### G.C.5 Flux-averaged Total Cross Section

The flux-averaged total cross section is a scaling factor that is needed to convert a distribution of simulated events into a prediction of the flux-averaged cross-section --- an experimentally-accessible quantity. Details on the definition of this quantity are given in Sec. 3.1 of the [NuHepMC paper](https://arxiv.org/abs/2310.13211).

The value of the flux-averaged total cross section is not always known at the beginning of a generator run. As described in Appendix B of the [NuHepMC paper](https://arxiv.org/abs/2310.13211), a running estimate of the flux-average total cross section may be computed as events are generated. Element [E.C.4](#ec4-estimated-cross-section) of this specification provides a means of storing the value of this running estimate in each event. If known at the start of a run, the value for the flux-averaged total cross section should be stored as a `HepMC3::DoubleAttribute` on the `HepMC3::GenRunInfo` named `"NuHepMC.FluxAveragedTotalCrossSection"`.
Optionally, the uncertainty in the flux-averaged total cross section may be stored as a `HepMC3::DoubleAttribute` on the `HepMC3::GenRunInfo` named `"NuHepMC.FluxAveragedTotalCrossSectionUncertainty"`.

#### G.C.6 Citation Metadata

Modelling components implemented based on published work should always be fully cited. The `HepMC3::GenRunInfo` should contain at least one `HepMC3::VectorStringAttribute` for each relevant modelling component, named according to the pattern `"NuHepMC.Citations.<Comp>.<Type>"`.

Valid substitutions for the `<Comp>` and `<Type>` fields are not restricted by this standard beyond the requirement that they are pure mixed-case alpha-numeric. We suggest using `<Comp>=Generator` for specifying the main citation for the interaction generator and `<Comp>=Process[<ID>]` for individual processes. For common reference formats in the HEP field, we suggest some common values for the `<Type>` field:

* `"InspireHEP"` might contain one or more unique InspireHep identifiers (texkeys).
* `"arXiv"` might contain one or more unique arXiv identifiers (eprint numbers).
* `"DOI"` might contain one or more unique Digital Object Identifiers.

If you don't cite your contributing theorists, and make it easy for users of your simulation to cite them as well, then don't come crying to us when they cannot find a job to stay in the field.

We hope that automatic bibliography generation tools using this metadata will be built.

#### G.C.7 Beam Energy Distribution Description

*Note:* Flux handling is a complex discussion that is currently not standardized. We provide the following convention suitable for the typical use case by theorists and phenomenologists interested in studying accelerator-based neutrino experiments. A general standardized flux format usable for both accelerator and atmospheric experiments and its corresponding description within the NuHepMC event record is left to future community efforts.

Each vector should contain a description of the beam particle flux used to simulate the output event vector. For many truth studies and experimental simulations where the detector is not physically close to the source, a simple beam energy distribution is enough to describe the particle beam. The two types of energy distribution covered by this convention are mono-energetic beams and those with distributions described by a simple histogram. The type should be signalled via a `HepMC3::StringAttribute` named `"NuHepMC.Beam[<PDG>].Type"` with value `"MonoEnergetic"` or `"Histogram"` stored on the `HepMC3::GenRunInfo`. For both types, relevant units can be signalled via two attributes:

* `"NuHepMC.Beam[<PDG>].EnergyUnit"`. Possible values of the attribute are not restricted, but we reserve the meanings of `"MEV"` and `"GEV"`. This attribute should always exist and be not empty.
* `"NuHepMC.Beam[<PDG>].RateUnit"`. Possible values of the attribute are not restricted, but we reserve the meaning of `"Arbitrary"` to signal that the normalization of the distribution was not known or used by the simulation. If this attribute is not used then the normalization will be assumed arbitrary.

For the case of a `"MonoEnergetic"`-type distribution, all beam particles in the vector must have identical energy. The attribute `"NuHepMC.Beam[<PDG>].MonoEnergetic.Energy"` can be used to signal the beam energy in the lab frame, but the usage of this attribute is optional as the energy can be determined from the first (or any) event in the vector.

For the case of a `"Histogram"`-type distribution, the histogram should be encoded into two `HepMC3::VectorDoubleAttribute` per beam species on the `HepMC3::GenRunInfo`:

* `"NuHepMC.Beam[<PDG>].Histogram.BinEdges"`
* `"NuHepMC.Beam[<PDG>].Histogram.BinContent"`

where `[<PDG>]` enumerates the PDG particle numbers of all beam particles present in the event vector. _N.B._ the number of entries in the `"BinEdges"` vector should always be one more than the number of entries in the `"BinContent"` vector.

The `HepMC3::BoolAttribute`,
* `"NuHepMC.Beam[<PDG>].Histogram.ContentIsPerWidth",

should be used to signal that the number of neutrinos in a given histogram is found by multiplying the bin content by the bin width, rather than from the content alone.
while this might be determined by parsing the `RateUnit` attribute, existing neutrino generators make different assumptions when sampling input neutrino beam energy distributions, so we specify an explicit attribute.
If this attribute is not provided, then it is expected that the number of neutrinos in a given bin is specified by the bin content alone and is independent of the width of the bin.

For a suggestion on how to encode useful information about more realistic neutrino beam descriptions, see [E.S.1](#es1-beam-description-beam-simulation).

#### G.C.8 Non-standard Particle Number Descriptions
For each additional particle number `<PDG>` declared in the `"NuHepMC.AdditionalParticleNumbers"` attribute, according to [G.R.8](#gr8-non-standard-particle-numbers-pdg-mc-codes), the `HepMC3::GenRunInfo` instance may contain an attribute giving a description of the particle:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.AdditionalParticleInfo[<PDG>].Description"`

For non-standard particles that should be further simulated by particle propagation simulations, such as [GEANT4](https://gitlab.cern.ch/geant4/geant4), additional information encoded here may be enough to enable automatic propagation.
In this version of NuHepMC, we do not attempt to prescribe a format for such information but highlight that `HepMC3::GenRunInfo` attributes of the form, `"NuHepMC.AdditionalParticleNumber[<PDG>].<SimName>.<AttrName>"`, might be useful for communicating such additional information. These additional attributes should include, at a minimum, the particle's mass, width, spin, and electric charge.

### Suggestions

#### G.S.1 Run Configuration

It is suggested that a NuHepMC `HepMC3::GenRunInfo` instance should contain all information required to reproduce the events in the vector. This may be stored in attributes with names beginning with `"NuHepMC.Provenance"`. The information required will necessarily be generator-specific, but we suggest two attributes that would be helpful to downstream users:

* type: `HepMC3::LongAttribute`, name: `"NuHepMC.Provenance.NEvents"`
* type: Implementation defined, name: `"NuHepMC.Provenance.RNGState"`
  * This might be a single seed number used as the seed to initialize the random number generator (RNG). It could also be a more complicated description of the RNG state.

#### G.S.3 Complete Status Metadata

While [G.R.5](#gr5-vertex-status-metadata) and [G.R.6](#gr6-particle-status-metadata) explicitly do not require implementations to emit metadata for standard status codes defined in the HepMC3 standard, it is suggested that the complete list of status codes used by an implementation are included in the `"NuHepMC.VertexStatusInfo"` and `"NuHepMC.ParticleStatusInfo"` attributes.

## Event Metadata

The event is used to store information about the event as a whole. An event is described by arbitrary metadata and a graph of particles (edges) and verices (nodes), each with their own arbitrary metadata.
Since generated events are meant to emulate the experimental measurements in order to ensure as similar analysis pipeline between the two as possible, all momenta for each event should be given in the lab frame.
However, it is important to note that while the momentum is in the lab frame, enough information in each event should be available to perform arbitrary transformations to other frames for calculations of observables.
The NuHepMC specifications for events are as follows:

### Requirements

#### E.R.1 HepMC3 Compatibility:

The HepMC3 standard places some constraints on valid event graphs, these constraints must be respected by valid NuHepMC events as we require full compatibility with HepMC3. More details of these constraints can be found in the [HepMC3 specifications](https://arxiv.org/abs/1912.08005).

Exisiting neutrino event generators often rely on effective descriptions of the nuclear environment in a neutrino-nucleus hard scattering process. This means that four-momentum is often not explicitly conserved for the neutrino-nucleus system. Energy and momenta can be exchanged with a _nuclear remnant_, which is not directly involved in a neutrino-nucleon hard scatter, through initial and final state interactions. Implementations are free to conserve four momentum and emit all physical initial and final state particles, including the fully-simulated nuclear remnant, but for those implementations where such a requirement is not feasible or would delay the adoption of this standard, [P.C.2](#pc1-nuclear-remnant-particle-code) reserves a non-standard particle number that can be used to represent a nuclear remnant this is not precisely simulated.

#### E.R.2 Event Number

Each `HepMC3::GenEvent` must have a non-negative event number that is unique within a given vector.

#### E.R.3 Process ID

The process ID for the primary physics process that is represented in the `HepMC3::GenEvent` must be recorded in a `HepMC3::IntAttribute` named `"signal_process_id"`. The metadata for this process ID must be stored according to [G.R.4](#gr4-process-metadata).

#### E.R.4 Units

Energy and position units must be explicitly set in the `HepMC3::GenEvent`.

#### E.R.5 Lab Position

The position of the event in the lab frame must be added as a `HepMC3::VectorDoubleAttribute`, named `"LabPos"`, with the same units as used when implementing [E.R.4](#er4-units). See [E.C.5](#ec5-lab-time) for how to optionally store time in this attribute.
If the simulation did not involve a macroscopic geometry, then this attribute may be set to `[0, 0, 0]`.

#### E.R.6 Vertices

An event must have at least one `HepMC3::GenVertex`, and must have one and only one with a _primary interaction vertex_ status code. No `HepMC3::GenVertex` may have a _not defined_ status code. (See [V.R.1](#vr1-vertex-status-codes) for additional details).

#### E.R.7 Beam and Target Particles

An event must contain exactly one particle with the _incoming beam particle_ status code and one particle with the _target particle_ status code (see [P.R.1](#pr1-particle-status-codes)). We recommend that, in cases where the colliding initial-state particles are distinct, the more massive of the two should be considered the target. For neutrino scattering, the target will thus often be a complex nucleus or a free nucleon. In the case of equally massive particles, the choice to label one of them as the target is arbitrary.

[P.C.1](#pc1-struck-nucleon-status) provides a convention for marking a constituent bound nucleon struck by the incoming beam particle in the event graph.


#### E.R.8 Event Completeness

All simulated incoming and outgoing physical particles must be written to the event. The storage of intermediate particles is considered an implementation detail.

### Conventions

#### E.C.1 Process IDs

It is not appropriate to mandate a specific set of interaction processes and assign them IDs in this standard. Different models make different choices, and it is not possible to foresee modelling developments that would require new process IDs to be defined in the future. Instead, the ranges of IDs given below are *recommended* for high-level categorization of processes. Even if an implementation uses the convention in the table below, it must still adhere to [G.R.4](#gr4-process-metadata).

| Identifier | Process                       |
| ---------- | -------                       |
| 100-199    | Low-Energy Nuclear Scattering |
| 200-299    | Quasielastic                  |
| 300-399    | Meson Exchange Current        |
| 400-499    | Resonance production          |
| 500-599    | Shallow inelastic scattering  |
| 600-699    | Deep inelastic scattering     |
| 700-       | Other process types           |

Charged current (CC) processes should have identifiers in the X00-X49 block and neutral current (NC) in the X50-X99 block.
Negative process IDs may be reserved for electromagnetic interactions in neutrino event generators that include them.
In the above Table, we provide recommended ID ranges for shallow inelastic scattering (SIS) and deep inelastic scattering (DIS).
We acknowledge that the boundary between these two is somewhat ambiguous, but recommend to follow the traditional definitions for DIS defined by the
parton distribution function (PDF) community. In the PDF community, the different sets need to define a scale at which the non-perturbative PDFs are defined.
This value is traidtionally taken to be given by Q^2 >~ 2 GeV^2 and W >~ 2 GeV. The SIS region would then be defined as the region where the resonance production
ends to where DIS starts. However, again the exact definitions of each region is generator dependent and left to each generator to uniquely define their processes.
There is no mandatory inclusion of SIS at all if a generator chooses to not include it.

These ranges are subject to change in a future version of this specification.

#### E.C.2 Total Cross Section

The total cross-section for the incoming beam particle, with its specific energy, to interact with the target particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named `"TotXS"`.
See [G.C.4](#gc4-cross-section-units-and-target-scaling) for conventions on signalling cross section units.

#### E.C.3 Process Cross Section

The total cross-section for the selected process ID for the incoming beam particle, with its specific energy, to interact with the target particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named "ProcXS".
See [G.C.4](#gc4-cross-section-units-and-target-scaling) for conventions on signalling cross section units.

#### E.C.4 Estimated Flux-Averaged Total Cross Section

Some simulations build up an estimate of the cross section as they run. This makes implementing [G.C.5](#gc5-flux-averaged-total-cross-section) impractical in many cases. As an alternative, the builtin attribute `HepMC3::GenCrossSection`, accessed via `GenEvent::cross_section`, should be used to store the current estimate of the total cross section. A user should only ever need to use the estimate provided with the last event read to correctly scale an event rate to a cross-section prediction. This means statistically correct predictions can be made without reading an event vector to the end. The included `HepMC3::GenCrossSection::cross_section_errors` data member can be used to decide when enough events have been read to reach some desired statistical precision on the total cross section. The best estimate from the generator run will always be provided on the final event in a vector.

For event generators that do not currently provide the value of the total cross section in the output, Appendix B of the [NuHepMC paper](https://arxiv.org/abs/2310.13211) provides suggestions for algorithms for computing a running estimate and associated Monte Carlo statistical uncertainty as events are produced.

When implementing this convention, ensure that the `cross_sections` and `cross_section_errors` data members are the same length as the number of weights defined in the header. These should be filled with the current estimate of the total cross section for each variation based on all events generated so far, including the current event.
Additionally, the `HepMC3::GenCrossSection` data members `accepted_events` and `attempted_events` should be filled with appropriate values.

#### E.C.5 Lab Time

If the "LabPos" attribute vector contains three entries then it is assumed to only contain the spatial position of the event. If it contains four entries, then the fourth entry is interpreted the time of the event in seconds.

### Suggestions

#### E.S.1 Beam Description (Beam Simulation)

For more complex beam simulations can not adequately be described by a single energy or energy histogram (see [G.C.7](#gc7-beam-energy-distribution-description)), it is suggested that the full parent decay history is included in the `HepMC3::GenEvent`. A full set of conventions for the description of beam particle production and parent particle decay chains (for the case of neutrino beams) is currently outside the scope of this specification, but generator implementations can signal that they adhere to this suggestion to notify users that some or all of the beam particle production information is included in the event.

## Vertex Information

The vertices in a HepMC3 event are used to connect groups of incoming and
outgoing particles. For the vertex information, there is only one requirement and one convention
in the present version of the NuHepMC standard.

### Requirements

#### V.R.1 Vertex Status Codes

We extend the HepMC3 definition of `HepMC3::GenVertex::status` to include the concept of a primary vertex, corresponding to the _primary_ process (_i.e._ the one labelled by [E.C.1](#ec1-process-ids)), and a final state interaction FSI summary vertex. The full set of defined status codes can be found in the table below. Implementations are free to define specific vertex status codes to refer to individual FSI (or ISI) processes and output as much information as they require. However, a single summary vertex may be useful for some simulated vectors if the full FSI history is very detailed or not often needed by users.

| Status Code | Meaning                               | Usage                     |
| ----------- | ------------------------------------- | ------------------------- |
| 0           | Not defined                           | Do not use                |
| 1           | Primary vertex                        | Recommended for all cases |
| 2           | FSI Summary vertex                    | Recommended for all cases |
| 3-20        | Reserved for future NuHepMC standards | Do not use                |
| 21-999      | Generator-dependent                   | For generator usage       |

Any secondary vertex included within a NuHepMC event may have a status between 21 and 999. Note that [G.R.5](#gr5-vertex-status-metadata) requires that all generator-specific status codes must be fully described by attributes stored in the `HepMC3::GenRunInfo`.

### Conventions

#### V.C.1 Bound Nucleon Separation Vertex

When an interaction with a nucleon bound within a nucleus with definite kinematics is simulated, a `HepMC3::GenVertex` corresponding to the separation of the struck nucleon and the nuclear remnant may be included and assigned status code 21. If this convention is signalled via the mechanism described in [G.C.1](gc1-signalling-followed-conventions), then status code 21 need not be included in the implementation of [G.R.5](#gr5-vertex-status-metadata).

## Particle Information

In the current version of the NuHepMC standard, there is only a single
requirement and two conventions for the particle information.

### Requirements

#### P.R.1 Particle Status Codes

We extend the HepMC3 definition of `HepMC3::GenParticle::status` slightly to include the concept of a target particle. For neutrino scattering, this will usually be a target nucleus.
The status codes are defined in the table below.

| Status Code | Description                           | Usage                                                   |
| ----------- | ------------------------------------- | ------------------------------------------------------- |
| 0           | Not defined                           | Do not use                                              |
| 1           | Undecayed physical particle           | Recommended for all cases                               |
| 2           | Decayed physical particle             | Recommended for all cases                               |
| 3           | Documentation line                    | Used for in/out particles in the primary process        |
| 4           | Incoming beam particle                | Recommended for all cases                               |
| 5-10        | Reserved for future HepMC3 standards  | Do not use                                              |
| 11-19       | Reserved for future NuHepMC standards | Do not use                                              |
| 20          | Target particle                       | Recommended for all cases                               |
| 21-200      | Generator-dependent                   | For generator usage                                     |
| 201-        | Simulation-dependent                  | For simulation software usage                           |

Note especially that any incoming real particle must have a status of 4 or 20, and any outgoing real particle must have a status of 1. Special care must be taken when including the effects of initial-state and final-state interactions.

Any internal particle included within a NuHepMC event may have a status in the range 21-200. Note that [G.R.6](#gr6-particle-status-metadata) requires that all generator-specific status codes must be fully described by attributes stored in the `HepMC3::GenRunInfo`.

### Conventions

#### P.C.1 Struck Nucleon Status

When an interaction with a bound nucleon with definite kinematics is simulated, the internal `HepMC3::GenParticle` corresponding to the bound nucleon should have status code 21. If this convention is signalled via the mechanism described in [G.C.1](gc1-signalling-followed-conventions), then status code 21 need not be included in the implementation of [G.R.6](#gr6-particle-status-metadata).

#### P.C.2 Nuclear Remnant Particle Code
HepMC3 places restrictions on all external particles in the event graph to facilitate automatic checking of four momentum conservation at the event graph level.
As a result, we define the new particle number 2009900000 to identify a nuclear remnant that is not precisely handled by the interaction simulation.
If a generator does correctly simulate any nuclear remnant, such that the total mass and energy are well defined, then this convention is unneccessary and the remnant can be written out as a particle with a standard PDG nuclear number and mass and momenta set as normal.
A pseudoparticle with this number is explicitly an implementation detail that should be used to abide by HepMC3 constraints to not have external vertices but should not be considered for physics analyses or onward simulation.
The number is chosen, according to the PDG scheme (see the [Monte Carlo Particle Numbering Scheme](https://pdg.lbl.gov/2023/mcdata/mc_particle_id_contents.html) for details), to be outside the range reserved for nuclear and quark-content particles and signals that it is a non-standard code by having the 6th and 7th least significant digits set to 9.

If the kinematics of the nuclear remnant are not known, but the number of protons and neutrons are well defined, then the PDG nuclear number may be added as a `HepMC3::IntAttribute` on the `HepMC3::GenParticle` named `"remnant_particle_number"`.

If this convention is signaled via the mechanism described in [G.C.1](#gc1-signalling-followed-conventions), then the particle number 2009900000 need not be included in the implementation of [G.R.8](#gr8-non-standard-particle-numbers-pdg-mc-codes).

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

* S. Gardiner: `gardiner[at]fnal[dot]gov`
* J. Isaacson: `isaacson[at]fnal[dot]gov`
* L. Pickering: `luke.pickering[at]rhul[dot]ac[dot]uk`
