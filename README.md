NuHepMC Specification Version 0.1a
----------------------------------

# TODO:
* Add details on the Abstract
* Give more details on HepMC3

# Abstract

In this specification we present an additional set of requirements, conventions, and suggestions for simulated interactions stored in [HepMC3](https://www.arxiv.org/pdf/1912.08005.pdf) format. These are designed to be used in specific context of neutrino event generators and associated simulation and analysis toolchains, and will lower the barrier for working with the output of a range of interaction simulations.

# Introduction

Until now, each event generator, experimental simulation, and analysis framework maintains their own proprietary set of interfaces and format converters. By developing an extensible and extendable set of conventions on top of the versatile and mature HepMC3 we aim to cater to analysers and simulation framework developers who want to handle interactions in a tool-agnostic way, while also not putting any limits on the information that individual event generators want to write out--enabling lossless, bidirectional format conversion with existing proprietary formats.

The specification is broken down into Requirements, Conventions, and Suggestions (RCs) for each of the following HepMC3 components.

* G: Generator Run Metadata
* E: Event Metadata
* V: Vertex Information
* P: Particle Information

We also provide a reference implementation in C++ and specification validator programme in this repository.

# NuHepMC

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

#### G.R.3 Generator Provenance
A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::GenRunInfo::ToolInfo` for each 'tool' involved in the production of the file thus far. The `ToolInfo` instance must contain non-empty name and version fields.

#### G.R.4 Process Metadata
A NuHepMC `HepMC3::GenRunInfo` instance must contain a `HepMC3::VectorIntAttribute` named "NuHepMC.ProcessIDs" listing all physics process IDs as integers.

The `HepMC3::GenRunInfo` instance must also contain two other attributes giving a name and description of the proceses

* type: `HepMC3::StringAttribute`, name: "NuHepMC.Process\[\<ID\>\].Name" 
* type: `HepMC3::StringAttribute`, name: "NuHepMC.Process\[\<ID\>\].Description" 

where \<ID\> enumerates all process IDs present in "NuHepMC.ProcessIDs".

#### G.R.5 Event Weights

For weights that will be calculated for every event, HepMC3 provides an interface for storing the weight names only once in the `HepMC3::GenRunInfo` instance. This specification requires that at least one event weight, named "CV" is included on the `HepMC3::GenRunInfo` instance, and filled for every event.

### Conventions

Following named conventions is optional. If conventions prove useful and are considered stable, they may become requirements in future standards.

#### G.C.1 Signalling Followed Conventions

To signal to a consumer that an implementation follows a named convention, add a `HepMC3::VectorStringAttribute` to the `HepMC3::GenRunInfo` instance named "NuHepMC.Conventions" containing the names of the conventions adhered to.

#### G.C.2 Process Descriptions

Each "NuHepMC.Process\[\<ID\>\].Description" attribute contains a reference to the theory paper the process calculation is based on. If you don't cite your contributing theorists, and make it easy for users of your simulation to cite them as well, then don't come crying to us when they cannot find a job to stay in the field.

#### G.C.3 Process IDs

It is not possible (or desireable) to mandate a specific set of interaction processes, and assign them IDs in this standard. Different models make different choices and it is not possible to forsee modelling developments that would require new processes or sub-processes to be enumerated in the future. Instead, we recommend ranges of IDs to use for high-level categories of processes that can be used to guide ID selection. Even if an implementation uses the convention in the table below, it must still adhere to G.R.4 and fully describe its process list in standardised attributes on the `HepMC3::GenRunInfo` instance.

| Status Code | Process                      |
| ----------- | -------                      |
| 100-199     | Coherent Nuclear scattering  |
| 200-299     | Quasielastic                 |
| 300-399     | Meson Exchange Current       |
| 400-499     | Resonance production         |
| 500-599     | Shallow inelastic scattering |
| 600-699     | Deep inelastic scattering    |
| 700-999     | Other process types          |

These ranges are subject to change in a future version of this specification.

### Suggestions

#### G.S.1 Run Configuration

A NuHepMC `HepMC3::GenRunInfo` instance should contain all information required to reproduce the events in the file. This should be stored in attributes with names beginning with "NuHepMC.Provenance". The specific information required is generator specific and will neccessarily be stored in a generator specific way, but we suggest a few attributes that would be helpful to downstream users.

* type: `HepMC3::IntAttribute`, name: "NuHepMC.Provenance.NEvents" 
* type: Implementation defined, name: "NuHepMC.Provenance.RNGState"
  * This might be a single seed number used for RNG initialization, or a more complicated description of the initialized RNG state.

#### G.S.2 GenCrossSection

It can be useful to include additional information about the generation of an event and the calculated cross-section. HepMC3 provides the `HepMC3::GenCrossSection` attribute for this purpose.

## Event Metadata

This section describes RCs for instances of `HepMC3::GenEvent`.

### Requirements

#### E.R.1 Event Number

Each `HepMC3::GenEvent` must have a unique event number within the file.

#### E.R.2 Process ID

The process ID that produced the `HepMC3::GenEvent` should be recorded in a `HepMC3::IntAttribute` named "ProcID". The metadata for this process ID must already be defined according to G.R.4.

#### E.R.3 Units

Energy and position units should be explicitly set in the `HepMC3::GenEvent`.

#### E.R.4 Lab Position

The position of the event in the lab frame should be added as a `HepMC3::VectorDoubleAttribute`, named "LabPos", with the same units as used when implementing E.R.3.

c.f. E.C.2 for how to store time in this vector

### Conventions

#### E.C.1 Cross-section Weight Units

The "CV" event weight should be used to store the cross-section weight in picobarns. For reference: 1E-38 cm^2 == 1E-2 pb.

#### E.C.2 Lab Time

If the "LabPos" attribute vector contains three entries then it is considered to be just the spatial position, if it contains four then the fourth entry is considered the time of the event in seconds.

## Vertex Information

### Requirements

#### V.R.1

### Conventions

#### V.C.1

| Status Code | Meaning             | Usage                            |
| ----------- | -------             | -----                            |
| 0           | Not defined         | Vertex with no meaningful status |
| 1-          | Generator-dependent | For generator usage              |

## Particle Information

### Requirements

#### P.R.1

### Conventions

#### P.C.1

| Status Code | Description                   | Usage                                                   |
| ----------- | ---------------------------   | --------------                                          |
| 0           | Not defined                   | Not meaningful                                          |
| 1           | Undecayed physical particle   | Recommended for all cases                               |
| 2           | Decayed physical particle     | Recommended for all cases                               |
| 3           | Documentation line            | Often used to indicate in/out particles in hard process |
| 4           | Incoming beam particle        | Recommneded for all cases                               |
| 5-10        | Reserved for future standards | Should not be used                                      |
| 11-200      | Generator-dependent           | For generator usage                                     |
| 201-        | Simulation dependent          | For simulation software usage                           |

