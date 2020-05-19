#include "CompareFullAndFast.h"

void CompareFullAndFast::initializeAnalyzer(){

  TString datapath = getenv("DATA_DIR");

  TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
  TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
  //==== Only for 2016 FastSim, use different one..
  if(DataYear==2016 && IsFastSim){
    PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
  }

  //cout << "[HNWRAnalyzer::initializeAnalyzer()] PUfname = " << PUfname << endl;
  //cout << "[HNWRAnalyzer::initializeAnalyzer()] PUhname = " << PUhname << endl;

  TFile *file_PUReweight = new TFile(PUfname);
  hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);

  genFinderSig = new GenFinderForHNWRSignal();

}

void CompareFullAndFast::executeEvent(){

  //cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
  //cout << "Event started" << endl;

  //==== Event
  Event ev = GetEvent();
  int bin_pu = hist_PUReweight->FindBin(nPileUp);
  double weight_PU = hist_PUReweight->GetBinContent(bin_pu);
  double weight_Prefire = GetPrefireWeight(0);

  vector<Gen> gens = GetGens();
  //cout << "genFinderSig" << endl;
  genFinderSig->Run(gens);
  //cout << "--> done" << endl;
  if(genFinderSig->priLep.IsEmpty()) return;

  //==== weight
  double weight = weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight()*weight_PU*weight_Prefire;

  if(genFinderSig->SignalLeptonChannel==0){
    //cout << "Running Electron" << endl;
    //==== Electron
    TString elID = "HNWRTight";
    if(DataYear>=2017 && IsFastSim) elID = "HNWRTightFastSim";
    if(DataYear==2018){
      elID = "HNWRTight2018Prompt";
      if(IsFastSim) elID = "HNWRTight2018PromptFastSim";
    }

    vector<Electron> electrons = GetElectrons("NOCUT", 35., 2.5);
    bool Found=false;
    Electron el_matcehd;
    double mindR=0.15;
    for(unsigned int i=0; i<electrons.size(); i++){
      if(electrons.at(i).DeltaR(genFinderSig->priLep)<mindR){
        Found = true;
        el_matcehd = electrons.at(i);
        mindR = electrons.at(i).DeltaR(genFinderSig->priLep);
      }
    }

    if(Found){
      TString str_EtaBin = TString::Itoa(el_matcehd.etaRegion(),10);
      FillHist("Electron_Den_Eta"+str_EtaBin+"_Pt", el_matcehd.Pt(), weight, 200, 0., 2000.);
      if(el_matcehd.PassID(elID)){
        FillHist("Electron_PassID_Eta"+str_EtaBin+"_Pt", el_matcehd.Pt(), weight, 200, 0., 2000.);
      }
    }
  }

  if(genFinderSig->SignalLeptonChannel==1){
    //cout << "Running Muon" << endl;
    //==== Muon
    TString muID = "HNWRTight";

    vector<Muon> muons = GetMuons("NOCUT", 50., 2.4);
    bool Found=false;
    Muon mu_matcehd;
    double mindR=0.15;
    for(unsigned int i=0; i<muons.size(); i++){
      if(muons.at(i).DeltaR(genFinderSig->priLep)<mindR){
        Found = true;
        mu_matcehd = muons.at(i);
        mindR = muons.at(i).DeltaR(genFinderSig->priLep);
      }
    }

    TString str_EtaBin = "0";
    if(fabs(mu_matcehd.Eta())<0.9) str_EtaBin = "0";
    else if(fabs(mu_matcehd.Eta())<1.2) str_EtaBin = "1";
    else if(fabs(mu_matcehd.Eta())<2.1) str_EtaBin = "2";
    else str_EtaBin = "3";
    FillHist("Muon_Den_Eta"+str_EtaBin+"_Pt", mu_matcehd.Pt(), weight, 200, 0., 2000.);
    if(mu_matcehd.PassID(muID)){
      FillHist("Muon_PassID_Eta"+str_EtaBin+"_Pt", mu_matcehd.Pt(), weight, 200, 0., 2000.);
    }
  }


}

CompareFullAndFast::CompareFullAndFast(){

}

CompareFullAndFast::~CompareFullAndFast(){

}


