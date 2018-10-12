#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");

  cout << "[HNWRAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] PromptLeptonOnly = " << PromptLeptonOnly << endl;

}

void HNWRAnalyzer::executeEvent(){

  //==== FIXME

  vector<Electron> T_els = GetElectrons("HNWRTight", 10., 2.5);
  vector<Electron> L_els = GetElectrons("passLooseID", 10., 2.5);
  vector<Electron> V_els = GetElectrons("passVetoID", 10., 2.5);
  vector<Electron> LNoIso_els = GetElectrons("HNWRLoose", 10., 2.5);
  vector<Electron> VNoIso_els = GetElectrons("HNWRVeto", 10., 2.5);

  for(unsigned int i=0;i<L_els.size(); i++){
    FillHist("Loose_Electron_Pt", L_els.at(i).Pt(), 1., 50, 0., 500.);
    if( L_els.at(i).passHEEPID() ){
      FillHist("Loose_Pass_Tight_Electron_Pt", L_els.at(i).Pt(), 1., 50, 0., 500.);
    }
  }
  for(unsigned int i=0;i<LNoIso_els.size(); i++){
    FillHist("LooseNoIso_Electron_Pt", LNoIso_els.at(i).Pt(), 1., 50, 0., 500.);
    if( LNoIso_els.at(i).passHEEPID() ){
      FillHist("LooseNoIso_Pass_Tight_Electron_Pt", LNoIso_els.at(i).Pt(), 1., 50, 0., 500.);
    }
  }

  for(unsigned int i=0;i<V_els.size(); i++){
    FillHist("Veto_Electron_Pt", V_els.at(i).Pt(), 1., 50, 0., 500.);
    if( V_els.at(i).passHEEPID() ){
      FillHist("Veto_Pass_Tight_Electron_Pt", V_els.at(i).Pt(), 1., 50, 0., 500.);
    }
  }
  for(unsigned int i=0;i<VNoIso_els.size(); i++){
    FillHist("VetoNoIso_Electron_Pt", VNoIso_els.at(i).Pt(), 1., 50, 0., 500.);
    if( VNoIso_els.at(i).passHEEPID() ){
      FillHist("VetoNoIso_Pass_Tight_Electron_Pt", VNoIso_els.at(i).Pt(), 1., 50, 0., 500.);
    }
  }
  return;


  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();
  //PrintGen(gens); return;

  //========================
  //==== AnalyzerParameter
  //========================

  AnalyzerParameter param;

  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Tight_RelIso = 0.15;
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNWRVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_FR_ID = "HNWR";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNWR";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Tight_RelIso = 0.15;
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "HNWRVeto";
  param.Muon_Veto_RelIso = 0.6;
  param.Muon_FR_ID = "HNWR";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNWR";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

  //==== Following EXO-17-011

  param.Clear();

  param.Name = "EXO17011";

  param.Electron_Tight_ID = "passHEEPID";
  param.Electron_Loose_ID = "passHEEPID";
  param.Electron_Veto_ID = "passHEEPID";
  param.Electron_ID_SF_Key = "Default";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 50.;

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Veto_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_Trigger_SF_Key = "Default";
  param.Muon_UseTuneP = true;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);


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

  bool PassSingleMuon = ev.PassTrigger("HLT_IsoMu27_v");
  bool PassSingleElectron = ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v");
  bool PassSingleMuonEXO17011 = ev.PassTrigger("HLT_Mu50_v");

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     Veto_muons      = GetHighPtMuons(param.Muon_Veto_ID, param.Muon_MinPt, 2.4);

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     Loose_muons     = GetHighPtMuons(param.Muon_Loose_ID, param.Muon_MinPt, 2.4);

  if(PromptLeptonOnly){
    Loose_electrons = ElectronPromptOnly(Loose_electrons, gens);
    Loose_muons = MuonPromptOnly(Loose_muons, gens);
  }

  std::vector<Electron> Tight_electrons;
  std::vector<Muon>     Tight_muons;

  for(unsigned int i=0; i<Loose_electrons.size(); i++){

    //==== Is Tight?
    if(Loose_electrons.at(i).PassID(param.Electron_Tight_ID)) Tight_electrons.push_back( Loose_electrons.at(i) );

  }
  for(unsigned int i=0; i<Loose_muons.size(); i++){

    //==== Is Tight?
    if(Loose_muons.at(i).PassID(param.Muon_Tight_ID)) Tight_muons.push_back( Loose_muons.at(i) );

  }

  int n_Veto_leptons = Veto_electrons.size()+Veto_muons.size();
  int n_Loose_leptons = Loose_electrons.size()+Loose_muons.size();
  int n_Tight_leptons = Tight_electrons.size()+Tight_muons.size();

  bool HasCFElectron = false;
  if(!IsDATA){
    for(unsigned int i=0; i<Loose_electrons.size(); i++){
      Gen genlep = GetGenMatchedLepton(Loose_electrons.at(i) , gens);
      if(genlep.Charge()!=Loose_electrons.at(i).Charge()){
        HasCFElectron = true;
        break;
      }
    }
  }

  bool NoExtraLepton = (n_Veto_leptons==n_Loose_leptons);
  bool IsAllTight = (n_Loose_leptons==n_Tight_leptons);

  //==== Veto Extra Lepton
  if(!NoExtraLepton) return;

  FillHist("NoExtraLepton_"+param.Name, 0., 1., 1, 0., 1.);

  //==== Loose sample or not
  if(RunFake){
    if(IsAllTight) return;
  }
  else{
    if(!IsAllTight) return;
  }

  FillHist("AllTight_"+param.Name, 0., 1., 1, 0., 1.);

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = GetFatJets(param.FatJet_ID, 200, 2.7);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 20., 2.7);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(JetsAwayFromFatJet(alljets, fatjets), Veto_electrons, Veto_muons);

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
    "SingleMuon", // at least two muons
    "SingleElectron", // at least two muons
    "SingleMuonEXO17011", // at least two muons
    //"SingleElectronEXO17011", // at least two muons
  };
  std::vector< bool > PassTriggers = {
    PassSingleMuon && (Loose_electrons.size()==0) && (Loose_muons.size()>=1),
    PassSingleMuon && (Loose_electrons.size()>=1) && (Loose_muons.size()==0),
    PassSingleMuonEXO17011 && (Loose_electrons.size()==0) && (Loose_muons.size()>=1),
  };

  //=================
  //==== Start loop
  //=================

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    TString Suffix = Suffixs.at(it_Suffix);
    if( !PassTriggers.at(it_Suffix) ) continue;

    if(Suffix.Contains("SingleMuon")){

      bool HasTightIsoMuon = false;
      for(unsigned int i=0; i<Loose_muons.size(); i++){
        if(Loose_muons.at(i).RelIso()<0.15){
          HasTightIsoMuon = true;
          break;
        }
      }
      if(!HasTightIsoMuon){
        FillHist("SingleMuon_PassTightIso_"+param.Name, 0., 1., 1, 0., 1.);
        continue;
      }

      if( Loose_muons.at(0).Pt() < 29. ) continue;
      if(Suffix.Contains("EXO17011")){
        if( Loose_muons.at(0).Pt() < 55. ) continue;
      }
    }
    else if(Suffix.Contains("SingleElectron")){
      if( Loose_electrons.at(0).Pt() < 38. ) continue;
    }
    else{

    }

    if(this->IsDATA){
      if(this->DataStream == "SingleMuon"){
        if(!( Suffix.Contains("SingleMuon") )) continue;
      }
      else if(this->DataStream == "SingleElectron"){
        if(!( Suffix.Contains("SingleElectron") )) continue;
      }
      else{

      }
    }

    std::vector<Lepton *> leps_el, leps_mu;
    std::vector<Lepton *> leps;
    leps_el = MakeLeptonPointerVector(Loose_electrons);
    leps_mu = MakeLeptonPointerVector(Loose_muons);
    for(unsigned int i=0; i<leps_el.size(); i++) leps.push_back( leps_el.at(i) );
    for(unsigned int i=0; i<leps_mu.size(); i++) leps.push_back( leps_mu.at(i) );

    double weight = 1.;
    if(!IsDATA){
      weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();

      mcCorr.IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

      for(unsigned int i=0; i<Loose_electrons.size(); i++){
        double this_recosf = mcCorr.ElectronReco_SF(Loose_electrons.at(i).scEta(),Loose_electrons.at(i).Pt());
        double this_idsf = mcCorr.ElectronID_SF(param.Electron_ID_SF_Key, Loose_electrons.at(i).scEta(), Loose_electrons.at(i).Pt());
        weight *= this_recosf*this_idsf;
      }
      for(unsigned int i=0; i<Loose_muons.size(); i++){
        double this_idsf  = mcCorr.MuonID_SF (param.Muon_ID_SF_Key,  Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_isosf = mcCorr.MuonISO_SF(param.Muon_ISO_SF_Key, Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_trigsf = mcCorr.MuonTrigger_SF(param.Muon_Trigger_SF_Key, "IsoMu27", Loose_muons);

        weight *= this_idsf*this_isosf*this_trigsf;
      }

    }
    else{

      if(RunFake){
        weight = fakeEst.GetWeight(leps, param);
        if(! (fakeEst.HasLooseLepton) ){
          cout << "--> WTF" << endl;
          exit(EXIT_FAILURE);
        }
      }

      if(RunCF){
        weight = cfEst.GetWeight(leps, param);
      }

    }

    std::map<TString, bool> TMP_map_bool_To_Region; // For SS/OS
    std::map<TString, bool> map_bool_To_Region;

    Particle WRCand;

    //==== One Lepton; Use IsOneLeptonEvent
    bool IsAwayFatJetEvent = false;

    FatJet HNJet;
    if(fatjets.size()>=1){

      //==== # of lepton can be 1 or 2
      //==== If we have fatjets in the event,
      //==== pick the leading lepton as a tag,
      //==== and see if there is a away fatjet.
      //==== Fatjet with higher pt has higher priority.
      //==== IF we have two muons,
      //==== that subleading muon should be inside the fatjet.

      for(unsigned int i=0; i<fatjets.size(); i++){
        if( fabs(leps[0]->DeltaPhi( fatjets.at(i) )) > 2.0 ){
          HNJet = fatjets.at(i);
          IsAwayFatJetEvent = true;
          break;
        }
      }
      if(leps.size()==2){
        if(leps[1]->DeltaR( HNJet ) > 0.8) IsAwayFatJetEvent = false;
      }

      if(IsAwayFatJetEvent){

        map_bool_To_Region["SingleLepton_WithFatJet"] = true;
        WRCand = (*leps[0])+HNJet;

      }
    }


    //==== Two Lepton; Use IsTwoLeptonEvent
    bool IsTwoLeptonEvent = (leps.size()==2);
    bool IsOS = false;
    Particle ZCand_IsTwoLeptonEvent;

    if(IsTwoLeptonEvent){

      TMP_map_bool_To_Region["DiLepton"] = true;

      ZCand_IsTwoLeptonEvent = (*leps[0])+(*leps[1]); // Only works for two lepton case
      IsOS = (leps[0]->Charge() != leps[1]->Charge()); // Only works for two lepton case
      if(RunCF){
        if(!IsOS) continue;
        IsOS = !IsOS;
      }

      if(!IsAwayFatJetEvent && (jets.size()>=2)){
        TMP_map_bool_To_Region["DiLepton_TwoJetNoFatJet"] = true;
        WRCand = (*leps[0])+(*leps[1])+jets.at(0)+jets.at(1);
      }

      for(std::map<TString, bool>::iterator it_map = TMP_map_bool_To_Region.begin(); it_map != TMP_map_bool_To_Region.end(); it_map++){
        TString this_region = it_map->first;

        if(it_map->second){
          map_bool_To_Region[this_region] = true;
          if(IsOS) map_bool_To_Region[this_region+"_OS"] = true;
          else     map_bool_To_Region[this_region+"_SS"] = true;
        }

      }

    }

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = (param.Name)+"_"+Suffix+"_"+this_region;

      if(it_map->second){

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        if(IsAwayFatJetEvent){
          JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( leps[0]->DeltaPhi(HNJet) ), weight, 40., 0., 4.);

          JSFillHist(this_region, "HNJet_Pt_"+this_region, HNJet.Pt(), weight, 1000, 0., 1000.);
          JSFillHist(this_region, "HNJet_Eta_"+this_region, HNJet.Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "HNJet_Mass_"+this_region, HNJet.M(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNJet_SDMass_"+this_region, HNJet.SDMass(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNJet_PuppiTau21_"+this_region, HNJet.PuppiTau2()/HNJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNJet_PuppiTau31_"+this_region, HNJet.PuppiTau3()/HNJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNJet_PuppiTau32_"+this_region, HNJet.PuppiTau3()/HNJet.PuppiTau2(), weight, 100, 0., 1.);

        }
        if(IsTwoLeptonEvent){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ZCand_IsTwoLeptonEvent.M(), weight, 2000, 0., 2000.);
        }

        JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

        FillLeptonPlots(leps, this_region, weight);
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
