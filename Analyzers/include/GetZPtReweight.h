#ifndef GetZPtReweight_h
#define GetZPtReweight_h

#include "AnalyzerCore.h"
#include "GenFinderForDY.h"
#include "GenFinderForHNWRSignal.h"

class GetZPtReweight : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();

  bool DoLowPt;

  GetZPtReweight();
  ~GetZPtReweight();

  GenFinderForDY *genFinderDY;

};



#endif

