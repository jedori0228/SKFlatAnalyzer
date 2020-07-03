#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

  genFinderSig = new GenFinderForHNWRSignal();
  SignalLeptonChannel = -1;

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

	int genNpid = -1;
	int SignalLeptonChannel = -1;
  Gen Gen_N;
	for(unsigned int i=2; i<gens.size(); i++){
		int pid = abs( gens.at(i).PID() );
		if( pid==9900012 || pid==9900014 ){
			genNpid = pid;
      Gen_N = gens.at(i);
			break;
		}
	}
	//==== 0 = electron
	//==== 1 = muon
  TString ChName = "";
	if(genNpid==9900012){
		SignalLeptonChannel = 0;
    ChName = "Electron";
	}
	else if(genNpid==9900014){
		SignalLeptonChannel = 1;
    ChName = "Muon";
	}
	else{
	}

	if(SignalLeptonChannel<0) return;

  vector<FatJet> fatjets = GetFatJets("HN", 200, 2.4);
  FatJet MatchedJet;
  bool IsFatJetMatched = false;
  for(unsigned int i=0; i<fatjets.size(); i++){
    if( fatjets.at(i).DeltaR( Gen_N ) < 0.8 ){
      IsFatJetMatched = true;
      MatchedJet = fatjets.at(i);
      break;
    }
  }
  if(IsFatJetMatched){

    FillHist(ChName+"_MatchedFatJetFound", 0., 1., 1, 0., 1.);

    FillHist(ChName+"_Gen_N_Pt", Gen_N.Pt(), 1., 3000, 0., 3000.);
    FillHist(ChName+"_Gen_N_M", Gen_N.M(), 1., 1000, 0., 1000.);

    FillHist(ChName+"_MatchedJet_Pt", MatchedJet.Pt(), 1., 3000, 0., 3000.);
    FillHist(ChName+"_MatchedJet_M", MatchedJet.M(), 1., 1000, 0., 1000.);


    //==== check lepton
    if(SignalLeptonChannel==0){
      //vector<Electron> electrons = GetElectrons("HNWRLoose", 53., 2.4);
      vector<Electron> electrons = GetElectrons("NOCUT", 53., 2.4);
      for(unsigned int j=0; j<electrons.size(); j++){
        if( electrons.at(j).DeltaR( MatchedJet ) < 0.8){
          FillHist(ChName+"_LeptonFoundInsideJet", 0., 1., 1, 0., 1.);
          FillHist(ChName+"_LeptonFoundInsideJet_MatchedJet_Pt", MatchedJet.Pt(), 1., 3000, 0., 3000.);
          break;
        }
      }
    }
    else{
      vector<Muon> muons = GetMuons("HNWRLoose", 53., 2.4);
      for(unsigned int j=0; j<muons.size(); j++){
        if( muons.at(j).DeltaR( MatchedJet ) < 0.8){
          FillHist(ChName+"_LeptonFoundInsideJet", 0., 1., 1, 0., 1.);
          FillHist(ChName+"_LeptonFoundInsideJet_MatchedJet_Pt", MatchedJet.Pt(), 1., 3000, 0., 3000.);
          break;
        }
      }
    }



  }

}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


