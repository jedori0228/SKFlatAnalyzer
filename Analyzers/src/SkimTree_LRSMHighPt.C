#include "SkimTree_LRSMHighPt.h"

void SkimTree_LRSMHighPt::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_LRSMHighPt::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);
  newtree->SetBranchStatus("gen_*",0);

  triggers.clear();
  if(DataYear==2016){
    triggers = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_Ele27_WPTight_Gsf_v",
      "HLT_Photon175_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_DoublePhoton60_v",
    };
  }
  else if(DataYear==2017){
    triggers = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
      "HLT_Ele35_WPTight_Gsf_v",
      "HLT_Photon200_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_DoublePhoton70_v",
    };
  }
  else if(DataYear==2018){
    triggers = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_Photon200_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_DoublePhoton70_v",
    };
  }
  else{
    cout << "[SkimTree_LRSMHighPt::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_LRSMHighPt::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_LRSMHighPt::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

  for(int i=0; i<Userflags.size(); i++){
    if(Userflags.at(i).Contains("LeptonPtCut")){
      //==== Userflags.at(i) = "LeptonPtCut40";
      TString aaa = "LeptonPtCut";
      LeptonPtCut = Userflags.at(i).ReplaceAll(aaa,"").Atof();
    }
    if(Userflags.at(i).Contains("AK4JetPtCut")){
      TString aaa = "AK4JetPtCut";
      AK4JetPtCut = Userflags.at(i).ReplaceAll(aaa,"").Atof();
    }
  }
  cout << "[SkimTree_LRSMHighPt::initializeAnalyzer] LeptonPtCut = " << LeptonPtCut << endl;
  cout << "[SkimTree_LRSMHighPt::initializeAnalyzer] AK4JetPtCut = " << AK4JetPtCut << endl;

}

void SkimTree_LRSMHighPt::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;

  //==== Skim 2) at least one lepton (e or mu) with pt > "LeptonPtCut"

  vector<Muon> muons = GetMuons("NOCUT", LeptonPtCut, 2.4);
  vector<Electron> electrons = GetElectrons("NOCUT", LeptonPtCut, 2.4);
  if( muons.size()+electrons.size() == 0 ) return;

  //==== Skim 3) AK4 jet 

  vector<Jet> jets = GetJets("tightLepVeto", AK4JetPtCut, 2.4);
  vector<FatJet> fatjets = GetFatJets("tight", 170, 2.4);
  if( jets.size()+fatjets.size() == 0 ) return;

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_LRSMHighPt::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_LRSMHighPt::SkimTree_LRSMHighPt(){

  newtree = NULL;

}

SkimTree_LRSMHighPt::~SkimTree_LRSMHighPt(){

}

void SkimTree_LRSMHighPt::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


