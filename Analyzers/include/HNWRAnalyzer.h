#ifndef HNWRAnalyzer_h
#define HNWRAnalyzer_h

#include "AnalyzerCore.h"

class HNWRAnalyzer : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNWRAnalyzer();
  ~HNWRAnalyzer();

  bool RunFake, RunCF;
  bool RunSyst;
  bool PromptLeptonOnly;
  bool NoTrigger;

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

  //==== My tool
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

};



#endif

