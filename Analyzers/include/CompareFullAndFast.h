#ifndef CompareFullAndFast_h
#define CompareFullAndFast_h

#include "AnalyzerCore.h"

class CompareFullAndFast : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();

  CompareFullAndFast();
  ~CompareFullAndFast();

  TH1D *hist_PUReweight;

};



#endif

