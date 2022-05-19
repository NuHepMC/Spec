NuHepMC Specification Version 0.1a
----------------------------------

# TODO:
* Add details on the Abstract
* Give more details on HepMC3
* Work on converging with the alternate proposed [specification](./Specification.md).

# Abstract

# Introduction

# Document conventions

# HepMC3

Details of the HepMC3 predefined information can be found [here](https://www.arxiv.org/pdf/1912.08005.pdf).

## Status Codes for particles

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

## Status Codes for vertices

| Status Code | Meaning             | Usage                            |
| ----------- | -------             | -----                            |
| 0           | Not defined         | Vertex with no meaningful status |
| 1-          | Generator-dependent | For generator usage              |


## Predefined Attributes

These are the list of predefined attributes that may be useful for neutrino event generation

### `GenCrossSection`

Contains the following data members:
1. `long int accepted_events`: The number of generated events
2. `long int attempted_events`: The number of attempted events
3. `std::vector<double> cross_sections`: Values of cross-sections
4. `std::vector<double> cross_section_errors`: Values of cross-section uncertainties

Structured as `std::string` as:
`cross_section[0] cross_section_errors[0] accepted_events attempted_events cross_section[1] cross_section_errors[1] ...`

### `signal_process_id`
Definition of the signal process in the event given by an integer value. In the default standard, this is generator dependent.

### `random_states`
An attribute corresponding to the initialization of the random number generator used, can be a series of numbers or a vector. Should be set to the value at the beginning of event simulation (i.e. the seed).

### `weights`


# NuHepMC3 Standard Definition

To expand upon the standard for neutrino event generation, we propose the following extension described within the following sections. 

## NuHepMC3 Header requirements

The header of the NuHepMC3 file has some basic requirements to appropriately setup generator specific components. This can be assigned via the `GenRunInfo` object.

The first required component is a description of the generator used to create the set of events, along with which version of the NuHepMC3 standard the file conforms to.
```
auto run = std::make_shared<GenRunInfo>();
struct GenRunInfo::ToolInfo generator = {name, version, short_description};
run->tools().push_back(generator);
struct GenRunInfo::ToolInfo standard = {"NuHepMC3", version, "The NuHepMC3 standard used here"};
run->tools().push_back(standard);
```

The next component to be defined is the process ids. In the case of the `signal_process_id` attribute, the default HepMC3 standard makes no requirement. Here again, we make no strict requirement other than that a key-value pair mapping is defined into the header using the `GenRunInfo::ToolInfo` attribute. An example of filling into the existing `run` object is:
```
struct GenRunInfo::ToolInfo qe_spectral_function = {qe_spectral_function_code, "Quasielastic Spectral Function", reference}
run -> tools().push_back(qe_spectral_function);
// Additional definitions for each process within the generator
...
```

This will create an entry that gives the status code for the interaction, a short string description of the interaction mode, and a reference to the theory paper the calculation is based on.

While we make no requirements on how the status codes are assigned for each process, we strongly recommend the following convention:

| Status Code | Process                      |
| ----------- | -------                      |
| 100-199     | Coherent                     |
| 200-299     | Quasielastic                 |
| 300-399     | Meson Exchange Current       |
| 400-499     | Resonance production         |
| 500-599     | Shallow inelastic scattering |
| 600-699     | Deep inelastic scattering    |
| 700-999     | Other process types          |

After the generator is defined and the status codes are written out, the final requirement in the header is the information needed to regenerate the events. This can be done by writing out all the details of the run card including the random seed, a reference to the run card (not recommended), or some other method determined by the generator. The only requirement is that the events must be able to be reproduced based on the information in the header. 

All weights available to the generation of the event should be given appropriate names. We do not require any specific names on the weights, but we require a minimum of one weight for the central value predicted. Additional weights may be added to account for shifts in different systematics. Adding a weight definition to the header can be done via:
```
run->set_weight_names({"Default", "Flux_uncertainty1", ...});
```

After filling out the header, the header can be written out using the HepMC3 stream objects (here `file` is one such object):
```
file.set_run_info(run);
file.write_run_info();
```

## NuHepMC3 Event requirements

In order to ensure the interface of generators to other external tools such as geometry and data analysis is uniform, we propose a unified format for the output of the events.

All events should be written with units of MeV for the energy, mm for the positions, and pb for the total cross section. An event with these units, the event number, the run info, the interaction type, and the cross section can be initialized with the following code snippet:
```
GenEvent evt(Units::MEV, Units::MM);
evt.set_run_info(file.run_info());
evt.set_event_number(current_event_idx);
// Interaction type
evt.add_attribute("InteractionType", std::make_shared<IntAttribute>(status_code));
// Cross section
auto cross_section = std::make_shared<GenCrossSection>();
cross_section -> set_cross_section(current_mean, current_error, non_zero_events, current_event_idx);
evt.add_attribute("GenCrossSection", cross_section);
evt.weight("Default") = event_weight;
// Additional lines for other weights
...
```
In the above snippet, the current_event_idx is the number of events attempted so far during generation, non_zero_events is the number of attempted events with non-zero weight. The variable current_mean (current_error) is the current estimate for the total cross section (uncertainty) for the current number of attempted events.

Additionally, a global position attribute should be included in the event to denote the position of the event within the detector. If geometry simulation is not available within the generator, this value should be set to (0, 0, 0) and a geometry driver can update the position to the appropriate location. This attribute can be set using:
```
// Default position here as an example
std::vector<double> position{0,0,0};
evt.add_attribute("GlobalPosition", std::make_shared<VectorDoubleAttribute>(position);
```

An event can be written to file by passing the event to the `write_event` function of the HepMC3 stream object. Details on adding particles and vertices to the event are detailed below.

### Vertex information

An interaction vertex is represented by the `GenVertex` object. The NuHepMC3 standard requires that each vertex is given a position in the nucleus frame, with the origin at the center of the nucleus. This information is only used for reweighting processes such as the cascade and should not be used by the geometry or detector simulation (these should use the "GlobalPosition" position attribute defined at event level. A vertex can be created and added to an event using:
```
HepMC3::FourVector position{x, y, z, t};
GenVertexPtr vertex = std::make_shared<GenVertex>(position);
evt.add_vertex(vertex);
```

Each vertex should have at least one particle coming in and at least one particle coming out. How to define particles and add them to the vertex is detailed in the following section

### Particle information 
Particles can be created using the `GenParticle` object passing in a `HepMC3::FourVector` object, a [PDG PID code](https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf), and a status code as defined below based on the already accepted codes from HepMC3.

| Status | Meaning                                      | Usage                                                   |
| ------ | -------                                      | -----------                                             |
| 0      | Not defined                                  | Not meaningful                                          |
| 1      | Undecayed physical particle                  | Recommended for all cases                               |
| 2      | Decayed physical particle (final state)      | Recommended for all cases                               |
| 3      | Documentation line                           | Often used to indicate in/out particles in hard process |
| 4      | Incoming beam particle (nucleus or neutrino) | Recommended for all cases                               |
| 5      | Cascade particle                             | Recommended for all cases                               |
| 6-10   | Reserved for future standards                | Should not be used                                      |
| 11-200 | Generator-dependent                          | For generator usage                                     |
| 201-   | Simulation dependent                         | For simulation software usage                           |

Particles can be created and added to a vertex using:
```
GenParticlePtr particle_in = std::make_shared<GenParticle>(momentum_in, pid_in, 3);
// Example here is without a cascade
GenParticlePtr particle_out = std::make_shared<GenParticle>(momentum_out, pid_out, 2);
vertex->add_particle_in(particle_in);
vertex->add_particle_out(particle_out);
```

# Example implementations
- The implementation within the Achilles generator can be found in Achilles/HepMC3EventWriter.hh and Achilles/HepMC3EventWriter.cc
