#include "HNWRSignalStudy.h"

void HNWRSignalStudy::initializeAnalyzer(){

}

void HNWRSignalStudy::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void HNWRSignalStudy::executeEventFromParameter(AnalyzerParameter param){

  Event ev = GetEvent();
  FillHist("NoCut", 0., 1., 1, 0., 1.);
  vector<Gen> gens = GetGens();

  //==== Find Gen objects
  Gen gen_WR, gen_N, gen_priLep, gen_secLep, gen_jet1, gen_jet2;
  int gen_N_PID = -1;
  bool gen_SignalLeptonChannel = -1; // 0 : ee, 1 : mm
  bool GenAllFound = true;
  bool IsOffShellWR = false;

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if(abs( gen.PID() ) == 9900024 || abs( gen.PID() ) == 34){

     //=== pythia sample has strange things. first WR is nan
     double forchecknan = gen.M();
     if(forchecknan != forchecknan) continue;

      gen_WR = gen;
      break;
    }
  }
  if(gen_WR.IsEmpty()){
    IsOffShellWR = true;
    cout << "Can't file gen_WR" << endl;
    PrintGen(gens);
    FillHist("GENFIND_no_gen_WR", 0., 1., 1, 0., 1.);
    GenAllFound = false;
  }
  FillHist("M_gen_WR", gen_WR.M(), 1., 6000, 0., 6000.);

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if(abs( gen.PID() ) == 9900012 || abs( gen.PID() ) == 9900014 ){
      gen_N_PID = gen.PID();
      gen_N = gen;
      if(abs( gen.PID() ) == 9900012) gen_SignalLeptonChannel = 0;
      else if(abs( gen.PID() ) == 9900014) gen_SignalLeptonChannel = 1;
      else gen_SignalLeptonChannel = 999;
      break;
    }
  }
  if(gen_N.IsEmpty()){
    cout << "Can't file gen_N" << endl;
    PrintGen(gens);
    FillHist("GENFIND_no_gen_N", 0., 1., 1, 0., 1.);
    GenAllFound = false;
  }
  FillHist("M_gen_N", gen_N.M(), 1., 6000, 0., 6000.);
  FillHist("gen_SignalLeptonChannel", gen_SignalLeptonChannel, 1., 2, 0., 2.);


  if(!IsOffShellWR){
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at(gen.MotherIndex());
      if(abs( mother.PID() ) == 9900024 || abs( mother.PID() ) == 34 ){
        if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
          gen_priLep = gen;
          break;
        }
      }
    }
  }
  else{
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at(gen.MotherIndex());
      if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
        gen_priLep = gen;
        break;
      }
    }
  }
  if(gen_priLep.IsEmpty()){
    cout << "Can't file gen_priLep" << endl;
    PrintGen(gens);
    FillHist("GENFIND_no_gen_priLep", 0., 1., 1, 0., 1.);
    GenAllFound = false;
  }

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    Gen mother = gens.at(gen.MotherIndex());
    if(abs( mother.PID() ) == abs( gen_N_PID )){
      if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
        gen_secLep = gen;
        break;
      }
    }
  }
  if(gen_secLep.IsEmpty()){
    cout << "Can't file gen_secLep" << endl;
    PrintGen(gens);
    FillHist("GENFIND_no_gen_secLep", 0., 1., 1, 0., 1.);
    GenAllFound = false;
  }

  vector<Gen> tmp_gen_jets; //gen_jet1, genjet2;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    Gen mother = gens.at(gen.MotherIndex());
    if(abs( mother.PID() ) == abs( gen_N_PID )){
      if(abs( gen.PID() ) <= 6){
        tmp_gen_jets.push_back( gen );
      }
    }
  }
  if(tmp_gen_jets.size()!=2){
    cout << "Can't file correct jets : tmp_gen_jets.size() = tmp_gen_jets" << endl;
    PrintGen(gens);
    FillHist("GENFIND_size_tmp_gen_jets", tmp_gen_jets.size(), 1., 10, 0., 10.);
    GenAllFound = false;
  }

