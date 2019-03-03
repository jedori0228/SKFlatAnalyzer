#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  RunSyst = HasFlag("RunSyst");
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  ApplyDYPtReweight = HasFlag("ApplyDYPtReweight");

  cout << "[HNWRAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunSyst = " << RunSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] PromptLeptonOnly = " << PromptLeptonOnly << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] ApplyDYPtReweight = " << ApplyDYPtReweight << endl;

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

}

void HNWRAnalyzer::executeEvent(){

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();
  //PrintGen(gens); return;

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

  std::vector<Electron> Veto_electrons = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 10., 2.4);
  std::vector<Muon> Veto_muons = SelectMuons(this_AllTunePMuons, param.Muon_Veto_ID, 10., 2.4);

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
    if(IsBTagged(alljets.at(i), Jet::DeepCSV, Jet::Medium,true,0)) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<this_AllJets.size(); i++){
    HT += this_AllJets.at(i).Pt();
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
    "EMu",
  };
  std::vector< bool > PassTriggers = {
    PassMu50           && (Tight_electrons.size()<=1) && (Tight_muons.size()>=1),
    PassSingleElectron && (Tight_electrons.size()>=1) && (Tight_muons.size()<=1),
    PassMu50           && (Tight_electrons.size()==1) && (Tight_muons.size()==1),
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

    bool IsEE = Suffix.Contains("SingleElectron");
    bool IsMM = Suffix.Contains("SingleMuon");
    bool IsEMu = Suffix.Contains("EMu");

    FillHist(Suffix+"_PassTrigger_"+param.Name, 0., 1., 1, 0., 1.);

    //==== Event Selection : Trigger Safe Pt Selection

    if(IsMM){
      if( Tight_muons.at(0).Pt() < TriggerSafePt_Muon ) continue;
    }
    else if(IsEE){
      if( Tight_electrons.at(0).Pt() < TriggerSafePt_Electron ) continue;
    }
    else if(IsEMu){
      if( Tight_muons.at(0).Pt() < TriggerSafePt_Muon ) continue;
      if( Tight_electrons.at(0).Pt() < TriggerSafePt_Electron ) continue;
    }
    else{

    }

    map_bool_To_Region["PassTrigger"] = true;

    if(this->IsDATA){
      if( IsMM ){
        if( !(this->DataStream == "SingleMuon") ) continue;
      }
      else if( IsEE ){
        if( !(this->DataStream == "SingleElectron") ) continue;
      }
      else if( IsEMu ){
        if( !(this->DataStream == "SingleMuon") ) continue;
      }
      else{

      }
    }

    std::vector<Lepton *> Tight_leps_el, Tight_leps_mu;
    std::vector<Lepton *> Tight_leps;
    Tight_leps_el = MakeLeptonPointerVector(Tight_electrons);
    Tight_leps_mu = MakeLeptonPointerVector(Tight_muons);

    if( IsEE || IsEMu ){
      for(unsigned int i=0; i<Tight_leps_el.size(); i++) Tight_leps.push_back( Tight_leps_el.at(i) );
    }
    if( IsMM || IsEMu ){
      for(unsigned int i=0; i<Tight_leps_mu.size(); i++) Tight_leps.push_back( Tight_leps_mu.at(i) );
    }

    //==== Inside AK8 jet
    std::vector<Lepton *> Loose_SF_leps, Loose_OF_leps;

    if( IsEE ){
      Loose_SF_leps = MakeLeptonPointerVector(Loose_electrons);
      Loose_OF_leps = MakeLeptonPointerVector(Loose_muons);
    }
    else if( IsMM ){
      Loose_SF_leps = MakeLeptonPointerVector(Loose_muons);
      Loose_OF_leps = MakeLeptonPointerVector(Loose_electrons);
    }
    else if( IsEMu ){
    }

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
      if(IsMM){
        this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons);
      }
      else if(IsEE){
        this_trigsf = 1.;
      }
      else if(IsEMu){
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
    FillHist(Suffix+"_NLooseSFLepton_"+param.Name, Loose_SF_leps.size(), 1., 5, 0., 5.);
    FillHist(Suffix+"_NLooseOFLepton_"+param.Name, Loose_OF_leps.size(), 1., 5, 0., 5.);

    vector<Lepton *> OnZ_leps, Used_leps;
    OnZ_leps.clear();
    Used_leps.clear();

    //==== UMN

    Lepton LeadLep = (*Tight_leps.at(0));
    bool LeadLepPtCut = (LeadLep.Pt() > 60.);
    //==== if emu, at(0) is always electron
    if( IsEMu ){
      LeadLepPtCut = max( Tight_leps.at(0)->Pt(), Tight_leps.at(1)->Pt() ) > 60.;
    }
    if(!LeadLepPtCut) continue;

    FillHist(Suffix+"_LeadingLeptonPtCut_"+param.Name, 0., 1., 1, 0., 1.);

    bool FourResolvedObject = false;

    if(Tight_leps.size()==2){

      OnZ_leps.push_back( Tight_leps.at(0) );
      OnZ_leps.push_back( Tight_leps.at(1) );

      Lepton SubLeadLep = (*Tight_leps[1]);
      bool SubLeadLepPtCut = (SubLeadLep.Pt() > 53.);
      if( IsEMu ){
        LeadLepPtCut = min( Tight_leps.at(0)->Pt(), Tight_leps.at(1)->Pt() ) > 53.;
      }
      bool DiLepMassOnZ = IsOnZ( (LeadLep+SubLeadLep).M(), 10. );

      //==== - HNWR_SingleElectron_OnZ : ee Resolved SR
      //==== - HNWR_SingleMuon_OnZ : mm Resolved SR
      //==== - HNWR_EMu_OnZ : em Resolved CR (ttbar dominant)
      if(SubLeadLepPtCut && DiLepMassOnZ) map_bool_To_Region["OnZ"] = true;

      if( jets.size() >= 2 ){

        Used_leps.push_back( Tight_leps.at(0) );
        Used_leps.push_back( Tight_leps.at(1) );

        FourResolvedObject = true;

        FillHist(Suffix+"_TwoLeptonAndTwoJet_"+param.Name, 0., 1., 1, 0., 1.);

        bool DiLepMassGT200 = ((LeadLep+SubLeadLep).M() > 200.);
        bool DiLepMassLT150 = ((LeadLep+SubLeadLep).M() < 150.);
        bool dRTwoLepton = (LeadLep.DeltaR( SubLeadLep ) > 0.4);
        bool dRTwoJets = (jets.at(0).DeltaR ( jets.at(1) ) > 0.4);

        //==== Resolved without mll cut
        bool IsResolved = SubLeadLepPtCut && dRTwoLepton && dRTwoJets;
        if( IsResolved ){

          FillHist(Suffix+"_ResolvedBeforeMassCut_"+param.Name, 0., 1., 1, 0., 1.);

          //==== - HNWR_SingleElectron_Resolved_SR : ee Resolved SR
          //==== - HNWR_SingleMuon_Resolved_SR : mm Resolved SR
          //==== - HNWR_EMu_Resolved_SR : em Resolved CR (ttbar dominant)
          if(DiLepMassGT200) map_bool_To_Region["Resolved_SR"] = true;

          //==== - HNWR_SingleElectron_Resolved_DYCR : ee Resolved CR (DY domiant) -> extrapolate with fiting
          //==== - HNWR_SingleMuon_Resolved_DYCR : mm Resolved CR (DY domiant) -> extrapolate with fiting
          //==== - HNWR_EMu_Resolved_SR : NOT USED
          if(DiLepMassLT150) map_bool_To_Region["Resolved_DYCR"] = true;

          WRCand = LeadLep+SubLeadLep+jets.at(0)+jets.at(1);
          NCand = SubLeadLep+jets.at(0)+jets.at(1);
          NCand_1 = LeadLep+jets.at(0)+jets.at(1);
          NCand_2 = SubLeadLep+jets.at(0)+jets.at(1);
        }
      }
    }

    //==== For EMu, do not fill boosted
    if(!FourResolvedObject && !IsEMu){

      bool HasAwayMergedFatJet = false;
      for(unsigned int i=0; i<fatjets_LSF.size(); i++){
        FatJet this_fatjet = fatjets_LSF.at(i);
        if( fabs( LeadLep.DeltaPhi(this_fatjet) ) > 2.0 ){
          HasAwayMergedFatJet = true;
          HNFatJet = this_fatjet;
          NCand = HNFatJet;
          WRCand = LeadLep+HNFatJet;
          break;
        }
      }

      if(HasAwayMergedFatJet){
        Used_leps.clear();
        Used_leps.push_back( Tight_leps.at(0) );

        bool HasSFLooseLepton = false;
        Lepton *SFLooseLepton;
        bool HasOFLooseLepton = false;
        Lepton *OFLooseLepton;

/*
        //==== Pointer checked.. address changed
        cout << "================================" << endl;
        cout << "Suffix = " << Suffix << endl;
        cout << "Isolated : " << Tight_leps.at(0) << " (" << Tight_leps.at(0)->LeptonFlavour() << ") : pt = " << Tight_leps.at(0)->Pt() << ", eta = " << Tight_leps.at(0)->Eta() << endl;
*/
        for(unsigned int k=0; k<Loose_SF_leps.size(); k++){
/*
          cout << "  " << Loose_SF_leps.at(k) << " (" << Loose_SF_leps.at(k)->LeptonFlavour() << ") : pt = " << Loose_SF_leps.at(k)->Pt() << ", eta = " << Loose_SF_leps.at(k)->Eta() << endl;
*/

          if( Tight_leps.at(0)->DeltaR( *(Loose_SF_leps.at(k)) ) < 0.01 ) continue;
          if( Loose_SF_leps.at(k)->Pt() <= 53. ) continue;
          if( HNFatJet.DeltaR( *(Loose_SF_leps.at(k)) ) < 0.8 ){
            HasSFLooseLepton = true;
            SFLooseLepton = Loose_SF_leps.at(k);
            break;
          }
        }

        for(unsigned int k=0; k<Loose_OF_leps.size(); k++){

          if( Tight_leps.at(0)->DeltaR( *(Loose_OF_leps.at(k)) ) < 0.01 ) continue;
          if( Loose_OF_leps.at(k)->Pt() <= 53. ) continue;
          if( HNFatJet.DeltaR( *(Loose_OF_leps.at(k)) ) < 0.8 ){
            HasOFLooseLepton = true;
            OFLooseLepton = Loose_OF_leps.at(k);
            break;
          }
        }

        //==== For isolated el + isolated mu event, it can be both "el + mu-jet" and "mu + el-jet" event.
        //==== Veto if the other side has AK8 jet
        bool NoTwoMergedJet = false;
        for(unsigned int i=0; i<fatjets_LSF.size(); i++){
          FatJet this_fatjet = fatjets_LSF.at(i);
          if( fabs( LeadLep.DeltaR(this_fatjet) ) < 0.8 ){
            NoTwoMergedJet = true;
            break;
          }
        }

        FillHist(Suffix+"_HasSFLooseLepton_"+param.Name, HasSFLooseLepton, 1., 2, 0., 2.);
        FillHist(Suffix+"_HasOFLooseLepton_"+param.Name, HasOFLooseLepton, 1., 2, 0., 2.);
        //==== ee or mm
        if(HasSFLooseLepton && !HasOFLooseLepton){
          if( (LeadLep+*SFLooseLepton).M() > 200 ){
            //==== - HNWR_SingleElectron_Boosted_SR : ee Boosted SR
            //==== - HNWR_SingleMuon_Boosted_SR : mm Boosted SR
            map_bool_To_Region["Boosted_SR"] = true;
          }
          else if( (LeadLep+*SFLooseLepton).M() < 150 ){
            //==== - HNWR_SingleElectron_Boosted_DYCR : ee Boosted CR (DY dominant) -> extrapolate with fiting
            //==== - HNWR_SingleMuon_Boosted_DYCR : mm Boosted CR (DY dominant) -> extrapolate with fiting
            map_bool_To_Region["Boosted_DYCR"] = true;
          }
          Used_leps.push_back( SFLooseLepton );
        }

        if(!HasSFLooseLepton && HasOFLooseLepton && NoTwoMergedJet){

          if( (LeadLep+*OFLooseLepton).M() > 200 ){
            //==== - HNWR_SingleElectron_EMu_Boosted_CR : isolated e + mu-AK8jet (ttbar dominant)
            //==== - HNWR_SingleMuon_EMu_Boosted_CR : isolated m + e-AK9jet (ttbar dominant)
            map_bool_To_Region["EMu_Boosted_CR"] = true;
          }
          Used_leps.push_back( OFLooseLepton );
        }

      }
    }

    if(ApplyDYPtReweight && Used_leps.size()>=2){
      double this_zpt = ((*Used_leps.at(0))+(*Used_leps.at(1))).Pt();
      if(IsEE) weight *= GetDYPtReweight(this_zpt, 0);
      else if(IsMM) weight *= GetDYPtReweight(this_zpt, 1);
      else {}
    }

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = (param.Name)+"_"+Suffix+"_"+this_region;

      if(RunCF){
        if(!(this_region.Contains("_SS"))) continue;
      }

      if(it_map->second){

        vector<Lepton *> this_leps;
        if(this_region.Contains("OnZ")) this_leps = OnZ_leps;
        else this_leps = Used_leps;

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

        JSFillHist(this_region, "nPileUp_"+this_region, nPileUp, weight, 200., 0., 200.);
        JSFillHist(this_region, "nPV_"+this_region, nPV, weight, 200., 0., 200.);
        JSFillHist(this_region, "N_VTX_"+this_region, N_VTX, weight, 200., 0., 200.);

        JSFillHist(this_region, "Lepton_Size_"+this_region, this_leps.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);
        JSFillHist(this_region, "HT_"+this_region, HT, weight, 4000, 0., 4000.);

        if(this_region.Contains("Boosted")){
          JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( this_leps.at(0)->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);

          JSFillHist(this_region, "HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 1000, 0., 1000.);
          JSFillHist(this_region, "HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_LSF_"+this_region, HNFatJet.LSF(), weight, 100, 0., 1.);

        }

        if( this_leps.size()>=2 ){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*this_leps.at(0))+(*this_leps.at(1))).M(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*this_leps.at(0))+(*this_leps.at(1))).Pt(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*this_leps.at(0)).DeltaPhi(*this_leps.at(1))), weight, 40, 0., 4.);
        }

        JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

        JSFillHist(this_region, "NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);


        FillLeptonPlots(this_leps, this_region, weight);
        FillJetPlots(jets, fatjets_LSF, this_region, weight);

      } // END if(pass Region)

    } // END Loop mapRegionBool

  } // END Loop Suffixs



}

HNWRAnalyzer::HNWRAnalyzer(){

  RunFake = false;

}

HNWRAnalyzer::~HNWRAnalyzer(){

}

double HNWRAnalyzer::GetDYPtReweight(double zpt, int flav){

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
    cout << "[HNWRAnalyzer::GetDYPtReweight] wrong flavour : " << flav << endl;
    exit(EXIT_FAILURE);
  }

}
