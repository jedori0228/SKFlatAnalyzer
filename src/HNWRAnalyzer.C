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
/*
  //==== [TEST]
  //==== Test Which Loose/Veto electron to use..

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
*/

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
  param.Muon_Trigger_SF_Key = "Default";
  param.Muon_FR_ID = "HNWR";
  //param.Muon_FR_ID = "HNWRTrkRelIso0p6";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNWRTight";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_UseTuneP = true;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);
/*
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
*/

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

  bool PassMu50 = ev.PassTrigger("HLT_Mu50_v");
  bool PassIsoMu27 = ev.PassTrigger("HLT_IsoMu27_v");
  bool PassSingleElectron = ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v");

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     Veto_muons      = GetMuons(param.Muon_Veto_ID, param.Muon_MinPt, 2.4, param.Muon_UseTuneP);

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     Loose_muons     = GetMuons(param.Muon_Loose_ID, param.Muon_MinPt, 2.4, param.Muon_UseTuneP);

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

  bool NoExtraLepton = (n_Veto_leptons  == n_Loose_leptons);
  bool IsAllTight    = (n_Loose_leptons == n_Tight_leptons);

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

  std::vector<FatJet>   fatjets         = FatJetsVetoLeptonInside( GetFatJets(param.FatJet_ID, 200, 2.7), Tight_electrons, Tight_muons);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 20., 2.7);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(alljets, Veto_electrons, Veto_muons);

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
    "SingleMuon_Mu50",
    "SingleMuon_IsoMu27",
    "SingleElectron",
  };
  std::vector< bool > PassTriggers = {
    PassMu50           && (Loose_electrons.size()==0) && (Loose_muons.size()>=1),
    PassIsoMu27        && (Loose_electrons.size()==0) && (Loose_muons.size()>=1),
    PassSingleElectron && (Loose_electrons.size()>=1) && (Loose_muons.size()==0),
  };

  //=================
  //==== Start loop
  //=================

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    TString Suffix = Suffixs.at(it_Suffix);
    if( !PassTriggers.at(it_Suffix) ) continue;

    FillHist(Suffix+"_PassTrigger_"+param.Name, 0., 1., 1, 0., 1.);

    if(! (n_Tight_leptons>=1) ) continue;

    FillHist(Suffix+"_AtLeastOneTightLepton_"+param.Name, 0., 1., 1, 0., 1.);

    if(Suffix.Contains("SingleMuon")){

      if(Suffix.Contains("Mu50")){
        if( Loose_muons.at(0).Pt() < 52. ) continue;
      }
      if(Suffix.Contains("IsoMu27")){
        if( Loose_muons.at(0).Pt() < 29. ) continue;
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

    FatJet HNFatJet;
    Particle HNDiJet;

    Particle WRCand;

    FillHist(Suffix+"_NLepton_"+param.Name, leps.size(), 1., 5, 0., 5.);

    //==== If only one lepton, it should be boosted
    if(leps.size()==1){

      bool FoundAwayFatJet = false;
      bool FoundAwayDiJet = false;

      if(fatjets.size()>=1){

        for(unsigned int i=0; i<fatjets.size(); i++){

          if( fabs(leps[0]->DeltaPhi( fatjets.at(i) )) > 2.0 ){
            HNFatJet = fatjets.at(i);
            FoundAwayFatJet = true;
            break;
          }

        }

      }
      else if(jets.size()>=2){

        Particle temp_HNDiJet = jets.at(0)+jets.at(1);
        if( fabs(leps[0]->DeltaPhi( temp_HNDiJet )) > 2.0 ){
          HNDiJet = temp_HNDiJet;
          FoundAwayDiJet = true;
        }

      }

      FillHist(Suffix+"_NFatJetWhenOneLepton_"+param.Name, fatjets.size(), 1., 5, 0., 5.);
      if(fatjets.size()==0){
        FillHist(Suffix+"_NJetWhenOneLeptonNoFatJet_"+param.Name, jets.size(), 1., 5, 0., 5.);
      }

      if(FoundAwayFatJet){
        map_bool_To_Region["OneLepton_AwayFatJet"] = true;
        WRCand = (*leps[0])+HNFatJet;
      }
      else if(FoundAwayDiJet){
        map_bool_To_Region["OneLepton_AwayDiJet"] = true;
        WRCand = (*leps[0])+HNDiJet;
      }

    }
    else if(leps.size()==2){

      Particle Jets;
      bool IsJetFound;
      if(fatjets.size()>=1){
        TMP_map_bool_To_Region["TwoLepton_FatJet"] = true;
        WRCand = (*leps[0])+(*leps[1])+fatjets.at(0);
      }
      else if(jets.size()>=2){
        TMP_map_bool_To_Region["TwoLepton_TwoJetNoFatJet"] = true;
        WRCand = (*leps[0])+(*leps[1])+jets.at(0)+jets.at(1);
      }
      else{}


      if(jets.size()>=2){
        TMP_map_bool_To_Region["TwoLepton_TwoJet"] = true;
        WRCand = (*leps[0])+(*leps[1])+jets.at(0)+jets.at(1);
      }

    }

    //==== For TwoLeptons
		for(std::map<TString, bool>::iterator it_map = TMP_map_bool_To_Region.begin(); it_map != TMP_map_bool_To_Region.end(); it_map++){
			TString this_region = it_map->first;

      bool IsOS = (leps[0]->Charge() != leps[1]->Charge());
      if(RunCF) IsOS = !IsOS;

			if(it_map->second){
				map_bool_To_Region[this_region] = true;
				if(IsOS) map_bool_To_Region[this_region+"_OS"] = true;
				else     map_bool_To_Region[this_region+"_SS"] = true;
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

        JSFillHist(this_region, "Lepton_Size_"+this_region, leps.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        if(this_region.Contains("OneLepton_AwayFatJet")){
          JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( leps[0]->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);

          JSFillHist(this_region, "HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 1000, 0., 1000.);
          JSFillHist(this_region, "HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
          JSFillHist(this_region, "HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
          JSFillHist(this_region, "HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);

        }
        if(this_region.Contains("OneLepton_AwayDiJet")){
          JSFillHist(this_region, "HNDiJet_Mass_"+this_region, HNDiJet.M(), weight, 3000, 0., 3000.);
          JSFillHist(this_region, "dPhi_ljj_"+this_region, fabs( leps[0]->DeltaPhi(HNDiJet) ), weight, 40., 0., 4.);
        }


        if(this_region.Contains("TwoLepton")){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*leps[0])+(*leps[1])).M(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*leps[0])+(*leps[1])).Pt(), weight, 2000, 0., 2000.);
        }

        JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 1000, 0., 1000.);

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
