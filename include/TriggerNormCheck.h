#ifndef TriggerNormCheck_h
#define TriggerNormCheck_h

#include "AnalyzerCore.C"
#include "TriggerWithPtRange.C"

class TriggerNormCheck : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  TriggerWithPtRange Muon_TrigWithPtRange, Electron_TrigWithPtRange;
  vector<double> Electron_FakeRatePtBinnings, Muon_FakeRatePtBinnings;

  TriggerNormCheck();
  ~TriggerNormCheck();

};



#endif

