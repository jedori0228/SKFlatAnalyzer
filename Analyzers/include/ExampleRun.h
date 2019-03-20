#ifndef ExampleRun_h
#define ExampleRun_h

#include "AnalyzerCore.h"

class ExampleRun : public AnalyzerCore {

public:

  void initializeAnalyzer();

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool RunSyst;
  bool RunNewPDF;
  bool RunXSecSyst;
  bool MakeNewTree;

  TString IsoMuTriggerName;
  double TriggerSafePtCut;

  vector<TString> MuonIDs, MuonIDSFKeys;
  vector<Muon> AllMuons;
  vector<Jet> AllJets;

  double weight_Prefire;

  ExampleRun();
  ~ExampleRun();

  //==== My tree variables;
  double treevar_mll;

};



#endif

