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

}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


