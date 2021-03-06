#include "AnalyzerParameter.h"

void AnalyzerParameter::Clear(){

  Name = "";

  MCCorrrectionIgnoreNoHist = false;

  Electron_Tight_ID = "";
  Electron_Loose_ID = "";
  Electron_Veto_ID = "";
  Electron_ID_SF_Key = "";
  Electron_FR_ID = "";
  Electron_FR_Key = "";
  Electron_CF_ID = "";
  Electron_CF_Key = "";
  Electron_Tight_RelIso = 999.;
  Electron_Loose_RelIso = 999.;
  Electron_Veto_RelIso = 999.;
  Electron_UseMini = false;
  Electron_UsePtCone = false;
  Electron_MinPt = 10.;

  Muon_Tight_ID = "";

  Muon_Loose_ID = "";
  Muon_Veto_ID = "";
  Muon_RECO_SF_Key = "";
  Muon_ID_SF_Key = "";
  Muon_ISO_SF_Key = "";
  Muon_Trigger_SF_Key = "";
  Muon_FR_ID = "";
  Muon_FR_Key = "";
  Muon_CF_ID = "";
  Muon_CF_Key = "";
  Muon_Tight_RelIso = 999.;
  Muon_Loose_RelIso = 999.;
  Muon_Veto_RelIso = 999.;
  Muon_UseMini = false;
  Muon_UsePtCone = false;
  Muon_UseTuneP = false;
  Muon_MinPt = 10.;

  Jet_ID = "";
  FatJet_ID = "";

  syst_ = Central;

}

AnalyzerParameter::AnalyzerParameter(){

  Name = "Default";

  MCCorrrectionIgnoreNoHist = false;

  Electron_Tight_ID = "passTightID";
  Electron_Loose_ID = "passLooseID";
  Electron_Veto_ID = "passVetoID";
  Electron_ID_SF_Key = "passTightID";

  Muon_Tight_ID = "POGTightWithTightIso";
  Muon_Loose_ID = "POGLoose";
  Muon_Veto_ID = "POGLoose";
  Muon_RECO_SF_Key = "";
  Muon_ID_SF_Key = "NUM_TightID_DEN_genTracks";
  Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
  Muon_Trigger_SF_Key = "POGTight";

  Jet_ID = "HN";
  FatJet_ID = "HN";

  syst_ = Central;

  dRSeparation = 0.4;

}

