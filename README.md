HepMC Provides three fundamental types

Event, Vertex, Particle

Events have multiple vertices

vertices have multiple particles which either enter or leave the vertex

particles that only enter a vertex come from the beam
particles that only leave, should be chased

The convention used here is that the 'first' vertex is the 'lab frame' vertex; i.e. target nucleus and neutrino in, all final state particles out.
The 'second' vertex is the hard scatter; i.e. target nucleon and neutrino in, 'primary' particles out.
later vertices can be used for hadronic re-scatters, and particle decays.

N.B. every event is expected to have at least one vertex, most will have two (coherent nuclear scattering events will only contain the lab frame vertex).

Specify/Reserve some vertex status codes, others available for use
 0: Lab frame vertex
 1: ISI vertex
 2: Hard scatter vertex
 3: FSI vertex
 4-9: Reserved for future NuHepMC use
 10+: Available for use

Specify/Reserve some particle status codes, others available for use
 0: Incoming, lab
 1: Generic intermediate (i.e. not visible in the lab)
 2: Outgoing, lab
 3: Re-interacted
 4: Decayed
 5-9: Reserved for future NuHepMC use
 10+: Available for use

It is recommended that generator flags which are a subset of the NuHepMC
definitions should include use the corresponding NuHepMC flag as the
lowest digit, e.g. GENERATORX has 'Decayed intra-nuclear' and
'Decayed extra-nuclear' flags, it is recommended, but not enforced, that these
should be 14 and 24, respectively. If all flags can be used like flag % 10 and
then correctly project onto the NuHepMC enumerations, the GenRunInfo
attribute StatusEnumsVersion=1.0 should be set. Future versions will be updated
here.

Generator specific flags should be defined in the GenRunInfo metadata.
This should be done by adding a int->string map attribute that defines any extra
enum values, one should separately be added for the Vertex and Particle Status.
Helper functions for doing this can be found in NuEvtGenRunInfoHelper.hxx

Units should be specified. MeV, CM