/*
  //=== 190113_ElectronTriggerTest

  if(gen_SignalLeptonChannel!=0) return;
  vector<Electron> els = GetElectrons("NOCUT", 50., 2.4);

  if(els.size()==0) return;

  Electron el_0 = els.at(0);

  if(el_0.Pt()<=60.) return;
  FillHist("AtLeastOnePt60Electron", 0., 1., 1, 0., 1.);

  if(! (el_0.passHEEPID()) ) return;
  FillHist("LeadingElectronPassHEEP", 0., 1., 1, 0., 1.);

  bool IsSubLeadingPassLoose = false;
  if( els.size()>=2 ){
    if( els.at(1).Pass_CutBasedLooseNoIso() && els.at(1).Pt() >= 53. ) IsSubLeadingPassLoose = true;
  }

  vector< vector<TString> > triggers;

  if(DataYear==2016){
    triggers  = {
      {"HLT_Ele27_WPTight_Gsf_v"},
      {"HLT_Photon175_v"},
      {"HLT_Ele27_WPTight_Gsf_v", "HLT_Photon175_v"},
      {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"},
      {"HLT_DoubleEle33_CaloIdL_MW_v"},
      {"HLT_DoublePhoton60_v"},
      {"HLT_DoubleEle33_CaloIdL_MW_v", "HLT_DoublePhoton60_v"},
    };
  }
  else if(DataYear==2017){
    triggers  = {
      {"Ele35_WPTight_Gsf"},
      {"HLT_Photon200_v"},
      {"Ele35_WPTight_Gsf", "HLT_Photon200_v"},
      {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"},
      {"HLT_DoubleEle33_CaloIdL_MW_v"},
      {"HLT_DoublePhoton70_v"},
      {"HLT_DoubleEle33_CaloIdL_MW_v", "HLT_DoublePhoton70_v"},
    };
  }

  for(unsigned int i=0; i<triggers.size(); i++){
    vector<TString> trigger = triggers.at(i);

    if(! (ev.PassTrigger(trigger)) ) continue;

    FillHist("Case1_TriggerFirst__PassTriggers", i, 1., 10, 0., 10.);

    if(IsSubLeadingPassLoose){
      FillHist("Case1_TriggerFirst__SecondElectron", i, 1., 10, 0., 10.);
    }

  }

  if(IsSubLeadingPassLoose){
    FillHist("Case2_TriggerFirst__SecondElectron", 0., 1., 1, 0., 1.);

    for(unsigned int i=0; i<triggers.size(); i++){
      vector<TString> trigger = triggers.at(i);

      if(! (ev.PassTrigger(trigger)) ) continue;

      FillHist("Case2_TriggerFirst__PassTriggers", i, 1., 10, 0., 10.);

    }

  }
*/

  //==========
  //=== MAIN
  //==========

  //==== # of LSF jets

  vector<FatJet> fatjets_LSF = GetFatJets("HNLSF", 200., 2.4);

  //==== Gen All Found
  if(!GenAllFound) return;

  //==== Gen gen_WR, gen_N, gen_priLep, gen_secLep, gen_jet1, genjet2;
  FillHist("GenStudy__dR_gen_N_gen_secLep", gen_N.DeltaR(gen_secLep), 1., 60, 0., 6.);
  Particle gen_twojets = tmp_gen_jets.at(0)+tmp_gen_jets.at(1);
  FillHist("GenStudy__dR_gen_twojets_gen_secLep", gen_twojets.DeltaR(gen_secLep), 1., 60, 0., 6.);
  Particle gen_secLep_gen_twojets = gen_secLep+gen_twojets;
  FillHist("GenStudy__M_gen_secLep_gen_twojets", (gen_secLep_gen_twojets).M(), 1., 6000, 0., 6000.);
  FillHist("GenStudy__dR_gen_jets", tmp_gen_jets.at(0).DeltaR( tmp_gen_jets.at(1) ), 1., 60, 0., 6.);
  FillHist("GenStudy__dR_gen_jets_gen_secLep", tmp_gen_jets.at(0).DeltaR( gen_secLep ), 1., 60, 0., 6.);
  FillHist("GenStudy__dR_gen_jets_gen_secLep", tmp_gen_jets.at(1).DeltaR( gen_secLep ), 1., 60, 0., 6.);

  //==== RECO vs GEN matching
  TString GENLepChannel = "";
  vector<Lepton> leps;
  vector<Lepton> Loose_leps, Tight_leps;
  vector<Lepton> NoIso_leps;
  std::vector<Electron> NoIso_electrons = GetElectrons("HNWRNoIso", 50., 2.5);
  std::vector<Muon> NoIso_muons = GetMuons("HNWRNoIso", 50., 2.4);
  if(gen_SignalLeptonChannel==0){
    GENLepChannel = "EE";
    vector<Electron> els = GetAllElectrons();
    for(unsigned int i=0; i<els.size(); i++){
      Lepton lep(els.at(i));
      if(lep.Pt()<53) continue;
      lep.SetRelIso( els.at(i).TrkIso()/els.at(i).Pt() );
      leps.push_back( lep );
      if( els.at(i).PassID("HNWRTight") ){
        Tight_leps.push_back( lep );
      }
      if( els.at(i).PassID("HNWRNoIso") ){
        Loose_leps.push_back( lep );
      }
    }
    for(unsigned int i=0; i<NoIso_electrons.size(); i++){
      Lepton lep(NoIso_electrons.at(i));
      if(lep.Pt()<50) continue;
      if(NoIso_electrons.at(i).PassID("HNWRLoose")) continue;
      NoIso_leps.push_back( lep );
    }
  }
  else if(gen_SignalLeptonChannel==1){
    GENLepChannel = "MuMu";
    vector<Muon> mus = GetAllMuons();
    for(unsigned int i=0; i<mus.size(); i++){
      Lepton lep(mus.at(i));
      if(lep.Pt()<53) continue;
      lep.SetRelIso( mus.at(i).TrkIso()/mus.at(i).TuneP4().Pt() );
      leps.push_back( lep );
      if( mus.at(i).PassID("HNWRTight") ){
        Tight_leps.push_back( lep );
      } 
      if( mus.at(i).PassID("POGLoose") ){
        Loose_leps.push_back( lep );
      }
    }
    for(unsigned int i=0; i<NoIso_muons.size(); i++){
      Lepton lep(NoIso_muons.at(i));
      if(lep.Pt()<50) continue;
      if(NoIso_muons.at(i).PassID("HNWRLoose")) continue;
      NoIso_leps.push_back( lep );
    }
  }
  JSFillHist(GENLepChannel, "NoCut_"+GENLepChannel, 0., 1., 1, 0., 1.);
  JSFillHist(GENLepChannel, "NoCut__NTightLep_"+GENLepChannel, Tight_leps.size(), 1., 5, 0., 5.);
  JSFillHist(GENLepChannel, "FatJet_LSF_Size_"+GENLepChannel, fatjets_LSF.size(), 1., 10, 0., 10.);

  double dummy_dR = 0.15;

  Lepton lep_mathced_gen_priLep;
  bool Found_lep_mathced_gen_priLep = false;
  for(unsigned int i=0; i<leps.size(); i++){
    Lepton lep = leps.at(i);
    if( gen_priLep.DeltaR( lep ) < dummy_dR ){
      lep_mathced_gen_priLep = lep;
      dummy_dR = gen_priLep.DeltaR( lep );
      Found_lep_mathced_gen_priLep = true;
    }
  }
  if(!Found_lep_mathced_gen_priLep) return;
  JSFillHist(GENLepChannel, "GenStudy__lep_mathced_gen_priLep__RelIso_"+GENLepChannel, lep_mathced_gen_priLep.RelIso(), 1., 100, 0., 1.);

  Lepton lep_mathced_gen_secLep;
  bool Found_lep_mathced_gen_secLep = false;
  dummy_dR = 0.15;
  for(unsigned int i=0; i<Loose_leps.size(); i++){
    Lepton lep = Loose_leps.at(i);
    if( gen_secLep.DeltaR( lep ) < dummy_dR ){
      lep_mathced_gen_secLep = lep;
      dummy_dR = gen_secLep.DeltaR( lep );
      Found_lep_mathced_gen_secLep = true;
    }
  }
  if(Found_lep_mathced_gen_secLep){
    JSFillHist(GENLepChannel, "GenStudy__lep_mathced_gen_secLep__RelIso_"+GENLepChannel, lep_mathced_gen_secLep.RelIso(), 1., 100, 0., 1.);
  }

  vector<Jet> jets = GetJets("HN", 40., 2.4);
  vector<Jet> jet_matched_gen_jets;
  for(int j=0; j<2; j++){
    Gen this_gen_jet = tmp_gen_jets.at(j);

    dummy_dR = 0.15;
    Jet jet_matched_gen_jet;
    bool this_found = false;
    for(unsigned int i=0; i<jets.size(); i++){
      Jet jet = jets.at(i);
      if( this_gen_jet.DeltaR( jet ) < dummy_dR ){
        this_found = true;
        jet_matched_gen_jet = jet;
        dummy_dR = this_gen_jet.DeltaR( jet );
      }
    }
    if(this_found){
      jet_matched_gen_jets.push_back( jet_matched_gen_jet );
    }

  }

  if(Found_lep_mathced_gen_secLep && jet_matched_gen_jets.size()==2){

    //==== Max
    double dR_ak4_max = max( lep_mathced_gen_secLep.DeltaR( jet_matched_gen_jets.at(0) ), lep_mathced_gen_secLep.DeltaR( jet_matched_gen_jets.at(1) ) );
    JSFillHist(GENLepChannel, "GenStudy__MaxdR_matchedAK4_secLep_"+GENLepChannel, dR_ak4_max, 1., 60, 0., 6.);

    //==== find matched AK4 
    dummy_dR = 0.15;
    Jet jet_matched_secLep;
    bool Found_jet_matched_secLep = false;
    for(unsigned int j=0; j<jets.size(); j++){
      if( lep_mathced_gen_secLep.DeltaR( jets.at(j) ) < dummy_dR ){
        Found_jet_matched_secLep = true;
        jet_matched_secLep = jets.at(j);
        dummy_dR = lep_mathced_gen_secLep.DeltaR( jets.at(j) );
      }
    }
    if(Found_jet_matched_secLep){
      JSFillHist(GENLepChannel, "GenStudy__dR_matchedAK4_secLep_"+GENLepChannel, jet_matched_secLep.DeltaR( lep_mathced_gen_secLep ), 1., 60, 0., 0.15);
    }

    double this_max_dR = -1;
    double this_min_dR = 9999;
    for(unsigned int j=0; j<jets.size(); j++){
      JSFillHist(GENLepChannel, "GenStudy__dR_AK4_secLep_"+GENLepChannel, jets.at(j).DeltaR( lep_mathced_gen_secLep ), 1., 60, 0., 6.);
      this_max_dR = max(this_max_dR, jets.at(j).DeltaR( lep_mathced_gen_secLep ));
      this_min_dR = min(this_max_dR, jets.at(j).DeltaR( lep_mathced_gen_secLep ));
    }
    JSFillHist(GENLepChannel, "GenStudy__MaxdR_AK4_secLep_"+GENLepChannel, this_max_dR, 1., 60, 0., 6.);
    JSFillHist(GENLepChannel, "GenStudy__MindR_AK4_secLep_"+GENLepChannel, this_min_dR, 1., 100, 0., 1.);


  }


  vector<FatJet> fatjets = GetFatJets("tight", 200., 2.4);
  FatJet fatjet_matched_gen_N;
  dummy_dR = 0.15;
  bool Found_fatjet_matched_gen_N = false;
  for(unsigned int i=0; i<fatjets.size(); i++){
    FatJet fatjet = fatjets.at(i);
    if( gen_N.DeltaR( fatjet ) < dummy_dR ){
      fatjet_matched_gen_N = fatjet;
      Found_fatjet_matched_gen_N = true;
      dummy_dR = gen_N.DeltaR( fatjet );
    }
  }
  if(Found_fatjet_matched_gen_N){
    JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__NotEmpty_"+GENLepChannel, 0., 1., 1, 0., 1.);
    JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__SDMass_"+GENLepChannel, fatjet_matched_gen_N.SDMass(), 1., 6000, 0., 6000.);
    JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__LSF_"+GENLepChannel, fatjet_matched_gen_N.LSF(), 1., 100, 0., 1.);
    double reldpf = fabs( fatjet_matched_gen_N.Pt() - gen_N.Pt() )/ gen_N.Pt();
    JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__reldpt_"+GENLepChannel, reldpf, 1., 200, 0., 2.);
    JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__dRgen_"+GENLepChannel, fatjet_matched_gen_N.DeltaR(gen_N), 1., 60, 0., 6.);
    if(Found_lep_mathced_gen_secLep) JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__dR_lep_mathced_gen_secLep_"+GENLepChannel, fatjet_matched_gen_N.DeltaR(lep_mathced_gen_secLep), 1., 60, 0., 6.);

    bool PassUMNTag = (fatjet_matched_gen_N.SDMass() > 40.) && (fatjet_matched_gen_N.LSF() > 0.7);;
    if(PassUMNTag){
      JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__PassUMNTag_"+GENLepChannel, 0., 1., 1, 0., 1.);

      //==== Fast vs Full

      JSFillHist(GENLepChannel, "GenStudy__PassUMNTag__lep_mathced_gen_priLep__Pt_"+GENLepChannel, lep_mathced_gen_priLep.Pt(), 1., 1000, 0., 1000.);
      if(Found_lep_mathced_gen_secLep){
        JSFillHist(GENLepChannel, "GenStudy__PassUMNTag__lep_mathced_gen_secLep__Pt_"+GENLepChannel, lep_mathced_gen_secLep.Pt(), 1., 1000, 0., 1000.);
        JSFillHist(GENLepChannel, "GenStudy__PassUMNTag__lep_mathced_gen_secLep_Found_"+GENLepChannel, 0., 1., 1, 0., 1.);
      }

    }
    else{
      JSFillHist(GENLepChannel, "GenStudy__FailUMNTag__NTightLep_"+GENLepChannel, Tight_leps.size(), 1., 5, 0., 5.);
      JSFillHist(GENLepChannel, "GenStudy__FailUMNTag__dR_gen_jets_"+GENLepChannel, tmp_gen_jets.at(0).DeltaR( tmp_gen_jets.at(1) ), 1., 60, 0., 6.);
    }

    bool PassSNUTag = false;
    for(unsigned int i=0; i<NoIso_leps.size(); i++){
      Lepton lep = NoIso_leps.at(i);
      if(lep.Pt()>=53.){
        if( fatjet_matched_gen_N.DeltaR( lep ) < 0.8 ){
          JSFillHist(GENLepChannel, "GenStudy__SNUTagLepton__RelIso_"+GENLepChannel, lep.RelIso(), 1., 100, 0., 1.);
          JSFillHist(GENLepChannel, "GenStudy__SNUTagLepton__Iso_"+GENLepChannel, lep.RelIso()*lep.Pt(), 1., 1000, 0., 1000.);
          PassSNUTag = true;
        }
      }
    }
    PassSNUTag &= (fatjet_matched_gen_N.SDMass() > 40.);
    if(PassSNUTag){
      JSFillHist(GENLepChannel, "GenStudy__fatjet_matched_gen_N__PassSNUTag_"+GENLepChannel, 0., 1., 1, 0., 1.);
    }

  } // FatJet Study


}

HNWRSignalStudy::HNWRSignalStudy(){

}

HNWRSignalStudy::~HNWRSignalStudy(){

}


