#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

  genFinderSig = new GenFinderForHNWRSignal();
  SignalLeptonChannel = -1;

  ApplyDYPtReweight = HasFlag("ApplyDYPtReweight");
  if(ApplyDYPtReweight){
    TString datapath = getenv("DATA_DIR");

    TFile *file_DYPtReweight = new TFile(datapath+"/"+TString::Itoa(DataYear,10)+"/HNWRDYPtReweight/Ratio.root");
    hist_DYPtReweight = (TH2D *)file_DYPtReweight->Get("Ratio");
  }

}

void MyPlayGround::executeEvent(){


  AnalyzerParameter param;
  param.Name = "Trigger";
  executeEventFromParameter(param);

  param.Name = "Lepton200GeV";
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

  vector<TString> Triggers_Muon;

  if(DataYear==2016){

    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
    };

  }
  else if(DataYear==2017){

    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };

  }
  else if(DataYear==2018){

    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };

  }

  Event ev = GetEvent();
  if( ! (ev.PassTrigger(Triggers_Muon)) ) return;

  vector<Muon> muons = GetMuons("POGTightWithTightIso", 20., 2.4);
  if(muons.size()!=2) return;
  if(muons.at(0).Pt()<53) return;
  if( !IsOnZ( (muons.at(0)+muons.at(1)).M(), 15 ) ) return;

  if(param.Name=="Lepton200GeV"){
    if(muons.at(0).Pt()<200.) return;
  }

  vector<FatJet> fatjets_LepVeto = GetFatJets("tightLepVeto", 200., 2.4);
  vector<FatJet> fatjets_HN = GetFatJets("HN", 200., 2.4);
  vector<FatJet> fatjets = GetFatJets("tight", 200., 2.4);
  vector<Jet> jets_LepVeto = GetJets("tightLepVeto", 200., 2.4);
  vector<Jet> jets = GetJets("tight", 200., 2.4);

  TString this_region = param.Name;
  double weight = ev.MCweight() * weight_norm_1invpb * ev.GetTriggerLumi("Full");
  if(IsDATA) weight = 1.;

  //==== zpt reweight
  if(ApplyDYPtReweight){

    vector<Gen> gens = GetGens();
    genFinderDY->Find(gens);
    Particle GenZParticle = genFinderDY->GenZ;

    double mZ = GenZParticle.M();
    double ptZ = GenZParticle.Pt();

    if(mZ<50.) mZ=51.;
    if(mZ>=1000.) mZ=999.;
    //if(ptZ<70.) ptZ=71.;
    if(ptZ>=1000.) ptZ=999.;

    int bin_mass = hist_DYPtReweight->GetXaxis()->FindBin(mZ);
    int bin_pt   = hist_DYPtReweight->GetYaxis()->FindBin(ptZ);

    double value = hist_DYPtReweight->GetBinContent( bin_mass, bin_pt );

    weight *= value;

  }


  for(unsigned int i=0; i<fatjets.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_"+this_region, fatjets.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Phi_"+this_region, fatjets.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_vs_Phi_"+this_region, fatjets.at(i).Eta(), fatjets.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_SDMass_"+this_region, fatjets.at(i).SDMass(), weight, 500, 0., 500.);

    FillHist(this_region+"/FatJet_Eta_"+this_region, fatjets.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/FatJet_Phi_"+this_region, fatjets.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/FatJet_Eta_vs_Phi_"+this_region, fatjets.at(i).Eta(), fatjets.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/FatJet_SDMass_"+this_region, fatjets.at(i).SDMass(), weight, 500, 0., 500.);

  }

  for(unsigned int i=0; i<fatjets_LepVeto.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/LepVetoFatJet_"+this_itoa+"_Eta_"+this_region, fatjets_LepVeto.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/LepVetoFatJet_"+this_itoa+"_Phi_"+this_region, fatjets_LepVeto.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/LepVetoFatJet_"+this_itoa+"_Eta_vs_Phi_"+this_region, fatjets_LepVeto.at(i).Eta(), fatjets_LepVeto.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/LepVetoFatJet_"+this_itoa+"_SDMass_"+this_region, fatjets_LepVeto.at(i).SDMass(), weight, 500, 0., 500.);

    FillHist(this_region+"/LepVetoFatJet_Eta_"+this_region, fatjets_LepVeto.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/LepVetoFatJet_Phi_"+this_region, fatjets_LepVeto.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/LepVetoFatJet_Eta_vs_Phi_"+this_region, fatjets_LepVeto.at(i).Eta(), fatjets_LepVeto.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/LepVetoFatJet_SDMass_"+this_region, fatjets_LepVeto.at(i).SDMass(), weight, 500, 0., 500.);

  }

  for(unsigned int i=0; i<fatjets_HN.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/HNFatJet_"+this_itoa+"_Eta_"+this_region, fatjets_HN.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/HNFatJet_"+this_itoa+"_Phi_"+this_region, fatjets_HN.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/HNFatJet_"+this_itoa+"_Eta_vs_Phi_"+this_region, fatjets_HN.at(i).Eta(), fatjets_HN.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/HNFatJet_"+this_itoa+"_SDMass_"+this_region, fatjets_HN.at(i).SDMass(), weight, 500, 0., 500.);

    FillHist(this_region+"/HNFatJet_Eta_"+this_region, fatjets_HN.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/HNFatJet_Phi_"+this_region, fatjets_HN.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/HNFatJet_Eta_vs_Phi_"+this_region, fatjets_HN.at(i).Eta(), fatjets_HN.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);
    FillHist(this_region+"/HNFatJet_SDMass_"+this_region, fatjets_HN.at(i).SDMass(), weight, 500, 0., 500.);

  }

  for(unsigned int i=0; i<jets.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/Jet_"+this_itoa+"_Eta_"+this_region, jets.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/Jet_"+this_itoa+"_Phi_"+this_region, jets.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/Jet_"+this_itoa+"_Eta_vs_Phi_"+this_region, jets.at(i).Eta(), jets.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);

    FillHist(this_region+"/Jet_Eta_"+this_region, jets.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/Jet_Phi_"+this_region, jets.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/Jet_Eta_vs_Phi_"+this_region, jets.at(i).Eta(), jets.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);

  }

  for(unsigned int i=0; i<jets_LepVeto.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/LepVetoJet_"+this_itoa+"_Eta_"+this_region, jets_LepVeto.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/LepVetoJet_"+this_itoa+"_Phi_"+this_region, jets_LepVeto.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/LepVetoJet_"+this_itoa+"_Eta_vs_Phi_"+this_region, jets_LepVeto.at(i).Eta(), jets_LepVeto.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);

    FillHist(this_region+"/LepVetoJet_Eta_"+this_region, jets_LepVeto.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/LepVetoJet_Phi_"+this_region, jets_LepVeto.at(i).Phi(), weight, 80, -4., 4.);
    FillHist(this_region+"/LepVetoJet_Eta_vs_Phi_"+this_region, jets_LepVeto.at(i).Eta(), jets_LepVeto.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);

  }

/*
  vector<Gen> gens = GetGens();
  vector<Muon> muons = GetMuons("POGTightWithTightIso", 10., 2.4);
  if(muons.size()!=2) return;
  if( muons.at(0).Charge()!=muons.at(1).Charge() ) return;
  for(unsigned int i=0; i<2; i++){
    int leptonType = GetLeptonType(muons.at(i), gens);
    FillHist("LeptonType", leptonType, 1., 20, -10., 10.);
  }
*/

}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


