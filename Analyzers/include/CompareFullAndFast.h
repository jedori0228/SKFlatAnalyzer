#ifndef CompareFullAndFast_h
#define CompareFullAndFast_h

#include "AnalyzerCore.h"
#include "GenFinderForHNWRSignal.h"

class CompareFullAndFast : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();

  CompareFullAndFast();
  ~CompareFullAndFast();

  TH1D *hist_PUReweight;

  GenFinderForHNWRSignal *genFinderSig;

};



#endif

