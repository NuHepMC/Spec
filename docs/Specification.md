NuHepMC Specification Version 0.1
---------------------------------

# Todo

* kLabFrame may not be the best name if there must be one yet other vertices defined within the lab frame are allowed, maybe change to kNuclearTargetVertex.

* Expunge any reference to c++ objects/methods defined in this package and define the specification only in terms of HepMC3 objects.

# Abstract

# Introduction

# Document conventions

# HepMC3

The HepMC3 C++ object model is used for Events, Vertices, Particles, and
meta-data (Attributes). The NuHepMC standard is built on top of the HepMC3
standard as an extra set of conventions and meta-data definitions. This
choice was made to take advantage of the robust, and well designed HepMC
format, I/O routines, and C++ object model, and allow this specification
to focus on  standardizing what is specifically needed for the
neutrino-scattering MC community.

The NuHepMC specification can be adhered to without the use of the
helper API defined in this package, as it only uses HepMC3-native
objects. The helper API that can be used by producers and consumers
of adherent event files is defined in a separate document.

# Objects

## Attributes

The HepMC3 format defines an interface for storing/retrieving arbitrary
meta-data. The arbitrary data is always stored as character string, and (de)serialization routines must be written to add/retrieve

The NuHepMC specification defines these extra Attribute subclasses that may be helpful in adhering to the specification on Generator Run Information and Events.

### NuHepMC Attributes

## Generator Run Information

The HepMC3::GenRunInfo object is designed to be used to store *Run* level information about a set of events.

### NuHepMC Requirements

#### NuHepMC Version

The specification version MUST be included via a `HepMC3::StringAttribute` named `NuHepMCV`

#### Particle status codes

Helper methods exist to write/read the standard-defined modes and add extra generator-specific modes

#### Vertex status codes

Helper methods exist to write/read the standard-defined modes and add extra generator-specific modes

### NuHepMC Optional

#### Hard scattering modes

Helper methods exist to help write/read these generator-specific interaction channel definitions. While their inclusion is not enforced by this standard it is very strongly recommended for user comprehension and ease of transition from existing formats.

## Particles

`HepMC3::GenParticle` have a four-momentum, a *pid* integer, and a status integer. They do not have a position, but MUST always be associated to at least one `HepMC3::GenVertex` as an 'incoming' or 'out-going' particle.

This standard enforces that all *pid* integers MUST conform to the PDG MC particle ID code standard. N.B. that depending on the context, a free proton can be either 2212 or 1000010000, and both are used by this standard.

`HepMC3::GenParticle` status integers are defined by this standard as so:
  0:      InitialState
  1:      Intermediate
  2:      FinalState
  3:      Reinteracted
  4:      Decayed
  5-9:    Reserved for future standard use
  10-998: Available
  999:    Other

*N.B.* In certain contexts, some particles MUST have a specific status, this is related to their associated vertex and is described in the *Events* section.

The standardized integers are defined by the class enumeration `NuHepMC::labels::ParticleState`.

Generally `HepMC3::GenParticle` with status codes InitialState and FinalState are those that will be consumed by downstream simulation or analysis software, but there are many cases where the other standard codes and extra, generator-specific codes will be useful.

It is not enforced, but is highly recommended (and expected by certain helper methods) that generator-specific specializations of these status' end in the same number used by the standard version; e.g. 13: Reinteracted and was absorbed. This is so that status % 10 is collasible onto helpful subsets of the definitions for downstream consumers that do not need such specific details about particle fate.

## Vertices

Each `HepMC3::GenVertex` has, among other attributes not governed by this standard, a vector of 'incoming' and 'out-going' particles, a four-position, and a status code.

Very similar rules about the definition, use, and extensibility of the particle status codes apply to the vertex status code definition, `NuHepMC::labels::VertexState`.
`HepMC3::GenVertex` status integers are defined by this standard as so:
  0:      LabFrame
  1:      ISInteraction
  2:      HardScatter
  3:      FSInteraction
  4:      Decayed
  5-9:    Reserved for future standard use
  10-998: Available
  999:    Other

The semantic meaning and usage constraints for specific status codes are described in the following section that deals with the structure of a full `HepMC3::GenEvent`

## Events

