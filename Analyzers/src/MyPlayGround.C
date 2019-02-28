#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

}

void MyPlayGround::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void MyPlayGround::executeEventFromParameter(AnalyzerParameter param){

  vector<Jet> jets = GetAllJets();
  double HT(0.);
  for(unsigned int i=0; i<jets.size(); i++){
    HT += jets.at(i).Pt();
  }
  FillHist("HT", HT, 1., 3000, 0., 3000.);

}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


