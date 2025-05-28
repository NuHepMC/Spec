from pyHepMC3 import HepMC3 as hm
from pyHepMC3 import std as std

from sys import argv

def iattr(attrs, an):
  ia = hm.IntAttribute()
  ia.from_string(attrs[an].value())
  return ia.value()

def sattr(attrs, an):
  sa = hm.StringAttribute()
  sa.from_string(attrs[an].value())
  return sa.value()

def vsattr(attrs, an):
  vsa = hm.VectorStringAttribute()
  vsa.from_string(attrs[an].value())
  return vsa.value()

def get_vin(run_info):

  attrs = run_info.attributes()

  return iattr(attrs,"NuHepMC.Version.Major") * 10000 \
         + iattr(attrs,"NuHepMC.Version.Minor") * 100 \
         + iattr(attrs,"NuHepMC.Version.Patch")

def update_gri_090_100(run_info):

  attrs = run_info.attributes()

  for an in run_info.attribute_names():

    if an == "NuHepMC.Conventions":

      at = vsattr(attrs, an)

      cvs_new = []

      update_map = { "G.C.1": None,
                     "G.C.2": None,
                     "G.C.3": "G.C.1",
                     "G.C.4": None,
                     "G.C.5": "G.C.2",
                     "G.C.6": "G.C.3",
                     "G.C.7": "G.C.4",
                     "G.C.8": "G.C.5" }

      for cv_old in at:

        if cv_old in update_map.keys():
          if update_map[cv_old] is not None:
            cvs_new.append(update_map[cv_old])
        else:
          cvs_new.append(cv_old)

      run_info.add_attribute("NuHepMC.Conventions",
        hm.VectorStringAttribute(std.vector_std_string(cvs_new)))

    if an == "NuHepMC.Units.CrossSection.TargetScale":
      at = sattr(attrs, an)

      update_map = { "PerTarget": "PerAtom",
                     "PerTargetAtom": "PerAtom",
                     "PerTargetNucleon": "PerNucleon",
                     "PerTargetMolecule": None,
                     "PerTargetMolecularNucleon": None }
      if at in update_map.keys():
        if update_map[at] is not None:
          run_info.add_attribute("NuHepMC.Units.CrossSection.TargetScale", 
            hm.StringAttribute(update_map[at]))
        else:
          raise RuntimeError(f"""Invalid G.C.4 TargetScale value encountered: {at}.
  Specify override with -G NuHepMC.Units.CrossSection.TargetScale=<value>.""")

  return run_info

def update_gri(run_info, vin):

  if vin < 10000:
    run_info = update_gri_090_100(run_info)

  return run_info

def update_event_090_100(evt):
  update_map = { "LabPos": "lab_pos",
                 "TotXS": "tot_xs",
                 "ProcXS": "proc_xs" }

  for an in evt.attribute_names():
    if an in update_map.keys():
      evt.add_attribute(update_map[an], hm.StringAttribute(evt.attribute_as_string(an)))
      evt.remove_attribute(an)

  return evt

def update_event(evt, vin):

  if vin < 10000:
    evt = update_event_090_100(evt)
  
  return evt

if __name__ == "__main__":


  print(f"Reading input file: {argv[1]}")
  inp = hm.deduce_reader(argv[1])
  print(f"Opening output file: {argv[2]}")
  out = None

  run_info = None

  vin = 0

  while not inp.failed():
      evt = hm.GenEvent()
      inp.read_event(evt)

      if inp.failed():
          break

      if run_info is None:
        run_info = evt.run_info()
        vin = get_vin(run_info)
        print(f"Upgrading file from NuHepMC v{vin//10000}.{vin//100 % 100}.{vin % 100} to v1.0.0")
        run_info = update_gri(run_info, vin)
        out = hm.WriterAscii(argv[2], run_info)

      evt = update_event(evt, vin)
      evt.set_run_info(run_info)
      out.write_event(evt)