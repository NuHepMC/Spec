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

Specify/Reserve some status, others available for use
-1: Incoming, lab
 0: Generic intermediate (i.e. not visible in the lab)
 1: Outgoing, lab
 2: Re-interacted
 3: Decayed
 4-9: Reserved for future HepMCNuEvtTools use
 10+: Available for use
