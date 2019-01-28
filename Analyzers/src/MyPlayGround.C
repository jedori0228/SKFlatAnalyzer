#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

}

void MyPlayGround::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void MyPlayGround::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();

  vector<Muon> allmuons = GetMuons("TEST", 20., 2.4);
  vector<Muon> muons = GetMuons("POGTight", 20., 2.4);

  FillHist("PassMETFilter", 0., 1., 1, 0., 1.);

  if(muons.size()!=2) return;

  FillHist("TwoTightMuons", 0., 1., 1, 0., 1.);

  bool FirstTwoPassTight = true;
  for(unsigned int i=0; i<2; i++){

    Muon muon = allmuons.at(i);
    bool PassTight = muon.PassID("POGTight");
    if(!PassTight){
      FirstTwoPassTight = false;
      break;
    }

  }
  if(!FirstTwoPassTight){
    FillHist("FirstTwoFailTight", 0., 1., 1, 0., 1.);
    FillHist("FirstTwoFailTight_ZMass", (allmuons.at(0)+allmuons.at(1)).M(), 1., 500, 0., 500.);
  }
  else{
    FillHist("FirstTwoPassTight", 0., 1., 1, 0., 1.);
    FillHist("FirstTwoPassTight_ZMass", (allmuons.at(0)+allmuons.at(1)).M(), 1., 500, 0., 500.);
  }

}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


