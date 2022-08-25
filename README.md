NuHepMC Specification Version 0.1.0
----------------------------------

# Abstract

In this specification we present an additional set of requirements, conventions, and suggestions for simulated interactions stored in [HepMC3](https://www.arxiv.org/pdf/1912.08005.pdf) format. These are designed to be used in specific context of neutrino event generators and associated simulation and analysis toolchains, and will lower the barrier for interfacing with the output of a range of interaction simulations.

# Introduction

Until now, each event generator, experimental simulation, and analysis framework maintains their own proprietary set of interfaces and format converters. By developing an extensible and extendable set of conventions on top of the versatile and mature HepMC3 we aim to cater to analysers and simulation framework developers who want to handle interactions in a tool-agnostic way, while also not putting any limits on the information that individual event generators want to write out, enabling lossless, bidirectional format conversion with existing proprietary formats.

The specification is broken down into Requirements, Conventions, and Suggestions (RCs) for each of the following HepMC3 components.

* G: Generator Run Metadata
* E: Event Metadata
* V: Vertex Information
* P: Particle Information

Where RCs are enumerated like \<Component\>.\<Category\>.\<Index\>, _i.e._ the second Convention for Event information should be referred to as [E.C.5](#ec5-lab-time).

Following named conventions is optional. If conventions prove useful and are considered stable, they may become requirements in future versions of this specification. [G.C.1](#gc1-signalling-followed-conventions) provides a convention on signalling that certain conventions have been followed. Suggestions cover information that is useful to make available to consumers, but has not always been included in proprietary formats or may not be simple to include in a first implementation.

See [NuHepMC/ReferenceImplementation](https://github.com/NuHepMC/ReferenceImplementation) for reference implementations of an NuHepMC event writer and [NuHepMC/Validator](https://github.com/NuHepMC/Validator) for a NuHepMC event parser and specification validator.

# NuHepMC

### HepMC3 C++ Classes

In this document we often make reference to various HepMC3 C++ classes, _e.g_ `HepMC3::GenRunInfo`, however these are used as a convenient handle for data objects and this specification should not be considered specific to the HepMC3 C++ reference implementation. Anywhere that the RCs are ambiguous outside of this context should be considered a bug in the specification.

## Generator Run Metadata

This section describes RCs for instances of `HepMC3::GenRunInfo`.

### Requirements

#### G.R.1 Valid GenRunInfo

All NuHepMC files must contain a `HepMC3::GenRunInfo` instance.

#### G.R.2 NuHepMC Version

A NuHepMC `HepMC3::GenRunInfo` instance must contain the following attributes that specify the version of NuHepMC that is implemented

* type: `HepMC3::IntAttribute`, name: "NuHepMC.Version.Major" 
* type: `HepMC3::IntAttribute`, name: "NuHepMC.Version.Minor" 
* type: `HepMC3::IntAttribute`, name: "NuHepMC.Version.Patch" 

#### G.R.3 Generator Identification

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::GenRunInfo::ToolInfo` for each 'tool' involved in the production of the file thus far. The `ToolInfo` instance must contain non-empty name and version fields.

#### G.R.4 Process Metadata

A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named "NuHepMC.ProcessIDs" listing all physics process IDs as integers.

For each valid process ID, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: "NuHepMC.ProcessInfo\[\<ID\>\].Name" 
* type: `HepMC3::StringAttribute`, name: "NuHepMC.ProcessInfo\[\<ID\>\].Description" 

where \<ID\> enumerates all process IDs present in "NuHepMC.ProcessIDs".

#### G.R.5 Vertex Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named "NuHepMC.VertexStatusIDs" declaring any generator-dependent status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each declared vertex status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: "NuHepMC.VertexStatusInfo\[\<ID\>\].Name" 
* type: `HepMC3::StringAttribute`, name: "NuHepMC.VertexStatusInfo\[\<ID\>\].Description" 

where \<ID\> enumerates all status codes present in "NuHepMC.VertexStatusIDs". _c.f._ [V.R.1](#vr1-vertex-status-codes) for more details.

#### G.R.6 Particle Status Metadata

The NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named "NuHepMC.ParticleStatusIDs" declaring any generator-dependent status codes used. Including the standard HepMC3 codes in this list is optional, but they must not be reused to mean something different than in the HepMC3 specification.

For each valid particle status, the `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of each:

* type: `HepMC3::StringAttribute`, name: "NuHepMC.ParticleStatusInfo\[\<ID\>\].Name" 
* type: `HepMC3::StringAttribute`, name: "NuHepMC.ParticleStatusInfo\[\<ID\>\].Description" 

where \<ID\> enumerates all status codes present in "NuHepMC.ParticleStatusIDs". _c.f._ [P.R.1](#pr1-particle-status-codes) for more details.

#### G.R.7 Event Weights

For weights that will be calculated for every event, HepMC3 provides an interface for storing the weight names only once in the `HepMC3::GenRunInfo` instance. At least one event weight, named "CV" must be declared on the `HepMC3::GenRunInfo` instance, and filled for every event. 

This weight can be 1 or constant for every event in a generator run (in the case of an _unweighted_ event vector). This weight must always be included by a consumer when producing correctly-normalized predictions from a NuHepMC file and cannot be assumed to be always 1. The exact form of this weight, whether it is the only information required to properly normalize a prediction, or if additional information is required is an implementation detail. If [G.C.2](#gc2-file-exposure-standalone) and [G.C.4](#gc4-flux-averaged-total-cross-section) are adopted, then the combination of the "CV" weight, the flux-averaged total cross section, and the total file exposure can be unambiguously used to convert an event rate into a cross-section prediction.

This requirement may become more prescriptive in future versions, but it may currently present a barrier to adoption if we require a strict form of the cross-section normalizing factor.

### Conventions

#### G.C.1 Signalling Followed Conventions

To signal to a consumer that an implementation follows a named convention from this specification, a `HepMC3::VectorStringAttribute` should be added to the `HepMC3::GenRunInfo` instance named "NuHepMC.Conventions" containing the names of the conventions adhered to.

#### G.C.2 File Exposure (Standalone)

Each file should contain a description of the exposure of the generator run.

When running a standalone event simulation this will often be the number of events requested, which may differ from the number of events output in some cases where 'blocked' events contribute to the total cross section calculation.

* type: `HepMC3::LongAttribute`, name: "NuHepMC.Exposure.NEvents"

#### G.C.3 File Exposure (Experimental)

Each file should contain a description of the exposure of the generator run.

When simulating with some experimental exposure, often Protons on Target (POT), the exposure should be described. Two attributes are reserved for signalling the exposure contained to consumers. One or both can be provided.

* type: `HepMC3::DoubleAttribute`, name: "NuHepMC.Exposure.POT" 
* type: `HepMC3::DoubleAttribute`, name: "NuHepMC.Exposure.Livetime" 

#### G.C.4 Flux-averaged Total Cross Section

When running a standalone simulation, with a target number of events, additional information about the simulated total cross section is required to properly normalize the output event rate to a cross-section prediction. The quantity required is the flux-averaged total cross section which is calculated from the ratio of the integral of incoming flux times the cross section to the integral of the flux. The flux-averaged total cross section should be stored as a `HepMC3::DoubleAttribute` on the `HepMC3::GenRunInfo` named "NuHepMC.FluxAveragedTotalCrossSection".

#### G.C.5 Process Descriptions

Each "NuHepMC.Process\[\<ID\>\].Description" attribute should contain a reference to the theory paper the process calculation is based on. If you don't cite your contributing theorists, and make it easy for users of your simulation to cite them as well, then don't come crying to us when they cannot find a job to stay in the field.

### Suggestions

#### G.S.1 Run Configuration

It is recommended that a NuHepMC `HepMC3::GenRunInfo` instance contains all information required to reproduce the events in the file. This may be stored in attributes with names beginning with "NuHepMC.Provenance". The information required is generator specific and will neccessarily be stored in a generator specific way, but we suggest a few attributes that would be helpful to downstream users.

* type: `HepMC3::LongAttribute`, name: "NuHepMC.Provenance.NEvents" 
* type: Implementation defined, name: "NuHepMC.Provenance.RNGState"
  * This might be a single seed number used for RNG initialization, or a more complicated description of the initialized RNG state.

## Event Metadata

This section describes RCs for instances of `HepMC3::GenEvent`.

### Requirements

#### E.R.1 Event Number

Each `HepMC3::GenEvent` must have a unique, positivea event number within the file.

#### E.R.2 Process ID

The process ID that produced the `HepMC3::GenEvent` must be recorded in a `HepMC3::IntAttribute` named "ProcID". The metadata for this process ID must already be defined according to [G.R.4](#gr4-process-metadata).

#### E.R.3 Units

Energy and position units must be explicitly set in the `HepMC3::GenEvent`.

#### E.R.4 Lab Position

The position of the event in the lab frame must be added as a `HepMC3::VectorDoubleAttribute`, named "LabPos", with the same units as used when implementing [E.R.3](#er3-cross-section-units).

_c.f._ [E.C.4](#ec4-lab-time) for how to optionally store time in this attribute.

#### E.R.5 Vertices

An event must have at least one `HepMC3::GenVertex`, and must have one and only one with a status of 1, which is considered as the primary process. No `HepMC3::GenVertex` may have a status of 0. _c.f._ [V.R.1](#vr1-vertex-status-codes).

#### E.R.6 Particles

An event must have at least one incoming beam particle. _c.f._ [P.R.1](#pr1-particle-status-codes).

### Conventions

#### E.C.1 Process IDs

It is not possible (or desireable) to mandate a specific set of interaction processes, and assign them IDs in this standard. Different models make different choices and it is not possible to forsee modelling developments that would require new processes or sub-processes to be enumerated in the future. Instead, the below ranges of IDs should be used for high-level categorisation of processes. Even if an implementation uses the convention in the table below, it must still adhere to [G.R.4](#gr4-process-metadata).

| Identifier | Process                      |
| ---------- | -------                      |
| 100-199    | Coherent Nuclear scattering  |
| 200-299    | Quasielastic                 |
| 300-399    | Meson Exchange Current       |
| 400-499    | Resonance production         |
| 500-599    | Shallow inelastic scattering |
| 600-699    | Deep inelastic scattering    |
| 700-999    | Other process types          |

Charged current (CC) processes should have identifiers in the `X00-X49` block and neutral current (NC) in the `X50-X99` block. 

These ranges are subject to change in a future version of this specification.

#### E.C.2 Total Cross Section

The total cross-section for the incoming beam particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named "TotXS". 

#### E.C.3 Process Cross Section

The cross-section for the relevant process ID for the incoming beam particle should be stored in a `HepMC3::DoubleAttribute` on the `HepMC3::GenEvent`, named "ProcXS". 

#### E.C.4 Cross Section Units

Cross sections should be stored in picobarns (1E-38 cm^2 == 1E-2 pb).

#### E.C.5 Lab Time

If the "LabPos" attribute vector contains three entries then it is considered to be just contain the spatial position, if it contains four entries then the fourth entry is considered the time of the event in seconds.

### Suggestions

#### E.S.1 GenCrossSection

It can be useful to include additional information about the generation of a single event and the calculated cross-section. HepMC3 provides the `HepMC3::GenCrossSection` attribute for this purpose.

## Vertex Information

### Requirements

#### V.R.1 Vertex Status Codes

We extend the HepMC3 definition of `HepMC3::GenVertex::status` slightly to include the concept of a primary vertex.

| Status Code | Meaning             | Usage                                           |
| ----------- | -------             | -----                                           |
| 0           | Not defined         | Do not use                                      |
| 1           | Primary Vertex      | The vertex corresponding to the primary process |
| 2-999       | Generator-dependent | For generator usage                             |

Any secondary vertex included within a NuHepMC event may have a status between 2 and 999, where [G.R.5](#gr5-vertex-status-metadata) mandates that all generator-dependent status codes must be fully described by attributes on the `HepMC3::GenRunInfo`. 

## Particle Information

### Requirements

#### P.R.1 Particle Status Codes

We extend the HepMC3 definition of `HepMC3::GenVertex::status` slightly to include the concept of a target particle, which for neutrino scattering will often be a target nucleus.

| Status Code | Description                   | Usage                                                   |
| ----------- | ---------------------------   | --------------                                          |
| 0           | Not defined                   | Not meaningful                                          |
| 1           | Undecayed physical particle   | Recommended for all cases                               |
| 2           | Decayed physical particle     | Recommended for all cases                               |
| 3           | Documentation line            | Often used to indicate in/out particles in hard process |
| 4           | Incoming beam particle        | Recommneded for all cases                               |
| 5-10        | Reserved for future standards | Should not be used                                      |
| 11          | Target particle               | Recommended for all cases                               |
| 12-20       | Reserved for future standards | Should not be used                                      |
| 21-200      | Generator-dependent           | For generator usage                                     |
| 201-        | Simulation dependent          | For simulation software usage                           |

Note especially that any incoming real particle must have a status of 4 or 11, and any outgoing real particle must have a status of 1. This allows consumers to know at-a-glance which simulated particles must be considered 'observable' and which are 'internal' details of the calculation. Special care must be taken when including the effects of initial-state and final-state interactions.

Any internal particle included within a NuHepMC event may have a status greater than 21, where [G.R.6](#gr6-particle-status-metadata) mandates that all generator-dependent status codes must be fully described by attributes on the `HepMC3::GenRunInfo`. 
