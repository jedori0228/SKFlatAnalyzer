#include "CompareFullAndFast.h"

void CompareFullAndFast::initializeAnalyzer(){

  TString datapath = getenv("DATA_DIR");

  TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
  TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
  //==== Only for 2016 FastSim, use different one..
  if(DataYear==2016 && IsFastSim){
    PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
  }

  cout << "[HNWRAnalyzer::initializeAnalyzer()] PUfname = " << PUfname << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] PUhname = " << PUhname << endl;

  TFile *file_PUReweight = new TFile(PUfname);
  hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);

}

void CompareFullAndFast::executeEvent(){

  //==== Event
  Event ev = GetEvent();
  int bin_pu = hist_PUReweight->FindBin(nPileUp);
  double weight_PU = hist_PUReweight->GetBinContent(bin_pu);

  //==== weight
  double weight = weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight()*weight_PU;

  //==== Electron
  TString elID = "passHEEPID";
  if(DataYear>=2017 && IsFastSim) elID = "HNWRTightFastSim";

  vector<Electron> electrons = GetElectrons("NOCUT", 35., 2.5);
  for(vector<Electron>::const_iterator it=electrons.begin(); it!=electrons.end(); ++it){
    FillHist("Electron_Den_Pt", it->Pt(), weight, 200, 0., 2000.);
    if(it->PassID(elID)){
      FillHist("Electron_PassID_Pt", it->Pt(), weight, 200, 0., 2000.);
    }
  }

}

CompareFullAndFast::CompareFullAndFast(){

}

CompareFullAndFast::~CompareFullAndFast(){

}


