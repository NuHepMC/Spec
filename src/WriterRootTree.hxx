#pragma once

#include "HepMC3/GenRunInfo.h"
#include "HepMC3/WriterRootTree.h"

#include "TFile.h"
#include "TTree.h"

namespace NuHepMC {

class WriterRootTree {
  std::unique_ptr<HepMC3::WriterRootTree> wrtr;

public:
  WriterRootTree(const std::string &filename,
                 std::shared_ptr<HepMC3::GenRunInfo> run =
                     std::shared_ptr<HepMC3::GenRunInfo>()) {
    wrtr = std::make_unique<HepMC3::WriterRootTree>(filename);

    if (run) {
      TFile *f = wrtr->m_tree->GetCurrentFile();
      TTree *grit = new TTree("hepmc3_metatree", "");
      HepMC3::GenRunInfoData *gri_data = nullptr;

      TBranch *grib = grit->Branch("GenRunInfo", &gri_data);
      run->write_data(*gri_data);

      grit->Fill();
      f->WriteTObject(grit);
    }
  }
  WriterRootTree(const std::string &filename, const std::string &treename_evt,
                 const std::string &branchname_evt,
                 const std::string &treename_gri = "",
                 const std::string &branchname_gri = "",
                 std::shared_ptr<HepMC3::GenRunInfo> run =
                     std::shared_ptr<HepMC3::GenRunInfo>()) {

    wrtr = std::make_unique<HepMC3::WriterRootTree>(filename, treename_evt,
                                                    branchname_evt);
    if (run) {
      TFile *f = wrtr->m_tree->GetCurrentFile();
      TTree *grit = new TTree(
          treename_gri.length() ? treename_gri.c_str() : "hepmc3_metatree", "");
      HepMC3::GenRunInfoData *gri_data = nullptr;

      TBranch *grib = grit->Branch(
          branchname_gri.length() ? branchname_gri.c_str() : "GenRunInfo",
          &gri_data);
      run->write_data(*gri_data);

      grit->Fill();
      f->WriteTObject(grit);
    }
  }

  void write_event(const HepMC3::GenEvent &evt) { wrtr->write_event(evt); }

  void write_run_info(){};

  void close() { wrtr->close(); }

  bool failed() { return wrtr->failed(); }
};

} // namespace NuHepMC
