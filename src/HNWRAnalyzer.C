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

  bool PassMu50 = ev.PassTrigger("HLT_Mu50_v");
  bool PassIsoMu27 = ev.PassTrigger("HLT_IsoMu27_v");
  bool PassSingleElectron = ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v");

  //==============
  //==== Leptons
  //==============

  std::vector<Electron> Veto_electrons = SelectElectrons(AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> Veto_muons = SelectMuons(AllTunePMuons, param.Muon_Veto_ID, 10., 2.4);

  std::vector<Electron> Loose_electrons = SelectElectrons(AllElectrons, param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon> Loose_muons = SelectMuons(AllTunePMuons, param.Muon_Loose_ID, param.Muon_MinPt, 2.4);

  if(PromptLeptonOnly){
    Loose_electrons = ElectronPromptOnly(Loose_electrons, gens);
    Loose_muons = MuonPromptOnly(Loose_muons, gens);
  }
  //==== Sorting
  std::sort(Loose_electrons.begin(), Loose_electrons.end(), PtComparing);
  std::sort(Loose_muons.begin(), Loose_muons.end(), PtComparing);

  std::vector<Electron> NoIso_electrons = SelectElectrons(AllElectrons, "HNWRNoIso", 50., 2.5);
  std::vector<Muon> NoIso_muons = SelectMuons(AllMuons, "HNWRNoIso", 50., 2.4);

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

  FillHist("n_Tight_electrons", Tight_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_electrons", Loose_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Veto_electrons", Veto_electrons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_muons", Tight_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Loose_muons", Loose_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Veto_muons", Veto_muons.size(), 1., 5, 0., 5.);
  FillHist("n_Tight_leptons", n_Tight_leptons, 1., 5, 0., 5.);
  FillHist("n_Loose_leptons", n_Loose_leptons, 1., 5, 0., 5.);
  FillHist("n_Veto_leptons", n_Veto_leptons, 1., 5, 0., 5.);

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

  std::vector<FatJet>   fatjets         = GetFatJets(param.FatJet_ID, 200, 2.4);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 40., 2.4);
  std::sort(alljets.begin(), alljets.end(), PtComparing);
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

    //if(! (n_Tight_leptons>=1) ) continue;
    //FillHist(Suffix+"_AtLeastOneTightLepton_"+param.Name, 0., 1., 1, 0., 1.);

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

    std::vector<Lepton *> leps_NoIso_el, leps_NoIso_mu;
    std::vector<Lepton *> leps_NoIso;
    leps_NoIso_el = MakeLeptonPointerVector(NoIso_electrons);
    leps_NoIso_mu = MakeLeptonPointerVector(NoIso_muons);
    for(unsigned int i=0; i<leps_NoIso_el.size(); i++) leps_NoIso.push_back( leps_NoIso_el.at(i) );
    for(unsigned int i=0; i<leps_NoIso_mu.size(); i++) leps_NoIso.push_back( leps_NoIso_mu.at(i) );

    double weight = 1.;
    if(!IsDATA){
      weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight()*weight_Prefire;

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
    Particle NCand;
    Particle NCand_1, NCand_2;

    FillHist(Suffix+"_NLepton_"+param.Name, leps.size(), 1., 5, 0., 5.);

    //==== If only one lepton, it should be boosted
    if(leps.size()==1){

      bool FoundAwayFatJet = false;
      bool FoundAwayFatJetWithLepton = false;
      bool FoundAwayDiJet = false;

      if(fatjets.size()>=1){

        for(unsigned int i=0; i<fatjets.size(); i++){

          if( fabs(leps[0]->DeltaPhi( fatjets.at(i) )) > 2.0 ){
            HNFatJet = fatjets.at(i);
            FoundAwayFatJet = true;


            for(unsigned int j=0; j<leps_NoIso.size(); j++){
              if(HNFatJet.DeltaR( *(leps_NoIso.at(j)) ) < 0.8){
                FoundAwayFatJetWithLepton = true;
                break;
              }
            }

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
        NCand = HNFatJet;
        NCand_1 = HNFatJet;
        NCand_2 = HNFatJet;

       if(FoundAwayFatJetWithLepton){
         map_bool_To_Region["OneLepton_AwayFatJetWithLepton"] = true;
         if(leps[0]->Pt() > 100.){
           map_bool_To_Region["OneLepton_AwayFatJetWithLepton100GeV"] = true;
         }
       }

        if(WRCand.Pt()>200.){
          map_bool_To_Region["OneLepton_AwayFatJet_WRCandPtgt200"] = true;
        }
        else{
          map_bool_To_Region["OneLepton_AwayFatJet_WRCandPtlt200"] = true;
          if(FoundAwayFatJetWithLepton){
            map_bool_To_Region["OneLepton_AwayFatJetWithLepton_WRCandPtlt200"] = true;
            if(leps[0]->Pt() > 100.){
              map_bool_To_Region["OneLepton_AwayFatJetWithLepton100GeV_WRCandPtlt200"] = true;
            }
          }
        }

      }
      else if(FoundAwayDiJet){
        map_bool_To_Region["OneLepton_AwayDiJet"] = true;
        WRCand = (*leps[0])+HNDiJet;
        NCand = HNDiJet;
        NCand_1 = HNDiJet;
        NCand_2 = HNDiJet;
      }

    }
    else if(leps.size()==2){

      double m_ll = ( (*leps[0])+(*leps[1]) ).M();

      if(m_ll>15.){

        Particle Jets;
        bool IsJetFound;
        if(fatjets.size()>=1){
          TMP_map_bool_To_Region["TwoLepton_FatJet"] = true;
          WRCand = (*leps[0])+(*leps[1])+fatjets.at(0);
          NCand = (*leps[1])+fatjets.at(0);
          NCand_1 = (*leps[0])+fatjets.at(0);
          NCand_2 = (*leps[1])+fatjets.at(0);
        }
        else if(jets.size()>=2){
          TMP_map_bool_To_Region["TwoLepton_TwoJetNoFatJet"] = true;
          WRCand = (*leps[0])+(*leps[1])+jets.at(0)+jets.at(1);
          NCand = (*leps[1])+jets.at(0)+jets.at(1);
          NCand_1 = (*leps[0])+jets.at(0)+jets.at(1);
          NCand_2 = (*leps[1])+jets.at(0)+jets.at(1);
        }
        else{}


        if(jets.size()>=2){
          TMP_map_bool_To_Region["TwoLepton_TwoJet"] = true;
          WRCand = (*leps[0])+(*leps[1])+jets.at(0)+jets.at(1);
          NCand = (*leps[1])+jets.at(0)+jets.at(1);
          NCand_1 = (*leps[0])+jets.at(0)+jets.at(1);
          NCand_2 = (*leps[1])+jets.at(0)+jets.at(1);

          if(m_ll>150.){
            TMP_map_bool_To_Region["TwoLepton_TwoJet_mllgt150"] = true;
          }
          else{
            TMP_map_bool_To_Region["TwoLepton_TwoJet_mlllt150"] = true;
          }

        }

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
          JSFillHist(this_region, "dPhi_ljj_"+this_region, fabs( leps[0]->DeltaPhi(HNDiJet) ), weight, 40, 0., 4.);
        }


        if(this_region.Contains("TwoLepton")){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*leps[0])+(*leps[1])).M(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*leps[0])+(*leps[1])).Pt(), weight, 2000, 0., 2000.);
          JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*leps[0]).DeltaPhi(*leps[1])), weight, 40, 0., 4.);
        }

        JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

        JSFillHist(this_region, "NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
        JSFillHist(this_region, "NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
        JSFillHist(this_region, "NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);


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
