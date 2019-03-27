#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

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

  vector<Muon> muons = GetAllMuons();
  if(muons.size()==0) return;

  cout << "===============================================================" << endl;

  Muon& muon = muons.at(0);
  Lepton *lepton = &muon;

  cout << "#### First element of muon vector ####" << endl;
  cout << "address = " << &( muons.at(0) ) << endl;
  cout << "#### Muon ####" << endl;
  cout << "address  = " << &muon << endl;
  cout << "Chi2 = " << muon.Chi2() << endl;
  cout << "#### Lepton ####" << endl;
  cout << "address = " << lepton << endl;
  Muon *mu_from_lep = (Muon *)lepton;
  cout << "Chi2 from lepton = " << mu_from_lep->Chi2() << endl;
  vector<Lepton *> vectest_lepton = TESTFunction(muons);
  cout << "#### Lepton from function ####" << endl;
  cout << "address = " << vectest_lepton.at(0) << endl;
  Muon *mu_from_lep_from_vec = (Muon *)(vectest_lepton.at(0));
  cout << "Chi2 from lepton = " << mu_from_lep_from_vec->Chi2() << endl;
}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


