#ifndef HNWROnZ_h
#define HNWROnZ_h

#include "AnalyzerCore.h"
#include "GenFinderForDY.h"

class HNWROnZ : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNWROnZ();
  ~HNWROnZ();

  bool RunFake, RunCF;
  bool RunSyst;
  bool PromptLeptonOnly;
  bool NoTrigger;
  bool FindGenZ;

  vector<Gen> gens;

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;

  //==== DYPtReweight
  bool ApplyDYPtReweight;
  TH1D *hist_DYPtReweight_Electron;
  TH1D *hist_DYPtReweight_Muon;

  double GetDYPtReweight(double zpt, int flav);

  double PUReweight;
  int N_VTX;

  GenFinderForDY *genDY;
  TTree *tree_DY;
  int treevar_lepch;
  double treevar_RECO_Mass;
  double treevar_RECO_Pt;
  double treevar_GEN_Mass;
  double treevar_GEN_Pt;

  void WriteHist();

};



#endif

