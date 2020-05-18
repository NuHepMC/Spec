#pragma once

#include "HepMC3/GenRunInfo.h"
#include "HepMC3/Print.h"

#define private public
#include "HepMC3/ReaderRootTree.h"
#undef private

#include "TFile.h"
#include "TTree.h"

namespace NuHepMC {

  using GenEvent = HepMC3::GenEvent;

class ReaderRootTree {
  std::unique_ptr<HepMC3::ReaderRootTree> rdr;
  std::shared_ptr<HepMC3::GenRunInfo> run;

public:
  ReaderRootTree(const std::string &filename) : run(nullptr) {

    rdr = std::make_unique<HepMC3::ReaderRootTree>(filename);

    TFile *f = rdr->m_tree->GetCurrentFile();
    TTree *grit;
    f->GetObject("hepmc3_metatree", grit);

    if (grit && grit->GetEntries()) {
      run = std::make_shared<HepMC3::GenRunInfo>();
      HepMC3::GenRunInfoData *gri_data = nullptr;

      grit->SetBranchAddress("GenRunInfo", &gri_data);
      grit->GetEntry(0);
      run->read_data(*gri_data);
    }
  }
  ReaderRootTree(const std::string &filename, const std::string &treename_evt,
                 const std::string &branchname_evt,
                 const std::string &treename_gri = "",
                 const std::string &branchname_gri = "")
      : run(nullptr) {

    rdr = std::make_unique<HepMC3::ReaderRootTree>(filename, treename_evt,
                                                   branchname_evt);
    TFile *f = rdr->m_tree->GetCurrentFile();
    TTree *grit;
    f->GetObject(
        treename_gri.length() ? treename_gri.c_str() : "hepmc3_metatree", grit);

    if (grit && grit->GetEntries()) {
      run = std::make_shared<HepMC3::GenRunInfo>();
      HepMC3::GenRunInfoData *gri_data = nullptr;

      grit->SetBranchAddress(branchname_gri.length() ? branchname_gri.c_str()
                                                     : "GenRunInfo",
                             &gri_data);
      grit->GetEntry(0);
      run->read_data(*gri_data);
    }
  }

  bool skip(const int i) {
    return rdr->skip(i);
  }

  bool read_event(HepMC3::GenEvent &evt) {
    bool rtn = rdr->read_event(evt);
    evt.set_run_info(run);
    return rtn;
  }

  UInt_t get_entries() { return rdr->m_tree->GetEntries(); }

  std::shared_ptr<HepMC3::GenRunInfo> run_info() { return run; }

  void close() { rdr->close(); }

  bool failed() { return rdr->failed(); }
};

} // namespace NuHepMC
