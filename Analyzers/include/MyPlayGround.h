#ifndef MyPlayGround_h
#define MyPlayGround_h

#include "AnalyzerCore.h"

class MyPlayGround : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  MyPlayGround();
  ~MyPlayGround();

};



#endif