The `HepMC3::GenEvent` object can contain an arbitrary number of vertices and particles, as well an arbitrary number of arbitrary `HepMC3::Attribute`s.

### Required `GenVertex`s

* Each `HepMC3::GenEvent` MUST contain at least one `HepMC3::GenVertex`.
* It MUST contain EXACTLY one vertex with a status of `NuHepMC::labels::VertexState::kLabFrame`
  * This `HepMC3::GenVertex` MUST have two 'incoming' `HepMC3::Genparticle`s and both MUST have status `NuHepMC::labels::ParticleState::kInitialState`.
    * One 'incoming' particle MUST have a single-particle PDG MC pid code (e.g. 14, 11, 211) and is considered the 'probe'.
    * One 'incoming' particle MUST have a nuclear PDG MC code (100ZZZAAA0) and is considered the 'target'. *N.B.* For free nucleon targets, the nuclear PDG code MUST still be used (H/p: 1000010000, n: 1000000010).
  * `HepMC3::Genparticle`s 'out-going' from this vertex are to be considered physically trackable outside of the nuclear target of the reaction. Generally they will have a status of `NuHepMC::labels::ParticleState::kFinalState`, but see the sub bullet for a possible exception.
    * *N.B.* This does not preclude the generator decaying particles outside of the target nucleus. If this needs to be specified, the 'out-going' particles from the `kLabFrame` vertex, should have a relevant status, and be 'in-going' particles to another vertex that represents the decay, however, this vertex should have a generator-specific vertex.

### Optional `GenVertex`s

* Each `HepMC3::GenEvent` MAY contain at most one `HepMC3::GenVertex` with a status of `NuHepMC::labels::kHardScatter`
  * This `HepMC3::GenVertex` represents the 'primary' probe--target reaction (e.g. muon neutrino + neutron -> muon + proton).
  * If `HepMC3::GenParticle`s are logically the same particles as those associated with the required `NuHepMC::labels::VertexState::kLabFrame` `HepMC3::GenVertex`, they MUST be deep copies, rather than references added via calls to `HepMC3::GenVertex::add_particle_in` for both `HepMC3::GenVertex`s.
    * This can be achieved with the `HepMC3::GenParticle(HepMC3::GenParticleData const&)` constructor. This is to preclude any confusion with the same particle instance being an 'out-going' particle from multiple vertex objects.
  * The inclusion of this specified vertex is optional to allow the consistent inclusion of coherent neutrino--nucleus, and neutrino--electron elastic scattering which occur outside the nuclear environment. For the majority of probe--nucleus interaction channels it is very strongly recommended.
* Each `HepMC3::GenEvent` MAY contain any number of `HepMC3::GenVertex` with a status of `NuHepMC::labels::kISInteraction` and `NuHepMC::labels::kFSInteraction`.
  * Particles 'incoming' or 'out-going' from these vertices MUST be connected to the `NuHepMC::labels::kHardScatter` `HepMC3::GenEvent` to form a single reaction tree that includes the 'hard scatter' and all relevant 'ISIs' and 'FSIs'.
* Each `HepMC3::GenEvent` MAY contain any number of `HepMC3::GenVertex` with generation-defined status modes (10+).
  * If these vertices represent generator-specific versions of the specification defined status'; i.e. a generator wants to specify specific FSI modes, these these vertices MUST adhere to the specification for `NuHepMC::labels::kFSInteraction`s.
  * It is strongly recommended, but not required, that generator-specific status' that are specializations of those defined in this standard end in the same number as used in this standard; i.e. An Absorption FSI vertex may use the status 10 + `NuHepMC::labels::kFSInteraction`.

### Optional Attributes

* It is strongly recommended but not required, that each Each `HepMC3::GenEvent` contains a `HepMC3::IntAttribute` named "HSMode" (for Hard Scatter Mode), that contains a generator-specific integer flag representing the hard-scattering mode.
  * Nothing is specified about the mapping of these flags to physics, but the definition of any used MUST be defined in the `HepMC3::GenRunInfo`.
  * If this attribute does not exist, the value will be considered 0.
  * Helper methods of writing and reading this attribute can be found in XXX.

# Extensibility

# Glossary of Terms

*Run:*

*ISI:*

*FSI:*

*generator-specific:*
