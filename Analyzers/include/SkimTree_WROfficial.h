#ifndef SkimTree_WROfficial_h
#define SkimTree_WROfficial_h

#include "AnalyzerCore.h"

class SkimTree_WROfficial : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_WROfficial();
  ~SkimTree_WROfficial();

  TTree *newtree;

  void WriteHist();

  LHAPDFHandler LHAPDFHandler_;

};



#endif

