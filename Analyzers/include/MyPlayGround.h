#ifndef MyPlayGround_h
#define MyPlayGround_h

#include "AnalyzerCore.h"
#include "GenFinderForDY.h"
#include "GenFinderForHNWRSignal.h"

class MyPlayGround : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();
  std::vector<Lepton *> TESTFunction(std::vector<Muon>& muons);

  MyPlayGround();
  ~MyPlayGround();

  GenFinderForDY *genFinderDY;

  GenFinderForHNWRSignal *genFinderSig;
  int SignalLeptonChannel;

};



#endif

