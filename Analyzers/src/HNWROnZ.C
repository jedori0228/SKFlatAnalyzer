#include "HNWROnZ.h"

void HNWROnZ::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  RunSyst = HasFlag("RunSyst");
  NoTrigger = HasFlag("NoTrigger");
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  ApplyDYPtReweight = HasFlag("ApplyDYPtReweight");
  FindGenZ = HasFlag("FindGenZ");

  cout << "[HNWROnZ::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWROnZ::initializeAnalyzer] RunCF = " << RunCF << endl;
  cout << "[HNWROnZ::initializeAnalyzer] RunSyst = " << RunSyst << endl;
  cout << "[HNWROnZ::initializeAnalyzer] PromptLeptonOnly = " << PromptLeptonOnly << endl;
  cout << "[HNWROnZ::initializeAnalyzer] ApplyDYPtReweight = " << ApplyDYPtReweight << endl;
  cout << "[HNWROnZ::initializeAnalyzer] FindGenZ = " << FindGenZ << endl;

  //===============================
  //==== Year-dependent variables
  //===============================

  //==== Triggers

  Triggers_Electron.clear();
  Triggers_Muon.clear();

  if(DataYear==2016){

    Triggers_Electron = {
      "HLT_Ele27_WPTight_Gsf_v",
      "HLT_Photon175_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
    };
    TriggerNameForSF_Electron = "Ele27_WPTight_Gsf";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 30.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2017){

    Triggers_Electron = {
      "HLT_Ele35_WPTight_Gsf_v",
      "HLT_Photon200_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
    };
    TriggerNameForSF_Electron = "Ele35_WPTight_Gsf";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 38.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2018){

    Triggers_Electron = {
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_Photon200_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "Ele32_WPTight_Gsf";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 35.;
    TriggerSafePt_Muon = 52.;

  }

  //==== DY Pt Reweighting

  if(ApplyDYPtReweight){
    TString datapath = getenv("DATA_DIR");
    TFile *file_DYPtReweight = new TFile(datapath+"/"+TString::Itoa(DataYear,10)+"/DYPtReweight/DYPtReweight.root");
    hist_DYPtReweight_Electron = (TH1D *)file_DYPtReweight->Get("Electron");
    hist_DYPtReweight_Muon = (TH1D *)file_DYPtReweight->Get("Muon");
  }

  //==== B-tagging

  std::vector<Jet::Tagger> vtaggers;
  vtaggers.push_back(Jet::DeepCSV);

  std::vector<Jet::WP> v_wps;
  v_wps.push_back(Jet::Medium);

  //=== list of taggers, WP, setup systematics, use period SFs
  SetupBTagger(vtaggers,v_wps, true, true);

  if(FindGenZ){
    genDY = new GenFinderForDY();

    TDirectory* origDir = gDirectory;

    outfile->cd();
    tree_DY = new TTree("tree_DY", "tree_DY");
    tree_DY->Branch("lepch",&treevar_lepch,"treevar_lepch/I");
    tree_DY->Branch("RECO_Mass",&treevar_RECO_Mass,"treevar_RECO_Mass/D");
    tree_DY->Branch("RECO_Pt",&treevar_RECO_Pt,"treevar_RECO_Pt/D");
    tree_DY->Branch("GEN_Mass",&treevar_GEN_Mass, "treevar_GEN_Mass/D");
    tree_DY->Branch("GEN_Pt",&treevar_GEN_Pt, "treevar_GEN_Pt/D");

    origDir->cd();

  }

}

void HNWROnZ::executeEvent(){

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();

  //==== Prefire weight

  weight_Prefire = GetPrefireWeight(0);

  //==== Nvtx

  N_VTX = nPV;
  if(!IsDATA) N_VTX = nPileUp;
  //PUReweight =  //TODO

  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = false;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_ID_SF_Key = "HEEP";
  param.Electron_Trigger_SF_Key = "HEEP";
  param.Electron_FR_ID = "HNWR";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNWRTight";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 53.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";
  param.Muon_FR_ID = "HNWR";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNWRTight";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_UseTuneP = true;
  param.Muon_MinPt = 53.;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  AllElectrons = GetAllElectrons();
  AllMuons = GetAllMuons();
  AllTunePMuons = UseTunePMuon( AllMuons );
  AllJets = GetAllJets();
  AllFatJets = GetAllFatJets();

  executeEventFromParameter(param);

  if(RunSyst){

    for(int it_syst=1; it_syst<AnalyzerParameter::NSyst; it_syst++){
      param.syst_ = AnalyzerParameter::Syst(it_syst);
      param.Name = "Syst_"+param.GetSystType()+"_HNWR";
      executeEventFromParameter(param);
    }
  }

}

