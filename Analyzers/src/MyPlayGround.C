#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

}

void MyPlayGround::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

std::vector<Lepton *> MyPlayGround::TESTFunction(std::vector<Muon>& muons){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<1; i++){
    //cout << "[MyPlayGround::TESTFunction] &(muons.at(i)) = " << &(muons.at(i))<< endl;
    Lepton *l = &(muons.at(i));
/*
    Muon *muon = &(muons.at(i));
    Lepton *l = muon;
*/
    out.push_back(l);
  }
  return out;

}

void MyPlayGround::executeEventFromParameter(AnalyzerParameter param){
/*
  vector<Gen> gens = GetGens();
  Particle GenZ = genFinderDY->Find(gens);
  FillHist("Unweighted_GenZ_Mass" , GenZ.M(), 1., 5000, 0., 5000.);
  FillHist("Unweighted_GenZ_Pt" , GenZ.Pt(), 1., 3000, 0., 3000.);

  Event ev = GetEvent();
  double weight = weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();
  FillHist("Weighted_GenZ_Mass" , GenZ.M(), weight, 5000, 0., 5000.);
  FillHist("Weighted_GenZ_Pt" , GenZ.Pt(), weight, 3000, 0., 3000.);

  int MethodUsed = genFinderDY->MethodUsed;
  TString str_MethodUsed = TString::Itoa(MethodUsed,10);

  FillHist("Method_"+str_MethodUsed+"_Unweighted_GenZ_Mass" , GenZ.M(), 1., 5000, 0., 5000.);
  FillHist("Method_"+str_MethodUsed+"_Unweighted_GenZ_Pt" , GenZ.Pt(), 1., 3000, 0., 3000.);
  FillHist("Method_"+str_MethodUsed+"_Weighted_GenZ_Mass" , GenZ.M(), weight, 5000, 0., 5000.);
  FillHist("Method_"+str_MethodUsed+"_Weighted_GenZ_Pt" , GenZ.Pt(), weight, 3000, 0., 3000.);
*/

  vector<TString> triggers_Default = {
    "HLT_Mu50_v",
    "HLT_TkMu50_v",
    "HLT_Ele27_WPTight_Gsf_v",
    "HLT_Photon175_v",
    "HLT_DoublePhoton60_v",
  };
  vector<TString> triggers_ToAdd = {
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
    "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
  };

  Event ev = GetEvent();
  bool Pass_Default = ev.PassTrigger(triggers_Default);
  bool Pass_ToAdd = ev.PassTrigger(triggers_ToAdd);

  if(Pass_Default){
    FillHist("LRSMHighPt", 0., 1., 1, 0., 1.);
  }
  if(Pass_Default||Pass_ToAdd){
    FillHist("LRSMHighPt_PlusDouble", 0., 1., 1, 0., 1.);
  }


}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


