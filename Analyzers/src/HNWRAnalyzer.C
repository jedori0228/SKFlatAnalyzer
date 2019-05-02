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
  FastSim = HasFlag("FastSim");

  cout << "[HNWRAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunSyst = " << RunSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] PromptLeptonOnly = " << PromptLeptonOnly << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] ApplyDYPtReweight = " << ApplyDYPtReweight << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] FastSim = " << FastSim << endl;

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

  //==== Signal finder
  genFinder = new GenFinderForHNWRSignal();

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
  param.Electron_ID_SF_Key = "HEEP";
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

  param.dRSeparation = 0.4;

  executeEventFromParameter(param);

/*
  //==== For dR Separation test

  param.Name = "HNWRdR0p5";
  param.dRSeparation = 0.5;
  executeEventFromParameter(param);

  param.Name = "HNWRdR0p6";
  param.dRSeparation = 0.6;
  executeEventFromParameter(param);

  param.Name = "HNWRdR0p7";
  param.dRSeparation = 0.7;
  executeEventFromParameter(param);
*/

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
  if(FastSim){
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
    cerr << "[HNWRAnalyzer::executeEventFromParameter] Wrong syst : param.syst_ = " << param.syst_ << endl;
    exit(EXIT_FAILURE);
  }

  //==============
  //==== Leptons
  //==============

  //==== First, make base lepton vectors
  std::vector<Electron> myelectrons = SelectElectrons(this_AllElectrons, "HNWRLT", param.Electron_MinPt, 2.4);
  std::vector<Muon> mymuons = SelectMuons(this_AllTunePMuons, "HNWRLT", param.Muon_MinPt, 2.4);
  if(PromptLeptonOnly){
    myelectrons = ElectronPromptOnly(myelectrons, gens);
    mymuons = MuonPromptOnly(mymuons, gens);
  }
  std::sort(myelectrons.begin(), myelectrons.end(), PtComparing);
  std::sort(mymuons.begin(), mymuons.end(), PtComparing);

  //==== With this, make Loose and Tight lepton vectors, but use pointer
  std::vector<Electron *> Loose_electrons, Tight_electrons;
  std::vector<Muon *> Loose_muons, Tight_muons;
  std::vector<Lepton *> Tight_leps_el, Tight_leps_mu, Tight_leps;
  std::vector<Lepton *> Loose_leps_el, Loose_leps_mu, Loose_leps;
  for(unsigned int i=0; i<myelectrons.size(); i++){
    Electron& el = myelectrons.at(i);
    if(el.PassID(param.Electron_Loose_ID)){
      Loose_electrons.push_back( &el );
      Loose_leps_el.push_back( &el );
      Loose_leps.push_back( &el );
    }
    if(el.PassID(param.Electron_Tight_ID)){
      Tight_electrons.push_back( &el );
      Tight_leps_el.push_back( &el );
      Tight_leps.push_back( &el );
    }
  }
  for(unsigned int i=0; i<mymuons.size(); i++){
    Muon& mu = mymuons.at(i);
    if(mu.PassID(param.Muon_Loose_ID)){
      Loose_muons.push_back( &mu );
      Loose_leps_mu.push_back( &mu );
      Loose_leps.push_back( &mu );
    }
    if(mu.PassID(param.Muon_Tight_ID)){
      Tight_muons.push_back( &mu );
      Tight_leps_mu.push_back( &mu );
      Tight_leps.push_back( &mu );
    }
  }
  //==== Sorting leptons, not distinguishing flavour
  std::sort(Tight_leps.begin(), Tight_leps.end(), PtComparingPtr);

  //==== # of leptons
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

  //==== [CUT] return if lead pt <= 60 GeV
  if(Tight_leps.at(0)->Pt()<=60.) return;

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.4);
  std::vector<FatJet>   fatjets_LSF    = SelectFatJets(this_AllFatJets, "HNLSF", 200, 2.4);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);
  std::sort(fatjets_LSF.begin(), fatjets_LSF.end(), PtComparing);

  std::vector<Jet>      alljets         = SelectJets(this_AllJets, param.Jet_ID, 40., 2.4);
  std::sort(alljets.begin(), alljets.end(), PtComparing);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(alljets, Tight_electrons, Tight_muons, param.dRSeparation);

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
      double this_recosf = mcCorr->ElectronReco_SF(Tight_electrons.at(i)->scEta(),Tight_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
      double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, Tight_electrons.at(i)->scEta(), Tight_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
      weight *= this_recosf*this_idsf;
    }
    for(unsigned int i=0; i<Tight_muons.size(); i++){
      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
      weight *= this_idsf*this_isosf;
    }

    //==== Trigger SF later
  }

  //=====================
  //==== Categorization
  //=====================

  std::map<TString, bool> map_bool_To_Region;

  FatJet HNFatJet;
  Particle WRCand;
  Particle NCand;
  Particle NCand_1, NCand_2;

  vector<Lepton *> leps_for_plot;
  int NExtraLooseElectron(0),NExtraLooseMuon(0),NExtraLooseLepton(0);

  //==== Check Resolved first

  bool IsResolvedEvent = false;
  bool IsResolved_SR_EE(false), IsResolved_SR_MM(false), IsResolved_SR_EM(false);
  bool IsResolved_DYCR_EE(false), IsResolved_DYCR_MM(false), IsResolved_DYCR_EM(false);
  if(n_Tight_leptons==2){

    TString Suffix = "";
    bool tmp_IsEE(false), tmp_IsMM(false), tmp_IsEM(false);
    bool this_triggerpass(false);
    if(      (Tight_electrons.size()==2) && (Tight_muons.size()==0) ){
      tmp_IsEE = true;
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;
    }
    else if( (Tight_electrons.size()==0) && (Tight_muons.size()==2) ){
      tmp_IsMM = true;
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;
    }
    else if( (Tight_electrons.size()==1) && (Tight_muons.size()==1) ){
      tmp_IsEM = true;
      Suffix = "EMu";
      this_triggerpass = PassMu50;
    }

    if(this_triggerpass){

      //==== lljj

      if( jets.size()>=2 ){

        Lepton *LeadLep = Tight_leps.at(0);
        Lepton *SubLeadLep = Tight_leps.at(1);

        bool dRTwoLepton = (LeadLep->DeltaR( *SubLeadLep ) > param.dRSeparation);
        bool dRTwoJets = (jets.at(0).DeltaR ( jets.at(1) ) > param.dRSeparation);

        if( dRTwoLepton && dRTwoJets ){

          IsResolvedEvent = true;

          bool DiLepMassGT200 = ((*LeadLep+*SubLeadLep).M() > 200.);
          bool DiLepMassLT150 = ((*LeadLep+*SubLeadLep).M() < 150.);

          if(DiLepMassGT200){
            map_bool_To_Region[Suffix+"_Resolved_SR"] = true;
            if(tmp_IsEE) IsResolved_SR_EE = true;
            else if(tmp_IsMM) IsResolved_SR_MM = true;
            else if(tmp_IsEM) IsResolved_SR_EM = true;
          }

          leps_for_plot.push_back( Tight_leps.at(0) );
          leps_for_plot.push_back( Tight_leps.at(1) );

          //==== - HNWR_SingleElectron_Resolved_DYCR : ee Resolved CR (DY domiant) -> extrapolate with fiting
          //==== - HNWR_SingleMuon_Resolved_DYCR : mm Resolved CR (DY domiant) -> extrapolate with fiting
          //==== - HNWR_EMu_Resolved_SR : NOT USED
          if(DiLepMassLT150){
            map_bool_To_Region[Suffix+"_Resolved_DYCR"] = true;
            if(tmp_IsEE) IsResolved_DYCR_EE = true;
            else if(tmp_IsMM) IsResolved_DYCR_MM = true;
            else if(tmp_IsEM) IsResolved_DYCR_EM = true;
          }

          WRCand = *LeadLep+*SubLeadLep+jets.at(0)+jets.at(1);
          NCand = *SubLeadLep+jets.at(0)+jets.at(1);
          NCand_1 = *LeadLep+jets.at(0)+jets.at(1);
          NCand_2 = *SubLeadLep+jets.at(0)+jets.at(1);

          for(unsigned int i=0; i<Loose_leps.size(); i++){
            if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SubLeadLep){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraLooseLepton++;
            if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
            else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
            else{
              cout << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in RESOLVED" << endl;
              exit(EXIT_FAILURE);
            }

          }

/*
          //==== XXX debugging
          if(IsResolved_SR_MM){
            if(NExtraLooseLepton>0){
              cout << "==========================================" << endl;
              cout << "NExtraLooseElectron = " << NExtraLooseElectron << endl;
              cout << "NExtraLooseMuon = " << NExtraLooseMuon << endl;
              //genFinder->Debug=true;
              genFinder->Run(gens);
              genFinder->Print();
              genFinder->PrintGen(gens);
              cout << "#### RECO ####" << endl;
              for(unsigned int i=0; i<Loose_leps.size(); i++){
                if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SubLeadLep){
                  cout << i << " (Duplicated) ";Loose_leps.at(i)->Print();
                  //cout << "--> duplicate" << endl;
                  continue;
                }
                if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON){
                  cout << i << " (Electron) ";Loose_leps.at(i)->Print();
                }
                else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON){
                  cout << i << " (Muon) ";Loose_leps.at(i)->Print();
                }
                else{
                  cout << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in RESOLVED" << endl;
                  exit(EXIT_FAILURE);
                }

              }

            }

          } // END Debugging
*/

        } // END if dR(l,l)>0.4 && dR(j,j)>0.4

      } // END if # of jets >= 2

    } // END Trigger fired

  } // END if # of tight lepton == 2

  bool IsBoostedEvent(false);
  bool IsBoosted_SR_EE(false), IsBoosted_SR_MM(false);
  bool IsBoosted_DYCR_EE(false), IsBoosted_DYCR_MM(false);
  bool IsBoosted_CR_EMJet(false), IsBoosted_CR_MEJet(false);
  if(!IsResolvedEvent){

    Lepton *LeadLep = Tight_leps.at(0);
    bool tmp_IsLeadE(false), tmp_IsLeadM(false);
    TString Suffix = "";
    bool this_triggerpass(false);
    if(LeadLep->LeptonFlavour()==Lepton::ELECTRON){
      tmp_IsLeadE = true;
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;
    }
    else if(LeadLep->LeptonFlavour()==Lepton::MUON){
      tmp_IsLeadM = true;
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;
    }
    else{
      cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong flavour : " << LeadLep->LeptonFlavour() << endl;
      exit(EXIT_FAILURE);
    }

    if(this_triggerpass){

      std::vector<Lepton *> Loose_SF_leps = tmp_IsLeadE ? Loose_leps_el : Loose_leps_mu;
      std::vector<Lepton *> Loose_OF_leps = tmp_IsLeadE ? Loose_leps_mu : Loose_leps_el;;

      bool HasAwayMergedFatJet = false;
      for(unsigned int i=0; i<fatjets_LSF.size(); i++){
        FatJet this_fatjet = fatjets_LSF.at(i);
        if( fabs( LeadLep->DeltaPhi(this_fatjet) ) > 2.0 ){
          HasAwayMergedFatJet = true;
          HNFatJet = this_fatjet;
          NCand = HNFatJet;
          WRCand = *LeadLep+HNFatJet;
          break;
        }
      }

      if(HasAwayMergedFatJet){

        bool HasSFLooseLepton = false;
        Lepton *SFLooseLepton;
        bool HasOFLooseLepton = false;
        Lepton *OFLooseLepton;

        for(unsigned int k=0; k<Loose_SF_leps.size(); k++){
          if( LeadLep->DeltaR( *(Loose_SF_leps.at(k)) ) < 0.01 ) continue;
          if( Loose_SF_leps.at(k)->Pt() <= 53. ) continue;
          if( HNFatJet.DeltaR( *(Loose_SF_leps.at(k)) ) < 0.8 ){
            HasSFLooseLepton = true;
            SFLooseLepton = Loose_SF_leps.at(k);
            break;
          }
        }

        for(unsigned int k=0; k<Loose_OF_leps.size(); k++){

          if( LeadLep->DeltaR( *(Loose_OF_leps.at(k)) ) < 0.01 ) continue;
          if( Loose_OF_leps.at(k)->Pt() <= 53. ) continue;
          if( HNFatJet.DeltaR( *(Loose_OF_leps.at(k)) ) < 0.8 ){
            HasOFLooseLepton = true;
            OFLooseLepton = Loose_OF_leps.at(k);
            break;
          }
        }

        leps_for_plot.push_back( Tight_leps.at(0) );
        if(HasSFLooseLepton && !HasOFLooseLepton){

          leps_for_plot.push_back( SFLooseLepton );

          if( (*LeadLep+*SFLooseLepton).M() > 200 ){
            //==== - HNWR_SingleElectron_Boosted_SR : ee Boosted SR
            //==== - HNWR_SingleMuon_Boosted_SR : mm Boosted SR
            map_bool_To_Region[Suffix+"_Boosted_SR"] = true;
            if(tmp_IsLeadE) IsBoosted_SR_EE = true;
            else if(tmp_IsLeadM) IsBoosted_SR_MM = true;
          }
          else if( (*LeadLep+*SFLooseLepton).M() < 150 ){
            //==== - HNWR_SingleElectron_Boosted_DYCR : ee Boosted CR (DY dominant) -> extrapolate with fiting
            //==== - HNWR_SingleMuon_Boosted_DYCR : mm Boosted CR (DY dominant) -> extrapolate with fiting
            map_bool_To_Region[Suffix+"_Boosted_DYCR"] = true;
            if(tmp_IsLeadE) IsBoosted_DYCR_EE = true; 
            else if(tmp_IsLeadM) IsBoosted_DYCR_MM = true;
          }
        }

        if(!HasSFLooseLepton && HasOFLooseLepton){

          leps_for_plot.push_back( OFLooseLepton );

          if( (*LeadLep+*OFLooseLepton).M() > 200 ){
            //==== - HNWR_SingleElectron_EMu_Boosted_CR : isolated e + mu-AK8jet (ttbar dominant)
            //==== - HNWR_SingleMuon_EMu_Boosted_CR : isolated m + e-AK9jet (ttbar dominant)
            map_bool_To_Region[Suffix+"_EMu_Boosted_CR"] = true;
            if(tmp_IsLeadE) IsBoosted_CR_EMJet = true;
            else if(tmp_IsLeadM) IsBoosted_CR_MEJet = true;
          }
        }

        for(unsigned int i=0; i<Loose_leps.size(); i++){
          if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SFLooseLepton || Loose_leps.at(i)==OFLooseLepton){
            //cout << "--> duplicate" << endl;
            continue;
          }
          NExtraLooseLepton++;
          if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
          else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
          else{
            cout << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in BOOSTED" << endl;
            exit(EXIT_FAILURE);
          }

        }

      } // END If has merged jet

    } // END If trigger fired
  } // END If not resolved

  if( map_bool_To_Region.size() == 0 ) return;

  double trigger_sf_SingleElectron = 1.;
  double trigger_sf_SingleMuon = 1.;
  if(!IsDATA){
    trigger_sf_SingleElectron = 1.; // TODO Measure it
    trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons);
  }

  if( IsResolved_SR_EE || IsResolved_DYCR_EE || IsBoosted_SR_EE || IsBoosted_DYCR_EE || IsBoosted_CR_EMJet ){
    weight *= trigger_sf_SingleElectron;
  }
  else if( IsResolved_SR_MM || IsResolved_SR_EM || IsResolved_DYCR_MM || IsResolved_DYCR_EM || IsBoosted_SR_MM || IsBoosted_DYCR_MM || IsBoosted_CR_MEJet ){
    weight *= trigger_sf_SingleMuon;
  }
  else{
    cerr << "[HNWRAnalyzer::executeEventFromParameter] Unkown event.." << endl;
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
  if( IsResolved_SR_EE || IsResolved_DYCR_EE || IsBoosted_SR_EE || IsBoosted_DYCR_EE ){
    weight *= ZPtReweight_EE;
  }
  else if( IsResolved_SR_MM || IsResolved_DYCR_MM || IsBoosted_SR_MM || IsBoosted_DYCR_MM ){
    weight *= ZPtReweight_MM;
  }
  else{
    //==== should be EM events
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

      JSFillHist(this_region, "NExtraLooseElectron_"+this_region, NExtraLooseElectron, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraLooseMuon_"+this_region, NExtraLooseMuon, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraLooseLepton_"+this_region, NExtraLooseLepton, weight, 10, 0., 10.);

      JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

      JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);
      JSFillHist(this_region, "HT_"+this_region, HT, weight, 4000, 0., 4000.);

      if(this_region.Contains("Boosted")){
        JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( leps_for_plot.at(0)->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);

        JSFillHist(this_region, "HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 1000, 0., 1000.);
        JSFillHist(this_region, "HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
        JSFillHist(this_region, "HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
        JSFillHist(this_region, "HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
        JSFillHist(this_region, "HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_LSF_"+this_region, HNFatJet.LSF(), weight, 100, 0., 1.);

      }

      if( leps_for_plot.size()>=2 ){
        JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).M(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*leps_for_plot.at(0)).DeltaPhi(*leps_for_plot.at(1))), weight, 40, 0., 4.);
      }

      JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

      JSFillHist(this_region, "NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
      JSFillHist(this_region, "NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
      JSFillHist(this_region, "NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);


      FillLeptonPlots(leps_for_plot, this_region, weight);
      FillJetPlots(jets, fatjets_LSF, this_region, weight);

    } // END if(pass Region)

  } // END Loop mapRegionBool

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
    cerr << "[HNWRAnalyzer::GetDYPtReweight] wrong flavour : " << flav << endl;
    exit(EXIT_FAILURE);
  }

}

std::vector<Jet> HNWRAnalyzer::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    Jet this_jet = jets.at(i);

    bool HasLeptonInside = false;

    for(unsigned int j=0; j<els.size(); j++){
      if( this_jet.DeltaR( *(els.at(j)) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    for(unsigned int j=0; j<mus.size(); j++){
      if( this_jet.DeltaR( *(mus.at(j)) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    //==== if all fine,
    out.push_back( this_jet );

  }
  return out;

}

bool HNWRAnalyzer::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cout << "[HNWRAnalyzer::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}
