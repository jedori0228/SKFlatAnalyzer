#ifndef HNWRSignalStudy_h
#define HNWRSignalStudy_h

#include "AnalyzerCore.h"
#include "GenFinderForHNWRSignal.h"

class HNWRSignalStudy : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool RunNewPDF;

  GenFinderForHNWRSignal *genFinderSig;

  HNWRSignalStudy();
  ~HNWRSignalStudy();

};



#endif

