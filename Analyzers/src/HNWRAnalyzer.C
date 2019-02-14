#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  RunSyst = HasFlag("RunSyst");
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");

  cout << "[HNWRAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunSyst = " << RunSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] PromptLeptonOnly = " << PromptLeptonOnly << endl;

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

}

void HNWRAnalyzer::executeEvent(){

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();
  //PrintGen(gens); return;

  //==== Prefire weight

  weight_Prefire = GetPrefireWeight(0);

  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_Veto_ID = "HNWRVeto";
  param.Electron_ID_SF_Key = "Default";
  param.Electron_FR_ID = "HNWR";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNWRTight";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 35.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_Veto_ID = "HNWRVeto";
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
  param.Muon_MinPt = 10.;

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

void HNWRAnalyzer::executeEventFromParameter(AnalyzerParameter param){

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

  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> this_AllTunePMuons = AllTunePMuons;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

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
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , +1 ); //FIXME not sure what to do for TuneP Muons..
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnDown){
    this_AllMuons = ScaleMuons( this_AllMuons, -1 );
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , -1 ); //FIXME not sure what to do for TuneP Muons..
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
  else{
    cout << "[HNWRAnalyzer::executeEventFromParameter] Wrong syst" << endl;
    exit(EXIT_FAILURE);
  }

  //==============
  //==== Leptons
  //==============

  std::vector<Electron> Veto_electrons = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> Veto_muons = SelectMuons(this_AllTunePMuons, param.Muon_Veto_ID, 10., 2.4);

  std::vector<Electron> Loose_electrons = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon> Loose_muons = SelectMuons(this_AllTunePMuons, param.Muon_Loose_ID, param.Muon_MinPt, 2.4);

  std::vector<Electron> Tight_electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);
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

  int n_Veto_leptons = Veto_electrons.size()+Veto_muons.size();
  int n_Loose_leptons = Loose_electrons.size()+Loose_muons.size();
  int n_Tight_leptons = Tight_electrons.size()+Tight_muons.size();

  FillHist("n_Tight_electrons_"+param.Name, Tight_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_electrons_"+param.Name, Loose_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Veto_electrons_"+param.Name, Veto_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_muons_"+param.Name, Tight_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_muons_"+param.Name, Loose_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Veto_muons_"+param.Name, Veto_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_leptons_"+param.Name, n_Tight_leptons, 1., 5, 0., 5.);
  FillHist("n_Loose_leptons_"+param.Name, n_Loose_leptons, 1., 5, 0., 5.);
  FillHist("n_Veto_leptons_"+param.Name, n_Veto_leptons, 1., 5, 0., 5.);

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
    if(alljets.at(i).IsTagged(Jet::CSVv2, Jet::Medium)) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<jets.size(); i++){
    HT += jets.at(i).Pt();
  }
  for(unsigned int i=0; i<fatjets.size(); i++){
    HT += fatjets.at(i).Pt();
  }

  //====================================================
  //==== Based on which trigger is fired
  //==== - Make sure, there is no duplicated events
  //====   between Suffixs. Apply exculsive selections
  //====   in map_bool_To_Region
  //====================================================

  std::vector< TString > Suffixs = {
    "SingleMuon",
    "SingleElectron",
  };
  std::vector< bool > PassTriggers = {
    PassMu50           && (Tight_electrons.size()==0) && (Tight_muons.size()>=1),
    PassSingleElectron && (Tight_electrons.size()>=1) && (Tight_muons.size()==0),
  };

  //=================
  //==== Start loop
  //=================

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    std::map<TString, bool> TMP_map_bool_To_Region; // For SS/OS
    std::map<TString, bool> map_bool_To_Region;

    //==== Event Selection : Trigger

    TString Suffix = Suffixs.at(it_Suffix);
    if( !PassTriggers.at(it_Suffix) ) continue;

    FillHist(Suffix+"_PassTrigger_"+param.Name, 0., 1., 1, 0., 1.);

    //==== Event Selection : Trigger Safe Pt Selection

    if(Suffix.Contains("SingleMuon")){
      if( Tight_muons.at(0).Pt() < TriggerSafePt_Muon ) continue;
      if(! (Tight_muons.at(0).PassID(param.Muon_Tight_ID)) ) continue;
    }
    else if(Suffix.Contains("SingleElectron")){
      if( Tight_electrons.at(0).Pt() < TriggerSafePt_Electron ) continue;
      if(! (Tight_electrons.at(0).PassID(param.Electron_Tight_ID)) ) continue;
    }
    else if(Suffix.Contains("EMu")){
      if( Tight_muons.at(0).Pt() < TriggerSafePt_Muon ) continue;
      if( Tight_electrons.at(0).Pt() < TriggerSafePt_Electron ) continue;
    }
    else{

    }

    map_bool_To_Region["PassTrigger"] = true;

    if(this->IsDATA){
      if( Suffix.Contains("SingleMuon") ){
        if( !(this->DataStream == "SingleMuon") ) continue;
      }
      else if( Suffix.Contains("SingleElectron") ){
        if( !(this->DataStream == "SingleElectron") ) continue;
      }
      else if( Suffix.Contains("EMu") ){
        if( !(this->DataStream == "SingleMuon") ) continue;
      }
      else{

      }
    }

    std::vector<Lepton *> Tight_leps_el, Tight_leps_mu;
    std::vector<Lepton *> Tight_leps;
    Tight_leps_el = MakeLeptonPointerVector(Tight_electrons);
    Tight_leps_mu = MakeLeptonPointerVector(Tight_muons);
    for(unsigned int i=0; i<Tight_leps_el.size(); i++) Tight_leps.push_back( Tight_leps_el.at(i) );
    for(unsigned int i=0; i<Tight_leps_mu.size(); i++) Tight_leps.push_back( Tight_leps_mu.at(i) );

    std::vector<Lepton *> Loose_leps_el, Loose_leps_mu;
    std::vector<Lepton *> Loose_leps;
    Loose_leps_el = MakeLeptonPointerVector(Loose_electrons);
    Loose_leps_mu = MakeLeptonPointerVector(Loose_muons);
    for(unsigned int i=0; i<Loose_leps_el.size(); i++) Loose_leps.push_back( Loose_leps_el.at(i) );
    for(unsigned int i=0; i<Loose_leps_mu.size(); i++) Loose_leps.push_back( Loose_leps_mu.at(i) );

    double weight = 1.;
    if(!IsDATA){
      weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight()*weight_Prefire;

      mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

      for(unsigned int i=0; i<Tight_electrons.size(); i++){
        double this_recosf = mcCorr->ElectronReco_SF(Tight_electrons.at(i).scEta(),Tight_electrons.at(i).Pt());
        double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, Tight_electrons.at(i).scEta(), Tight_electrons.at(i).Pt());
        weight *= this_recosf*this_idsf;
      }
      for(unsigned int i=0; i<Tight_muons.size(); i++){
        double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  Tight_muons.at(i).Eta(), Tight_muons.at(i).MiniAODPt());
        double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, Tight_muons.at(i).Eta(), Tight_muons.at(i).MiniAODPt());
        weight *= this_idsf*this_isosf;
      }

      double this_trigsf = 1.;
      if(Suffix.Contains("SingleMuon")){
        this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons);
      }
      else if(Suffix.Contains("SingleElectron")){
        this_trigsf = 1.;
      }
      else if(Suffix.Contains("EMu")){
        this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons);
      }
      else{

      }

      weight *= this_trigsf;

    }

    //=======================
    //==== Analysis Regions
    //=======================

    FatJet HNFatJet;

    Particle WRCand;
    Particle NCand;
    Particle NCand_1, NCand_2;

    FillHist(Suffix+"_NTightLepton_"+param.Name, Tight_leps.size(), 1., 5, 0., 5.);
    FillHist(Suffix+"_NLooseLepton_"+param.Name, Loose_leps.size(), 1., 5, 0., 5.);

    vector<Lepton *> Used_leps;

    //==== UMN

    Lepton LeadLep = (*Tight_leps.at(0));
    bool LeadLepPtCut = (LeadLep.Pt() > 60.);
    if(!LeadLepPtCut) continue;

    if(Tight_leps.size()==2 && (jets.size() >= 2)){
      Lepton SubLeadLep = (*Tight_leps[1]);
      bool SubLeadLepPtCut = (SubLeadLep.Pt() > 53.);
      bool IsResolved = SubLeadLepPtCut && ( (LeadLep+SubLeadLep).M() >= 200. );
      IsResolved = IsResolved && (LeadLep.DeltaR( SubLeadLep ) > 0.4);
      IsResolved = IsResolved && (jets.at(0).DeltaR ( jets.at(1) ) > 0.4);
      if( IsResolved ){
        map_bool_To_Region["Resolved"] = true;
        WRCand = LeadLep+SubLeadLep+jets.at(0)+jets.at(1);
        NCand = SubLeadLep+jets.at(0)+jets.at(1);
        NCand_1 = LeadLep+jets.at(0)+jets.at(1);
        NCand_2 = SubLeadLep+jets.at(0)+jets.at(1);
        Used_leps.push_back( Tight_leps.at(0) );
        Used_leps.push_back( Tight_leps.at(1) );
      }

    }
    else{
      bool IsBoosted = false;
      for(unsigned int i=0; i<fatjets_LSF.size(); i++){
        FatJet this_fatjet = fatjets_LSF.at(i);
        if( fabs( LeadLep.DeltaPhi(this_fatjet) ) > 2.0 ){
          IsBoosted = true;
          HNFatJet = this_fatjet;
          NCand = HNFatJet;
          WRCand = LeadLep+HNFatJet;
          break;
        }
      }
      if(IsBoosted){
        Used_leps.push_back( Tight_leps.at(0) );

        for(unsigned int k=0; k<Loose_leps.size(); k++){
          if( Loose_leps.at(k)->Pt() <= 53. ) continue;
          if( HNFatJet.DeltaR( *(Loose_leps.at(k)) ) < 0.8 ){
            //if( ( LeadLep+*(Loose_leps.at(k)) ).M() > 200. ){ //TODO Check if this cut is good
            map_bool_To_Region["Boosted"] = true;
            Used_leps.push_back( Loose_leps.at(k) );
            break;
          }
        }

      }
    }

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = (param.Name)+"_"+Suffix+"_"+this_region;

      if(RunCF){
        if(!(this_region.Contains("_SS"))) continue;
      }

      if(it_map->second){

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

        JSFillHist(this_region, "nPileUp_"+this_region, nPileUp, weight, 200., 0., 200.);
        JSFillHist(this_region, "nPV_"+this_region, nPV, weight, 200., 0., 200.);

        JSFillHist(this_region, "Lepton_Size_"+this_region, Used_leps.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        if(this_region.Contains("Boosted")){
          JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( Used_leps.at(0)->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);

          JSFillHist(this_region, "HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 1000, 0., 1000.);
          JSFillHist(this_region, "HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_LSF_"+this_region, HNFatJet.LSF(), weight, 100, 0., 1.);

        }

        if( Used_leps.size()>=2 ){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*Used_leps.at(0))+(*Used_leps.at(1))).M(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*Used_leps.at(0))+(*Used_leps.at(1))).Pt(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*Used_leps.at(0)).DeltaPhi(*Used_leps.at(1))), weight, 40, 0., 4.);
        }

        JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

        JSFillHist(this_region, "NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);


        FillLeptonPlots(Used_leps, this_region, weight);
        FillJetPlots(jets, fatjets, this_region, weight);

      } // END if(pass Region)

    } // END Loop mapRegionBool

  } // END Loop Suffixs



}

HNWRAnalyzer::HNWRAnalyzer(){

  RunFake = false;

}

HNWRAnalyzer::~HNWRAnalyzer(){

}