TString AnalyzerParameter::GetSystType(){

  if(syst_==Syst::Central){
    return "Central";
  }
  else if(syst_==Syst::JetResUp){
    return "JetResUp";
  }
  else if(syst_==Syst::JetResDown){
    return "JetResDown";
  }
  else if(syst_==Syst::JetEnUp){
    return "JetEnUp";
  }
  else if(syst_==Syst::JetEnDown){
    return "JetEnDown";
  }
  else if(syst_==Syst::JetMassUp){
    return "JetMassUp";
  }
  else if(syst_==Syst::JetMassDown){
    return "JetMassDown";
  }
  else if(syst_==Syst::MuonRecoSFUp){
    return "MuonRecoSFUp";
  }
  else if(syst_==Syst::MuonRecoSFDown){
    return "MuonRecoSFDown";
  }
  else if(syst_==Syst::MuonEnUp){
    return "MuonEnUp";
  }
  else if(syst_==Syst::MuonEnDown){
    return "MuonEnDown";
  }
  else if(syst_==Syst::MuonIDSFUp){
    return "MuonIDSFUp";
  }
  else if(syst_==Syst::MuonIDSFDown){
    return "MuonIDSFDown";
  }
  else if(syst_==Syst::MuonISOSFUp){
    return "MuonISOSFUp";
  }
  else if(syst_==Syst::MuonISOSFDown){
    return "MuonISOSFDown";
  }
  else if(syst_==Syst::MuonTriggerSFUp){
    return "MuonTriggerSFUp";
  }
  else if(syst_==Syst::MuonTriggerSFDown){
    return "MuonTriggerSFDown";
  }
  else if(syst_==Syst::ElectronRecoSFUp){
    return "ElectronRecoSFUp";
  }
  else if(syst_==Syst::ElectronRecoSFDown){
    return "ElectronRecoSFDown";
  }
  else if(syst_==Syst::ElectronResUp){
    return "ElectronResUp";
  }
  else if(syst_==Syst::ElectronResDown){
    return "ElectronResDown";
  }
  else if(syst_==Syst::ElectronEnUp){
    return "ElectronEnUp";
  }
  else if(syst_==Syst::ElectronEnDown){
    return "ElectronEnDown";
  }
  else if(syst_==Syst::ElectronIDSFUp){
    return "ElectronIDSFUp";
  }
  else if(syst_==Syst::ElectronIDSFDown){
    return "ElectronIDSFDown";
  }
  else if(syst_==Syst::ElectronTriggerSFUp){
    return "ElectronTriggerSFUp";
  }
  else if(syst_==Syst::ElectronTriggerSFDown){
    return "ElectronTriggerSFDown";
  }
  else if(syst_==Syst::LSFSFUp){
    return "LSFSFUp";
  }
  else if(syst_==Syst::LSFSFDown){
    return "LSFSFDown";
  }
  else if(syst_==Syst::PUUp){
    return "PUUp";
  }
  else if(syst_==Syst::PUDown){
    return "PUDown";
  }
  else if(syst_==Syst::ZPtRwUp){
    return "ZPtRwUp";
  }
  else if(syst_==Syst::ZPtRwDown){
    return "ZPtRwDown";
  }
  else if(syst_==Syst::ZPtRwQCDScaleUp){
    return "ZPtRwQCDScaleUp";
  }
  else if(syst_==Syst::ZPtRwQCDScaleDown){
    return "ZPtRwQCDScaleDown";
  }
  else if(syst_==Syst::ZPtRwQCDPDFErrorUp){
    return "ZPtRwQCDPDFErrorUp";
  }
  else if(syst_==Syst::ZPtRwQCDPDFErrorDown){
    return "ZPtRwQCDPDFErrorDown";
  }
  else if(syst_==Syst::ZPtRwQCDPDFAlphaSUp){
    return "ZPtRwQCDPDFAlphaSUp";
  }
  else if(syst_==Syst::ZPtRwQCDPDFAlphaSDown){
    return "ZPtRwQCDPDFAlphaSDown";
  }
  else if(syst_==Syst::ZPtRwEW1Up){
    return "ZPtRwEW1Up";
  }
  else if(syst_==Syst::ZPtRwEW1Down){
    return "ZPtRwEW1Down";
  }
  else if(syst_==Syst::ZPtRwEW2Up){
    return "ZPtRwEW2Up";
  }
  else if(syst_==Syst::ZPtRwEW2Down){
    return "ZPtRwEW2Down";
  }
  else if(syst_==Syst::ZPtRwEW3Up){
    return "ZPtRwEW3Up";
  }
  else if(syst_==Syst::ZPtRwEW3Down){
    return "ZPtRwEW3Down";
  }
  else if(syst_==Syst::PrefireUp){
    return "PrefireUp";
  }
  else if(syst_==Syst::PrefireDown){
    return "PrefireDown";
  }
  else if(syst_==Syst::DYReshapeSystUp){
    return "DYReshapeSystUp";
  }
  else if(syst_==Syst::DYReshapeSystDown){
    return "DYReshapeSystDown";
  }
  else if(syst_==Syst::BTagUp){
    return "BTagUp";
  }
  else if(syst_==Syst::BTagDown){
    return "BTagDown";
  }
  else{
    cout << "[AnalyzerParameter::GetSystType] Wrong Syst" << endl;
    exit(EXIT_FAILURE);
    return "ERROR";
  }

}

AnalyzerParameter::~AnalyzerParameter(){
}
