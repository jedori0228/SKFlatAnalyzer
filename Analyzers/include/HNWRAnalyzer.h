#ifndef HNWRAnalyzer_h
#define HNWRAnalyzer_h

#include "AnalyzerCore.h"
#include "GenFinderForHNWRSignal.h"

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
  bool RunXsecSyst;
  bool Signal;
  bool SignalElectronOnly, SignalMuonOnly;
  bool HEM1516;
  bool BeforeRun319077;

  vector<Gen> gens;

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire, weight_Prefire_Up, weight_Prefire_Down;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;

  //==== DYPtReweight
  bool ApplyDYPtReweight;
  double ZPtReweight, ZPtReweight_Up, ZPtReweight_Down;

  //==== My tool
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

  GenFinderForHNWRSignal *genFinderSig;
  int SignalLeptonChannel;

  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;

  //==== DY reshape
  TH1D *hist_DYReshape_Resolved_ratio_AllCh;
  TH1D *hist_DYReshape_Resolved_EEOnlyRatio;
  TH1D *hist_DYReshape_Resolved_MuMuOnlyRatio;

  TH1D *hist_DYReshape_Boosted_ratio_AllCh;
  TH1D *hist_DYReshape_Boosted_EEOnlyRatio;
  TH1D *hist_DYReshape_Boosted_MuMuOnlyRatio;

  double GetDYReshape(double mwr, TString region, int SystType);
  bool ApplyDYReshape;

  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);

  //==== LSF SF
  double LSFSF(int lepflav, int dir);

};



#endif

