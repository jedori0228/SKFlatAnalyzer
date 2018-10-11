#ifndef HNWRAnalyzer_h
#define HNWRAnalyzer_h

#include "AnalyzerCore.C"

class HNWRAnalyzer : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNWRAnalyzer();
  ~HNWRAnalyzer();

  bool RunFake, RunCF;
  bool PromptLeptonOnly;

  vector<Gen> gens;

};



#endif
