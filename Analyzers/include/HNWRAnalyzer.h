#ifndef HNWRAnalyzer_h
#define HNWRAnalyzer_h

#include "AnalyzerCore.h"
#include "GenFinderForDY.h"
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
  bool CalculateAverageKFactor;
  bool HEM1516;
  bool BeforeRun319077;
  bool UseJetPtRwg;
  bool UseDYCR1Reshape;

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
  GenFinderForDY *genFinderDY;
  bool ApplyDYPtReweight;
  TH1D *hist_DYPt_PDFError;
  TH1D *hist_DYPt_ScaleUp;
  TH1D *hist_DYPt_ScaleDown;
  TH1D *hist_DYPt_PDFAlphaSUp;
  TH1D *hist_DYPt_PDFAlphaSDown;
  Particle GenZParticle;
  double ZPtReweight, ZPtReweight_Up, ZPtReweight_Down;
  double ZPtReweight_QCDScaleUp, ZPtReweight_QCDScaleDown;
  double ZPtReweight_QCDPDFErrorUp, ZPtReweight_QCDPDFErrorDown;
  double ZPtReweight_QCDPDFAlphaSUp, ZPtReweight_QCDPDFAlphaSDown;

  //==== DYPt EW corr
  TH1D *hist_DYPtEWCorr;
  TH1D *hist_DYPtEWCorrE1;
  TH1D *hist_DYPtEWCorrE2;
  TH1D *hist_DYPtEWCorrE3;
  double ZPtEWCorr;
  double ZPtEWCorr_E1Up, ZPtEWCorr_E1Down;
  double ZPtEWCorr_E2Up, ZPtEWCorr_E2Down;
  double ZPtEWCorr_E3Up, ZPtEWCorr_E3Down;

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

  //==== k-factor
  TH1D *hist_kfactor;
  int mWR, mN;
  double mlN;
  double this_avg_kfactor;
  double this_kfactor;
  double GetHNWRKFactor(double mlN);

};



#endif

