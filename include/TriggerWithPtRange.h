#ifndef TriggerWithPtRange_h
#define TriggerWithPtRange_h

class TriggerWithPtRange{

public:

  vector<double> PtValues;
  vector<TString> Triggers;
  vector<double> TriggerSafePtCuts;
  bool IsDATA;
  TString DataStream;

  void Validate();

  TString GetTriggerFromPt(double pt);

  TriggerWithPtRange();
  ~TriggerWithPtRange();

};

#endif


