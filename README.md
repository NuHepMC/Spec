NuHepMC Specification Version 1.0.0
----------------------------------

# Abstract

In this specification we present an additional set of *Requirements*, 
*Conventions*, and *Suggestions* for simulated interactions stored in 
[HepMC3](https://doi.org/10.1016/j.cpc.2020.107310) format. These are designed 
to be used in the specific context of neutrino event generators and associated 
simulation and analysis toolchains. By doing so, we hope to lower the barrier 
for interfacing with the output of a range of interaction simulations.

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
* [Generator Adoption of NuHepMC](#generator-adoption-of-nuhepmc)
* [Contributing and NuHepMC Revisions](#contributing-and-nuhepmc-revisions)
* [Major Changelog](#major-changelog)
* [Contact](#contact)

# Introduction

In the accelerator neutrino HEP subfield, currently, each event generator, 
experimental simulation, and analysis framework maintains their own proprietary 
set of interfaces and format converters. By developing an extensible and 
extendable set of conventions on top of the versatile and mature HepMC3 we aim 
to cater to analysers and simulation framework developers who want to handle 
interactions in a tool-agnostic way, while also not putting any constraints on 
the information that individual event generators output, enabling lossless, 
bidirectional format conversion with existing proprietary formats.

The specification is broken down into *Requirements*, *Conventions*, and 
*Suggestions* for each of the following HepMC3 components.

* G: Generator Run Metadata
* E: Event Metadata
* V: Vertex Information
* P: Particle Information

Elements of the specification are enumerated like 
\<Component\>.\<Category\>.\<Index\>, _i.e._ the _fifth_ _Convention_ for 
  _Event_ information should be referred to as [E.C.5](#ec5-lab-time).

Following named conventions is optional. If conventions prove useful and are 
considered stable, they may become requirements in future revisions. Suggestions
cover information that is useful to make available to users, but has not 
routinely been included in proprietary formats or may not be simple to include
in every implementation. [G.R.4](#gr4-signalling-followed-conventions) provides
a requirement on signalling which optional Conventions or Suggestions have been
followed.

See 
[NuHepMC/ReferenceImplementation](https://github.com/NuHepMC/ReferenceImplementation) 
for utilities for writing, reading, and interpreting NuHepMC files in C++ and 
Python; 
[NuHepMC/ReferenceImplementation](https://github.com/NuHepMC/ReferenceImplementation) 
for the reference implementation of a NuHepMC event writer; 
[NuHepMC/Validator](https://github.com/NuHepMC/Validator) for a validator that 
can be used to test NuHepMC files; and 
[Generator Adoption of NuHepMC](#generator-adoption-of-nuhepmc) for a snapshot 
of the NuHepMC adoption status.

# NuHepMC

This document is a _living_ version of the 
[NuHepMC specification paper](https://arxiv.org/abs/2310.13211).

## A Note on the Usage of HepMC3 C++ Class Names

Throughout this standard, references are made to various HepMC3 C++ types, 
_e.g._ `HepMC3::GenRunInfo`, which are equivalent to the namespace-qualified 
C++ typename in the 
[HepMC3 reference implementation](https://gitlab.cern.ch/hepmc/HepMC3). However,
these are used as a convenient handle for data objects. This specification 
should not be considered specific to the HepMC3 C++ reference implementation.

## Generator Run Metadata

The generator run metadata describes the overall setup of the event generator, 
_i.e._, information that is not unique to a specific event. The NuHepMC 
specifications for this metadata are as follows:

### Requirements

#### G.R.1 Valid GenRunInfo

All NuHepMC vectors must contain a `HepMC3::GenRunInfo` instance.

_n.b._ The term _vector_ is used herein, rather than _file_, as HepMC3 events 
are frequently piped or streamed between MC tools without ever being wholly 
persisted to memory or disk.

#### G.R.2 NuHepMC Version

A NuHepMC `HepMC3::GenRunInfo` instance must contain the following three 
`HepMC3::IntAttribute`s that specify the version of NuHepMC that is implemented

* `"NuHepMC.Version.Major"`
* `"NuHepMC.Version.Minor"`
* `"NuHepMC.Version.Patch"`

This document describes *version 1.0.0* of NuHepMC. For a list of breaking 
changes between major versions, see [Major Changelog](#major-changelog).

_n.b._  A script is provided for converting existing NuHepMC files from an older
version of the standard to the latest version of the standard at 
[scripts/NuHepMC_update_standard.py](scripts/NuHepMC_update_standard.py).

#### G.R.3 Generator Identification

A NuHepMC `HepMC3::GenRunInfo` instance must contain a 
`HepMC3::GenRunInfo::ToolInfo` for each 'tool' involved in the production of the
vector thus far. The `ToolInfo` instance must contain non-empty name and version
fields.

#### G.R.4 Signaling Followed Conventions

To signal to a user that an implementation follows a named convention from
this specification, a `HepMC3::VectorStringAttribute` attribute must be added to
the `HepMC3::GenRunInfo `instance named `"NuHepMC.Conventions"`
containing the labels of the conventions adhered to, _e.g._ `"G.C.2"`. 

_n.b._ [G.R.5](#gr5-flux-averaged-total-cross-section) requires the 
implementation of at least one convention and so this attribute must exist.

#### G.R.5 Flux-Averaged Total Cross Section

The flux-averaged total cross section is a scaling factor that is
needed to convert a distribution of simulated events into a cross-section 
prediction. Details on the mathematical definition of this quantity are given in 
the [NuHepMC specification paper](https://arxiv.org/abs/2310.13211), Sec. 3.1. 
If the value is known at the start of a generator run, convention [G.C.2](#gc2-flux-averaged-total-cross-section) should be 
used to store it in the `HepMC3::GenRunInfo`.

In general, the value is not known at the start of a generator run, but it can 
be calculated as events are produced (see 
[NuHepMC specification paper](https://arxiv.org/abs/2310.13211) Appendix A). In 
this case, convention [E.C.4](#ec4-estimated-flux-averaged-total-cross-section)
should be used to store the running estimate and  associated statistical 
uncertainty in each `HepMC3::GenEvent`.

One of these two methods for communicating the flux-averaged total cross section
to users must be implemented and signaled via 
[G.R.4](#gr4-signaling-followed-conventions).

#### G.R.6 Cross Section Units and Target Scaling

There are a variety of units typically used to report both measured and 
predicted cross sections in HEP. For neutrino cross sections specifically,
`10^{-38} cm^2` per nucleon is common, but not ubiquitous. Both of the
following `HepMC3::StringAttribute`s must be included on the 
`HepMC3::GenRunInfo` to indicate the cross section units used within a vector. 
Possible values of the attributes are not restricted by this specification, but
the meanings of the following values are reserved to standardize existing 
conventions. It is strongly recommended that new implementations use these 
wherever possible.

* `"NuHepMC.Units.CrossSection.Unit"`
  * `"pb"`: Picobarns or `10^{-36} cm^2`. Our recommendation
  * `"10-38 cm2"`: The choice of `10^{-38} cm^2` is the most frequent in the 
    neutrino literature.
  * `"cm2"`: Using bare cm^2 in this option, without any power-of-ten scaling, 
    is not recommended due to numerical precision concerns. The natural scale 
    of neutrino--nucleon cross sections is approximately `10^{-38}`, which is 
    very close to the minimum representable IEEE 754 single-precision floating 
    point number.
* `"NuHepMC.Units.CrossSection.TargetScale"`
  * `"PerAtom"`:  Our recommendation.
  * `"PerNucleon"`: A common alternative in the literature.

The chosen units should be assumed to apply to the reported value of the 
flux-averaged total cross section, which is a property of the generator run 
and may be stored according to either 
[G.C.2](#gc2-flux-averaged-total-cross-section) or 
[E.C.4](#ec4-estimated-flux-averaged-total-cross-section). Consistent units must
also be used for the total ([E.C.2](#ec2-total-cross-section)) and 
process-specific ([E.C.3](#ec3-process-cross-section)) cross sections describing
the primary interaction in each individual event, if they are provided.

It is ultimately up to the user to parse these attributes and decide whether
any additional scaling is needed for their purposes, but the use of the above 
reserved values will facilitate automated processing. The recommended value of 
picobarns per atom is chosen to remain consistent with the assumptions of other
tools that read and write HepMC3 files beyond the neutrino community, such as 
[Rivet](https://gitlab.com/hepcedar/rivet/). Consistency facilitates 
interoperability and reduces maintenance burdens.

#### G.R.7 Event Weights

For weights that will be calculated for every event, HepMC3 provides an 
interface for storing the weight names only once in the `HepMC3::GenRunInfo` 
instance. At least one event weight named `"CV"` must be declared on the 
`HepMC3::GenRunInfo` instance and filled for every event.

This weight may be 1 or constant for every event in a generator run 
(in the case of an _unweighted_ event vector). This weight must always be 
included by a user when producing cross section predictions from a NuHepMC 
vector and should never be assumed to be 1 for every event.

#### G.R.8 Process Metadata

A NuHepMC `HepMC3::GenRunInfo` instance must contain a 
`HepMC3::VectorIntAttribute` named `"NuHepMC.ProcessIDs"` listing all physics 
process Ids as integers.

For each valid process Id, the `HepMC3::GenRunInfo` instance must also contain 
two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.ProcessInfo[<Id>].Name"`
* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.ProcessInfo[<Id>].Description"`

where `<ID>` enumerates all process IDs present in `"NuHepMC.ProcessIDs"`. 
(See also [E.C.1](#ec1-process-id))

#### G.R.9 Vertex Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a 
`HepMC3::VectorIntAttribute` named `"NuHepMC.VertexStatusIDs"` declaring any 
generator-specific vertex status codes used. Including the standard HepMC3 codes
in this list is optional, but they must not be reused to mean something 
different than in the HepMC3 specification.

For each declared vertex status, the `HepMC3::GenRunInfo` instance must also 
contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: `"NuHepMC.VertexStatusInfo[<Id>].Name"`
* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.VertexStatusInfo[<Id>].Description"`

where `<ID>` enumerates all status codes present in `"NuHepMC.VertexStatusIDs"`.
(See also [V.R.1](#vr1-vertex-status-codes))

#### G.R.10 Particle Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a 
`HepMC3::VectorIntAttribute` named `"NuHepMC.ParticleStatusIDs"` declaring any 
generator-specific status codes used. Including the standard HepMC3 codes in 
this list is optional, but they must not be reused to mean something different 
than in the HepMC3 specification.

For each valid particle status, the `HepMC3::GenRunInfo` instance must also 
contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.ParticleStatusInfo[<Id>].Name"`
* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.ParticleStatusInfo[<Id>].Description"`

where `<ID>` enumerates all status codes present in 
`"NuHepMC.ParticleStatusIDs"`. (See also [P.R.1](#pr1-particle-status-codes))

#### G.R.11 Non-standard Particle Numbers (PDG MC Codes)

Essentially all event generators in HEP use a standard set of integer codes
to identify particle species. This numbering scheme is maintained by
the Particle Data Group (PDG) and is regularly updated in their Review
of Particle Physics: 
[Monte Carlo Particle Numbering Scheme](https://pdg.lbl.gov/2023/mcdata/mc_particle_id_contents.html).

It is expected that neutrino event generators may need to use codes for 
non-standard particle species (_i.e._, those without an existing PDG code) for 
a variety of applications. This could include simulating exotic physics 
processes involving new particles as well as implementing bookkeeping methods 
involving generator-specific pseudoparticles.

The NuHepMC `HepMC3::GenRunInfo` instance must contain a 
`HepMC3::VectorIntAttribute` named `"NuHepMC.AdditionalParticleNumbers"` 
declaring any particle codes used that are not defined in the current PDG 
numbering scheme. Including any of the standard codes in this list is permitted
but not required. The standard particle codes must not be reused to mean 
something different than in the PDG specification.

For each additional particle number, the `HepMC3::GenRunInfo` instance must also
contain an attribute giving a unique name to the represented particle species:

* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.AdditionalParticleNumber[<PDG>].Name"`

where `<PDG>` enumerates all particle numbers present in 
`"NuHepMC.AdditionalParticleNumbers"`.

See also [G.C.8](#gc8-non-standard-particle-number-descriptions) for a 
suggested way of storing descrptions of these special particle species.

### Conventions

#### G.C.1 Vector Exposure

Each vector should contain a description of the exposure of the generator run. 
When simulating with some experimental exposure, often represented for 
accelerator neutrino experiments in units of "protons on target" (POT), the 
exposure should be described. Two attributes are reserved for signaling the 
exposure used to users. One or both can be provided.

* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.POT" `
* type: `HepMC3::DoubleAttribute`, name: `"NuHepMC.Exposure.Livetime"`

#### G.C.2 Flux-averaged Total Cross Section

If known at the start of a run, the value of the flux-averaged total cross 
section should be stored as a `HepMC3::DoubleAttribute` in the 
`HepMC3::GenRunInfo` named `"NuHepMC.FluxAveragedTotalCrossSection".` 
Optionally, the uncertainty in the flux-averaged total cross section may be 
stored as a `HepMC3::DoubleAttribute` in the `HepMC3::GenRunInfo` named
`"NuHepMC.FluxAveragedTotalCrossSectionUncertainty"`.

See [E.C.4](#ec4-estimated-flux-averaged-total-cross-section) if the cross 
section is not known at the start. Also, recall that from 
[G.R.5](#gr5-flux-averaged-total-cross-section), either this convention or 
[E.C.4](#ec4-estimated-flux-averaged-total-cross-section) must be used.

#### G.C.3 Citation Metadata

Modeling components implemented based on published work should always be fully 
cited. The `HepMC3::GenRunInfo` should contain at least one 
`HepMC3::VectorStringAttribute` for each relevant modeling component, named 
according to the pattern `"NuHepMC.Citations.<Comp>.<Type>"`. Valid 
substitutions for the `<Comp>` and `<Type>` fields are not restricted by this
standard beyond the requirement that they are pure mixed-case alpha-numeric. 
The keys `<Comp>=Generator` and `<Comp>=Process[<ID>]` are reserved for use in 
specifying the main citation(s) for the interaction generator and for citing 
theoretical motivations behind individual processes, respectively. For common 
reference formats in the HEP field, some reserved values for the 
`<Type>` field are:

* `"InspireHep"` might contain one or more unique InspireHep identifiers 
  (texkeys).
* `"arXiv"` might contain one or more unique arXiv identifiers (eprint numbers).
* `"DOI"` might contain one or more unique Digital Object Identifiers.

_n.b._ If you don't cite your contributing theorists, and make it easy for users 
of  your simulation to cite them as well, then don't come crying to us when they
cannot find a job to stay in the field.

A tool that can read this metadata and automatically produce a BibTeX file 
containing entries for all cited publications can be found at 
[NuDevTools/HEPReference](https://github.com/NuDevTools/HEPReference).

#### G.C.4 Beam Energy Distribution Description

Each vector should contain a description of the beam particle flux used to
simulate the output event vector. For many MC studies and experimental
simulations where the detector is not physically close to the source, a simple
univariate energy distribution is enough to describe the particle beam. The two
types of energy distribution covered by this convention are mono-energetic beams
and those with distributions described by a simple histogram. The type should 
be signalled via a `HepMC3::StringAttribute` named `"NuHepMC.Beam[<PDG>].Type"` 
with value `"MonoEnergetic"` or `"Histogram"` stored on the 
`HepMC3::GenRunInfo`. For both types, relevant units can be signalled via two 
attributes:

* `"NuHepMC.Beam[<PDG>].EnergyUnit"`. Possible values of the attribute are not 
  restricted, but we reserve the meanings of `"MEV"` and `"GEV"`. This attribute
  should always exist and be not empty.
* `"NuHepMC.Beam[<PDG>].RateUnit"`. Possible values of the attribute are not 
  restricted, but we reserve the meaning of `"Arbitrary"` to signal that the 
  normalization of the distribution was not known or used by the simulation. 
  If this attribute is not used then the normalization will be assumed 
  arbitrary.

For the case of a `"MonoEnergetic"`-type distribution, all beam particles in the
vector must have identical energy. The attribute 
`"NuHepMC.Beam[<PDG>].MonoEnergetic.Energy"` can be used to signal the beam 
energy in the lab frame, but the usage of this attribute is optional as the 
energy can be determined from the first (or any) event in the vector.

For the case of a `"Histogram"`-type distribution, the histogram should be 
encoded into two `HepMC3::VectorDoubleAttribute` per beam species on the 
`HepMC3::GenRunInfo`:

* `"NuHepMC.Beam[<PDG>].Histogram.BinEdges"`
* `"NuHepMC.Beam[<PDG>].Histogram.BinContent"`

where `[<PDG>]` enumerates the PDG particle numbers of all beam particles 
present in the event vector. _n.b._ the number of entries in the `"BinEdges"` 
vector should always be one more than the number of entries in the 
`"BinContent"` vector.

The `HepMC3::BoolAttribute`,
* `"NuHepMC.Beam[<PDG>].Histogram.ContentIsPerWidth"`,

should be used to signal that the number of neutrinos in a given histogram is 
found by multiplying the bin content by the bin width, rather than from the 
content alone. While this might be determined by parsing the `RateUnit` 
attribute, existing neutrino generators make different assumptions when sampling
input neutrino beam energy distributions, so an explicit attribute is reserved 
to reduce ambiguity. If this attribute is not provided, then it is expected that
the number of neutrinos in a given bin is specified by the bin content alone and
is independent of the width of the bin.

For a suggestion on how to encode useful information about more realistic 
neutrino beam descriptions, see [E.S.1](#es1-beam-description-beam-simulation).

General flux handling is a complex problem that is not currently standardized. 
This convention is suitable for simplified studies by theorists and 
phenomenologists interested in accelerator-based neutrino experiments. A 
standardized flux format usable for many other applications, _e.g._, atmospheric
neutrino measurements, as well as its corresponding description within the event
record, is outside the scope of this specification. Development of such a 
standard is left to future community efforts.

#### G.C.5 Non-standard Particle Number Descriptions

For each additional particle number `<PDG>` declared in the 
`"NuHepMC.AdditionalParticleNumbers"` attribute, according to 
[G.R.11](#gr11-non-standard-particle-numbers-pdg-mc-codes), the 
`HepMC3::GenRunInfo` instance may contain an attribute giving a description of 
the particle:

* type: `HepMC3::StringAttribute`, name: 
  `"NuHepMC.AdditionalParticleInfo[<PDG>].Description"`

For non-standard particles that should be further simulated by particle 
propagation simulations, such as [GEANT4](https://gitlab.cern.ch/geant4/geant4),
additional information encoded here may be enough to enable automatic 
propagation. In this version of NuHepMC, no attempt is made to standardize a 
format for such information, but it is suggested that `HepMC3::GenRunInfo` 
attributes of the form, 
`"NuHepMC.AdditionalParticleNumber[<PDG>].<SimName>.<AttrName>"`, might be 
useful for communicating such additional information. These additional 
attributes should include, at a minimum, the particle's mass, width, spin, and 
electric charge.

### Suggestions

#### G.S.1 Run Configuration

It is suggested that a NuHepMC `HepMC3::GenRunInfo` instance should contain all 
information required to reproduce the events in the vector. This may be stored 
in attributes with names beginning with `"NuHepMC.Provenance"`. The information 
required will necessarily be generator-specific, but it should be noted that 
storing the full initial state of any random number generators used during the 
generator run is crucial for exact reproduction.

#### G.S.2 Complete Status Metadata

While [G.R.9](#gr9-vertex-status-metadata) and 
[G.R.10](#gr10-particle-status-metadata) explicitly do not require 
implementations to emit metadata for standard status codes defined in the HepMC3
standard, it is suggested that the complete list of status codes used by an 
implementation are included in the `"NuHepMC.VertexStatusInfo"` and 
`"NuHepMC.ParticleStatusInfo"` attributes.

## Event Metadata

The event is used to store information about one primary interaction process and
any relevant secondary processes. An event is described by arbitrary metadata 
and a graph of particles (edges) and vertices (nodes), each with their own 
arbitrary metadata.

The NuHepMC specifications for events are as follows:

### Requirements

#### E.R.1 HepMC3 Compatibility:

The HepMC3 standard places some constraints on valid event graphs, these 
constraints must be respected by valid NuHepMC events as we require full 
compatibility with HepMC3. More details of these constraints can be found in the
[HepMC3 specifications](https://doi.org/10.1016/j.cpc.2020.107310).

Existing neutrino event generators often rely on effective descriptions of the 
nuclear environment in a neutrino-nucleus hard scattering process. This means 
that four-momentum may not be explicitly conserved for the neutrino-nucleus 
system. Energy and momentum can be exchanged with a _nuclear remnant_, which is
often not treated carefully in the simulation of a primary neutrino-nucleon 
collision, via initial- and final-state interactions. Implementations are of 
course free to thoroughly model all involved particles, conserving four-momentum
over the whole system, including a fully-simulated nuclear remnant. For those 
implementations where such a requirement is not feasible or would delay the 
adoption of this standard, [P.C.2](#pc2-nuclear-remnant-particle-code) reserves
a non-standard particle number that can be used to represent a nuclear remnant
that is not precisely simulated.

#### E.R.2 Event Number

Each `HepMC3::GenEvent` must have a non-negative event number that is unique 
within a given vector.

#### E.R.3 Process ID

The process ID for the primary physics process that is represented in the 
`HepMC3::GenEvent` must be recorded in a `HepMC3::IntAttribute` named 
`"signal_process_id"`. The metadata for this process ID must be stored according
to [G.R.8](#gr8-process-metadata).

#### E.R.4 Units

Energy and position units must be explicitly set in the `HepMC3::GenEvent`.

#### E.R.5 Lab Position

The position of the event in the lab frame must be added as a 
`HepMC3::VectorDoubleAttribute`, named `"lab_pos"`, with the same units as used 
when implementing [E.R.4](#er4-units). See [E.C.5](#ec5-lab-time) for how to 
optionally store time in this attribute. If the simulation did not involve a 
macroscopic geometry, then this attribute may be set to `[0, 0, 0]`.

#### E.R.6 Vertices

An event must have at least one `HepMC3::GenVertex`, and must have one and 
only one with a _primary interaction vertex_ status code. No `HepMC3::GenVertex`
may have a _not defined_ status code. 
(See [V.R.1](#vr1-vertex-status-codes) for additional details).

#### E.R.7 Beam and Target Particles

An event must contain exactly one particle with the _incoming beam particle_ 
status code and one particle with the _target particle_ status code 
(see [P.R.1](#pr1-particle-status-codes)). We recommend that, in cases where the
colliding initial-state particles are distinct, the more massive of the two 
should be considered the target. For neutrino scattering, the target will thus
often be a complex nucleus or a free nucleon. In the case of equally massive
particles, the choice to label one of them as the target is arbitrary.

_n.b._ A nucleon (or quark) bound within a nucleus should never be marked as the
target particle; the nucleus itself should be considered the target. 
[P.C.1](#pc1-struck-nucleon-status) provides a convention for marking a 
constituent bound nucleon struck by the incoming beam particle in the event 
graph.

#### E.R.8 Event Completeness

All simulated incoming and outgoing physical particles must be written to the 
event. The storage of intermediate particles is considered an implementation 
detail.

#### E.R.9 Event Frame of Reference

As the primary use of HepMC3 is for experiment simulation, all particle momenta 
specified via standard HepMC3 mechanisms must be defined in the lab frame. From 
the information required by a standard HepMC3 event, arbitrary frame 
transformations should be possible and can be performed by the 
`HepMC3::GenEvent::boost` method. If storing kinematic information in other 
frames of reference facilitates later calculations, this should be done via 
simulation-specific `HepMC3::Attribute`s on the relevant `HepMC3::GenParticle`s.

### Conventions

#### E.C.1 Process IDs

It is not appropriate to mandate a specific set of interaction processes and
assign them IDs in this standard. Different models make different choices, and
it is impossible to foresee modeling developments that would require new process
IDs to be defined in the future. Instead, the ranges of IDs given below are 
provided as a categorization of processes to facilitate high-level analysis. If 
a given process naturally fits in one of these categories it can be helpful to 
choose IDs that follow this convention, but an explicit aim of this standard is 
to avoid constraining or defining what processes can be stored and described. 

_n.b._ Even if an implementation uses this convention, it must still adhere to 
[G.R.8](#gr8-process-metadata). 

| Identifier | Process                       |
| ---------- | -------                       |
| 100-199    | Low-Energy Nuclear Scattering |
| 200-299    | Quasielastic                  |
| 300-399    | Meson Exchange Current        |
| 400-499    | Resonance production          |
| 500-599    | Shallow inelastic scattering  |
| 600-699    | Deep inelastic scattering     |
| 700-       | Other process types           |

If following this convention, charged-current (CC) processes should have 
identifiers in the X00-X49 block and neutral-current (NC) processes should have 
them in the X50-X99 block. Negative process IDs should be used for 
electromagnetic interactions in event generators that include them. 

Distinct ranges for shallow inelastic scattering and deep inelastic scattering 
are provided because existing neutrino generators often make such a distinction 
in both the modeling details and simulated event metadata. It should be noted 
that the boundary between these two is not _a priori_ well defined and choices 
varies significantly in current neutrino scattering simulations, usually defined 
with reference to some boundary value in the invariant mass of the hadronic 
final state. The lack of consensus and common definitions hinders progress in 
modeling neutrino-induced low energy hadronization.

#### E.C.2 Total Cross Section

The total cross-section for the incoming beam particle, with its specific 
energy, to interact with the target particle should be stored in a 
`HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named `"tot_xs"`.
See [G.R.6](#gr6-cross-section-units-and-target-scaling) for how to communicate 
cross section units.

#### E.C.3 Process Cross Section

The total cross-section for the selected process ID for the incoming beam 
particle, with its specific energy, to interact with the target particle should 
be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named 
`proc_xs`. See [G.R.6](#gr6-cross-section-units-and-target-scaling) for how to 
communicate  cross section units.

#### E.C.4 Estimated Flux-Averaged Total Cross Section

Some simulations build up an estimate of the flux-averaged total cross section 
as they run. This makes implementing 
[G.C.2](#gc2-flux-averaged-total-cross-section) impractical in many cases. As an
alternative, the builtin attribute `HepMC3::GenCrossSection`, accessed via 
`GenEvent::cross_section`, should be used to store the current estimate of the 
total cross section. A user should only ever need to use the estimate provided 
with the last event that they read to correctly scale an event rate to a 
cross-section prediction. This means statistically correct predictions can be 
made without reading an event vector to the end. The included 
`HepMC3::GenCrossSection::cross_section_errors` data member can be used to 
decide when enough events have been read to reach some desired statistical 
precision on the total cross section. The best estimate from the generator run 
will always be provided on the final event in a vector.

For event generators that do not currently provide the value of the total cross 
section in the output, Appendix B of the 
[NuHepMC paper](https://arxiv.org/abs/2310.13211) provides an algorithm for 
computing a running estimate, with associated Monte Carlo statistical 
uncertainty, as events are generated.

When implementing this convention, ensure that the `cross_sections` and 
`cross_section_errors` data members are the same length as the number of weights
defined in the header. These should be filled with the current estimate of the 
total cross section for each variation based on all events generated so far, 
including the current event. Additionally, the `HepMC3::GenCrossSection` data 
members `accepted_events` and `attempted_events` should be filled with 
appropriate values.

#### E.C.5 Lab Time

If the `"lab_pos"` ([E.R.5](#er5-lab-position)) attribute vector contains three
entries then it is assumed to only contain the spatial position of the event. If
it contains four entries, then the fourth entry is interpreted the time of the
event in seconds.

### Suggestions

#### E.S.1 Beam Description (Beam Simulation)

For more complex beam simulations that can not adequately be described by a 
single energy or energy histogram (see 
[G.C.4](#gc4-beam-energy-distribution-description)), it is suggested that the 
full parent decay history is included in the `HepMC3::GenEvent`. A full set of 
conventions for the description of beam particle production and parent particle
decay chains (for the case of neutrino beams) is currently outside the scope of
this specification, but generator implementations can signal that they adhere to
this suggestion to notify users that some or all of the beam particle production
information is included in the event.

## Vertex Information

The vertices in a HepMC3 event are used to connect groups of incoming and
outgoing particles. For the vertex information, there is only one requirement 
and one convention in the present version of the NuHepMC standard.

### Requirements

#### V.R.1 Vertex Status Codes

The standard HepMC3 range of reserved values for `HepMC3::GenVertex::status` is 
extended to include the concept of a primary vertex, corresponding to the 
_primary_ process (_i.e._ the one labelled by [E.C.1](#ec1-process-ids)), and a
final state interaction FSI summary vertex. The full set of defined status codes
can be found in the table below. Implementations are free to define specific 
vertex status codes to refer to individual FSI (or ISI) processes and output as
much information as they require. However, a single summary vertex may be useful
for some simulated vectors if the full FSI history is very detailed or not often
needed by users.

| Status Code | Meaning                               | Usage                     |
| ----------- | ------------------------------------- | ------------------------- |
| 0           | Not defined                           | Do not use                |
| 1           | Primary vertex                        | Recommended for all cases |
| 2           | FSI Summary vertex                    | Recommended for all cases |
| 3-20        | Reserved for future NuHepMC standards | Do not use                |
| 21-999      | Generator-dependent                   | For generator usage       |

Any secondary vertex included within a NuHepMC event may have a status between 
21 and 999. 

_n.b._ [G.R.9](#gr9-vertex-status-metadata) requires that all 
generator-specific status codes must be fully described by attributes stored in 
the `HepMC3::GenRunInfo`.

### Conventions

#### V.C.1 Bound Nucleon Separation Vertex

When an interaction with a nucleon bound within a nucleus with definite 
kinematics is simulated, a `HepMC3::GenVertex` corresponding to the separation 
of the struck nucleon and the nuclear remnant may be included and assigned 
status code 21. If this convention is signalled via the mechanism described in 
[G.R.4](gr4-signalling-followed-conventions), then status code 21 need not be 
included in the implementation of [G.R.9](#gr9-vertex-status-metadata).

## Particle Information

In the current version of the NuHepMC standard, there is only a single
requirement and two conventions for the particle information.

### Requirements

#### P.R.1 Particle Status Codes

The standard HepMC3 range of reserved values for `HepMC3::GenParticle::status`
is slightly extended to include the concept of a target particle. For neutrino
scattering, this will usually be a target nucleus. The status codes are defined
in the table below.

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

_n.b._ Any incoming real particle must have a status of 4 or 20, and any 
outgoing real particle must have a status of 1. Special care must be taken when 
including the effects of initial-state and final-state interactions.

Any internal particle included within a NuHepMC event may have a status in the 
range 21-200. 

_n.b._ [G.R.10](#gr10-particle-status-metadata) requires that all
generator-specific status codes must be fully described by attributes stored in
the `HepMC3::GenRunInfo`.

### Conventions

#### P.C.1 Struck Nucleon Status

When an interaction with a bound nucleon with definite kinematics is simulated, 
the internal `HepMC3::GenParticle` corresponding to the bound nucleon should 
have status code 21. If this convention is signalled via the mechanism described
in [G.R.4](gr4-signalling-followed-conventions), then status code 21 need not 
be included in the implementation of [G.R.10](#gr10-particle-status-metadata).

#### P.C.2 Nuclear Remnant Particle Code

HepMC3 places restrictions on all external particles in the event graph to 
facilitate automatic checking of four momentum conservation at the event graph 
level. As a result, a new particle number 2009900000 is defined to identify a 
nuclear remnant that is not precisely handled by the interaction simulation. If
a generator does correctly simulate any nuclear remnant, such that the total 
mass and energy are well defined, then this convention is unnecessary and the 
remnant can be written out as a particle with a standard PDG nuclear number and
mass and momentum set as normal. A pseudoparticle with this number is explicitly
an implementation detail that should be used to abide by HepMC3 constraints to 
not have vertices with no outgoing particles. Any pseudoparticle with this 
number should not be considered for physics analyses or onward simulation. The 
number is chosen, according to the PDG scheme (see the 
[Monte Carlo Particle Numbering Scheme](https://pdg.lbl.gov/2023/mcdata/mc_particle_id_contents.html)
for details), to be outside the range reserved for nuclear and quark-content 
particles. Its status as a non-standard particle code is signaled by the 6th and
7th least significant digits being set to 9.

If the kinematics of the nuclear remnant are not known, but the number of protons and neutrons are well defined, then the PDG nuclear number may be added as a `HepMC3::IntAttribute` on the `HepMC3::GenParticle` named `"remnant_particle_number"`.

If this convention is signaled via the mechanism described in 
[G.R.4](#gr4-signalling-followed-conventions), then the particle number 
2009900000 need not be included in the implementation of 
[G.R.11](#gr11-non-standard-particle-numbers-pdg-mc-codes).

# Generator Adoption of NuHepMC

**Last updated 2025-05-12**

The current status of support for the NuHepMC standard, along with links to converters if publicly available.

| Generator | Supports NuHepMC   | Notes                                  |
| --------- | ------------------ | -------------------------------------- |
| Achilles  | Yes                | Native output format                   |
| GENIE     | No                 | Private converter available by request |
| GiBUU     | Yes (version 2025) | Native output format                   |
| MARLEY    | Yes (upcoming v2)  | Native output format                   |
| NEUT v5   | No                 | Public converter available [here](https://github.com/neut-devel/neutvect-converter)    |
| NEUT v6+  | Yes                | Native output format    |
| NuWro     | No                 | Public converter available [here](https://github.com/NuHepMC/nuwro2hepmc3)    |

# Contributing and NuHepMC Revisions

Feature requests and defect reports should be raised as an Issue on this 
repository.

# Major Changelog

* v0.9.0 -> v1.0.0:
  * New G.R.5 clarifies the requirement to provide flux-averaged total cross 
    section information via G.C.2 or E.C.4.
  * Many GenRunInfo conventions and requirements were reorganized for clarity 
    and consistency:
    + G.R.4 -> G.R.8
    + G.R.5 -> G.R.9
    + G.R.6 -> G.R.10
    + G.C.1 -> G.R.4
    + G.C.3 -> G.C.1
    + G.C.4 -> G.R.6
    + G.C.[5-8] -> G.C.[2-5]
  * (G.C.4 -> G.R.6) the reserved options for the target scale have been 
    clarified to: 
    + "PerTargetAtom" -> "PerAtom"
    + "PerTargetNucleon" -> "PerNucleon"
  * A number of Event attribute names were changed to snake_case for 
    consistency:
    + E.R.5 "LabPos" -> "lab_pos"
    + E.C.2 "TotXS" -> "tot_xs"
    + E.C.3 "ProcXS" -> "proc_xs"

# Contact

* S. Gardiner: `gardiner[at]fnal[dot]gov`
* J. Isaacson: `isaacson[at]fnal[dot]gov`
* L. Pickering: `luke.pickering[at]stfc[dot]ac[dot]uk`