void HNWROnZ::executeEventFromParameter(AnalyzerParameter param){

  //=============
  //==== No Cut
  //=============

  FillHist("NoCut_"+param.Name, 0., 1., 1, 0., 1.);

  //========================
  //==== Event selecitions
  //========================

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  bool PassSingleElectron = ev.PassTrigger(Triggers_Electron);
  bool PassMu50 = ev.PassTrigger(Triggers_Muon);
  if(NoTrigger){
    PassSingleElectron = true;
    PassMu50 = true;
  }
  //if(!PassSingleElectron && !PassMu50) return;

  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> this_AllTunePMuons = AllTunePMuons;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  int SystDir_MuonIDSF(0);
  int SystDir_ElectronIDSF(0);

  if(param.syst_ == AnalyzerParameter::Central){

  }
  else if(param.syst_ == AnalyzerParameter::JetResUp){
    this_AllJets = SmearJets( this_AllJets, +1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetResDown){
    this_AllJets = SmearJets( this_AllJets, -1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnUp){
    this_AllJets = ScaleJets( this_AllJets, +1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnDown){
    this_AllJets = ScaleJets( this_AllJets, -1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnUp){
    this_AllMuons = ScaleMuons( this_AllMuons, +1 );
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , +1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnDown){
    this_AllMuons = ScaleMuons( this_AllMuons, -1 );
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonIDSFUp){
    SystDir_MuonIDSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonIDSFDown){
    SystDir_MuonIDSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResUp){
    this_AllElectrons = SmearElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResDown){
    this_AllElectrons = SmearElectrons( this_AllElectrons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp){
    this_AllElectrons = ScaleElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown){
    this_AllElectrons = ScaleElectrons( this_AllElectrons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFUp){
    SystDir_ElectronIDSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFDown){
    SystDir_ElectronIDSF = -1;
  }
  else{
    cerr << "[HNWROnZ::executeEventFromParameter] Wrong syst" << endl;
    exit(EXIT_FAILURE);
  }

  //==============
  //==== Leptons
  //==============

  std::vector<Electron> Loose_electrons = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, param.Electron_MinPt, 2.4);
  std::vector<Muon> Loose_muons = SelectMuons(this_AllTunePMuons, param.Muon_Loose_ID, param.Muon_MinPt, 2.4);

  std::vector<Electron> Tight_electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, param.Electron_MinPt, 2.4);
  std::vector<Muon> Tight_muons = SelectMuons(this_AllTunePMuons, param.Muon_Tight_ID, param.Muon_MinPt, 2.4);

  if(PromptLeptonOnly){
    Loose_electrons = ElectronPromptOnly(Loose_electrons, gens);
    Loose_muons = MuonPromptOnly(Loose_muons, gens);
    Tight_electrons = ElectronPromptOnly(Tight_electrons, gens);
    Tight_muons = MuonPromptOnly(Tight_muons, gens);
  }

  //==== Sorting
  std::sort(Loose_electrons.begin(), Loose_electrons.end(), PtComparing);
  std::sort(Loose_muons.begin(), Loose_muons.end(), PtComparing);
  std::sort(Tight_electrons.begin(), Tight_electrons.end(), PtComparing);
  std::sort(Tight_muons.begin(), Tight_muons.end(), PtComparing);

  int n_Loose_leptons = Loose_electrons.size()+Loose_muons.size();
  int n_Tight_leptons = Tight_electrons.size()+Tight_muons.size();
  //==== [CUT] : return if no tight lepton
  if(n_Tight_leptons==0) return;

  FillHist("n_Tight_electrons_"+param.Name, Tight_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_electrons_"+param.Name, Loose_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_muons_"+param.Name, Tight_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_muons_"+param.Name, Loose_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_leptons_"+param.Name, n_Tight_leptons, 1., 5, 0., 5.);
  FillHist("n_Loose_leptons_"+param.Name, n_Loose_leptons, 1., 5, 0., 5.);

  //==== Conver them to Lepton objects
  std::vector<Lepton *> Tight_leps_el, Tight_leps_mu;
  Tight_leps_el = MakeLeptonPointerVector(Tight_electrons);
  Tight_leps_mu = MakeLeptonPointerVector(Tight_muons);

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.4);
  std::vector<FatJet>   fatjets_LSF    = SelectFatJets(this_AllFatJets, "HNLSF", 200, 2.4);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);
  std::sort(fatjets_LSF.begin(), fatjets_LSF.end(), PtComparing);

  std::vector<Jet>      alljets         = SelectJets(this_AllJets, param.Jet_ID, 40., 2.4);
  std::sort(alljets.begin(), alljets.end(), PtComparing);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(alljets, Tight_electrons, Tight_muons);

  int NBJets=0;
  for(unsigned int i=0; i<alljets.size(); i++){
    if(IsBTagged(alljets.at(i), Jet::DeepCSV, Jet::Medium,true,0)) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<this_AllJets.size(); i++){
    HT += this_AllJets.at(i).Pt();
  }

  //===================
  //==== Event weight
  //===================

  double weight = 1.;
  if(!IsDATA){
    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight()*weight_Prefire;

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    for(unsigned int i=0; i<Tight_electrons.size(); i++){
      double this_recosf = mcCorr->ElectronReco_SF(Tight_electrons.at(i).scEta(),Tight_electrons.at(i).Pt(), SystDir_ElectronIDSF);
      double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, Tight_electrons.at(i).scEta(), Tight_electrons.at(i).Pt(), SystDir_ElectronIDSF);
      weight *= this_recosf*this_idsf;
    }
    for(unsigned int i=0; i<Tight_muons.size(); i++){
      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  Tight_muons.at(i).Eta(), Tight_muons.at(i).MiniAODPt(), SystDir_MuonIDSF);
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, Tight_muons.at(i).Eta(), Tight_muons.at(i).MiniAODPt(), SystDir_MuonIDSF);
      weight *= this_idsf*this_isosf;
    }

    //==== Trigger SF later
  }

  //=====================
  //==== Categorization
  //=====================

  std::map<TString, bool> map_bool_To_Region;
  vector<Lepton *> leps_for_plot;
  bool IsEE(false), IsMM(false);

  if(n_Tight_leptons==2){

    TString Suffix = "";
    bool this_triggerpass(false);
    if(      (Tight_electrons.size()==2) && (Tight_muons.size()==0) ){
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;
      IsEE = true;
    }
    else if( (Tight_electrons.size()==0) && (Tight_muons.size()==2) ){
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;
      IsMM = true;
    }

    if(this_triggerpass){

      std::vector<Lepton *> Tight_leps; // let's keep electron first for EM
      if( IsEE ){
        for(unsigned int i=0; i<Tight_leps_el.size(); i++) Tight_leps.push_back( Tight_leps_el.at(i) );
      }
      if( IsMM ){
        for(unsigned int i=0; i<Tight_leps_mu.size(); i++) Tight_leps.push_back( Tight_leps_mu.at(i) );
      }

      //==== Lepton pt cuts
      Lepton LeadLep = (*Tight_leps.at(0));
      Lepton SubLeadLep = (*Tight_leps.at(1));
      bool LeadLepPtCut = max( LeadLep.Pt(), SubLeadLep.Pt() ) > 60.;
      bool SubLeadLepPtCut = min( LeadLep.Pt(), SubLeadLep.Pt() ) > 53.;

      //==== lepton pt cuts
      if(LeadLepPtCut && SubLeadLepPtCut){

        if(IsOnZ( (LeadLep+SubLeadLep).M(), 10. )){

          map_bool_To_Region[Suffix+"_OnZ"] = true;

          leps_for_plot.push_back( Tight_leps.at(0) );
          leps_for_plot.push_back( Tight_leps.at(1) );

        }

      } // END if pt1>63 && pt2>50

    } // END if trigger fired

  } // END if # of tight lepton == 2

  if( map_bool_To_Region.size() == 0 ) return;

  double trigger_sf_SingleElectron = 1.;
  double trigger_sf_SingleMuon = 1.;
  if(!IsDATA){
    trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons);
    trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons);
  }

  if( IsEE ){
    weight *= trigger_sf_SingleElectron;
  }
  else if( IsMM ){
    weight *= trigger_sf_SingleMuon;
  }
  else{
    cerr << "[HNWROnZ::executeEventFromParameter] Unkown event.." << endl;
    exit(EXIT_FAILURE);
  }

  double ZPtReweight_EE(1.), ZPtReweight_MM(1.);
  if(ApplyDYPtReweight){
    if(leps_for_plot.size()>=2){
      double this_zpt = ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt();
      ZPtReweight_EE = GetDYPtReweight(this_zpt, 0);
      ZPtReweight_MM = GetDYPtReweight(this_zpt, 1);
    }
  }
  if( IsEE ){
    weight *= ZPtReweight_EE;
  }
  else if( IsMM ){
    weight *= ZPtReweight_MM;
  }
  else{
    //==== should be EM events
  }

  if(FindGenZ){

    Particle genZ = genDY->Find(gens);
    int leppid = genDY->LeptonPID;
    if(IsEE && leppid!=11) FillHist("WTF_EE_but_GenFindPIDIs..", leppid, 1., 20, 0., 20.);
    if(IsMM && leppid!=13) FillHist("WTF_MM_but_GenFindPIDIs..", leppid, 1., 20, 0., 20.);

    int this_int_lepch = IsEE ? 0 : 1;
    treevar_lepch = this_int_lepch;
    treevar_RECO_Mass = ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).M();
    treevar_RECO_Pt = ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt();
    treevar_GEN_Mass = genZ.M();
    treevar_GEN_Pt = genZ.Pt();

    tree_DY->Fill();

  }

  for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

    TString this_region = it_map->first;
    this_region = (param.Name)+"_"+this_region;

    if(it_map->second){

      JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
      JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

      JSFillHist(this_region, "nPileUp_"+this_region, nPileUp, weight, 200., 0., 200.);
      JSFillHist(this_region, "nPV_"+this_region, nPV, weight, 200., 0., 200.);
      JSFillHist(this_region, "N_VTX_"+this_region, N_VTX, weight, 200., 0., 200.);

      JSFillHist(this_region, "Lepton_Size_"+this_region, leps_for_plot.size(), weight, 10, 0., 10.);

      JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

      JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);
      JSFillHist(this_region, "HT_"+this_region, HT, weight, 4000, 0., 4000.);

      if( leps_for_plot.size()>=2 ){
        JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).M(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*leps_for_plot.at(0)).DeltaPhi(*leps_for_plot.at(1))), weight, 40, 0., 4.);
      }

      FillLeptonPlots(leps_for_plot, this_region, weight);
      FillJetPlots(jets, fatjets_LSF, this_region, weight);

    } // END if(pass Region)

  } // END Loop mapRegionBool

}

HNWROnZ::HNWROnZ(){

  RunFake = false;

}

HNWROnZ::~HNWROnZ(){

}

double HNWROnZ::GetDYPtReweight(double zpt, int flav){

  if(zpt>=500.) zpt = 499.;
  if(flav==0){
    int this_bin = hist_DYPtReweight_Electron->FindBin(zpt);
    return hist_DYPtReweight_Electron->GetBinContent(this_bin);
  }
  else if(flav==1){
    int this_bin = hist_DYPtReweight_Muon->FindBin(zpt);
    return hist_DYPtReweight_Muon->GetBinContent(this_bin);
  }
  else{
    cerr << "[HNWROnZ::GetDYPtReweight] wrong flavour : " << flav << endl;
    exit(EXIT_FAILURE);
  }

}

void HNWROnZ::WriteHist(){

  AnalyzerCore::WriteHist();

  if(FindGenZ){
    outfile->cd();
    tree_DY->Write();
  }

}
