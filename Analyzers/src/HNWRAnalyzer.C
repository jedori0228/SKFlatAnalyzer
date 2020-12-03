#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  ApplyDYPtReweight = HasFlag("ApplyDYPtReweight");
  ApplyDYReshape = HasFlag("ApplyDYReshape");
  RunXsecSyst = HasFlag("RunXsecSyst");
  Signal = HasFlag("Signal");
  CalculateAverageKFactor = HasFlag("CalculateAverageKFactor");
  SignalElectronOnly = HasFlag("SignalElectronOnly");
  SignalMuonOnly = HasFlag("SignalMuonOnly");

  if(Signal){
    //==== MCSample = WRtoNLtoLLJJ_WR1400_N1000
    TString massstring = "";
    if(MCSample.Contains("WRtoNLtoLLJJ_")) massstring = MCSample(TString("WRtoNLtoLLJJ_").Length(), MCSample.Length());
    else if(MCSample.Contains("Official") && MCSample.Contains("WR")) massstring = MCSample(TString("Official_FullSim_").Length(), MCSample.Length());
    cout << "[HNWRAnalyzer::initializeAnalyzer()] massstring = " << massstring << endl;
    int i_underscore=-1;
    for(int i=0; i<massstring.Length(); i++){
      TString thissubstring = massstring(i);
      if(thissubstring=="_"){
        i_underscore = i;
        break;
      }
    }
    TString str_mWR = massstring(2,i_underscore-2);
    TString str_mN = massstring(i_underscore+1+1,massstring.Length());
    cout << "[HNWRAnalyzer::initializeAnalyzer()] str_mWR = ["<<str_mWR<<"]" << endl;
    cout << "[HNWRAnalyzer::initializeAnalyzer()] str_mN = ["<<str_mN<<"]" << endl;

    mWR = str_mWR.Atoi();
    mN = str_mN.Atoi();

    cout << "[HNWRAnalyzer::initializeAnalyzer()] mWR = " << mWR << endl;
    cout << "[HNWRAnalyzer::initializeAnalyzer()] mN = " << mN << endl;

    //==== Read kfactor vs m(lN) histogram
    TString datapath = getenv("DATA_DIR");
    TFile *file_kfactor = new TFile(datapath+"/HNWRGeneralData/kfactor.root");
    hist_kfactor = (TH1D *)file_kfactor->Get("kfactor_all");
    //==== get averaged kfactor
    string kfactor_line;
    TString path_AveragedKFactor = datapath+"/"+TString::Itoa(DataYear,10)+"/HNWRKFactor/AveragedKFactor.txt";
    if(MCSample.Contains("Official") && MCSample.Contains("WR")){
      path_AveragedKFactor = datapath+"/"+TString::Itoa(DataYear,10)+"/HNWRKFactor/AveragedKFactor_WROfficial.txt";
    }
    cout << "[HNWRAnalyzer::initializeAnalyzer()] Average-kfactor taken from \"" << path_AveragedKFactor << "\"" << endl;
    ifstream kfactor_in(path_AveragedKFactor);
    this_avg_kfactor = -1.;
    while(getline(kfactor_in,kfactor_line)){
      std::istringstream is( kfactor_line );

      TString this_mass;
      double avg_k_ee, avg_k_mm;
      is >> this_mass;
      is >> avg_k_ee;
      is >> avg_k_mm;

      if(massstring==this_mass){
        if(SignalElectronOnly) this_avg_kfactor = avg_k_ee;
        else if(SignalMuonOnly) this_avg_kfactor = avg_k_mm;
        break;
      }
    }
    cout << "[HNWRAnalyzer::initializeAnalyzer()] this_avg_kfactor = " << this_avg_kfactor << endl;

  }

  HEM1516 = HasFlag("HEM1516");
  BeforeRun319077 = HasFlag("BeforeRun319077");

  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] ApplyDYPtReweight = " << ApplyDYPtReweight << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunXsecSyst = " << RunXsecSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] Signal = " << Signal << endl;

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
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 30.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2017){

    Triggers_Electron = {
      "HLT_Ele35_WPTight_Gsf_v",
      "HLT_Photon200_v",
      //"HLT_Ele115_CaloIdVT_GsfTrkIdT_v", // This trigger was off for the first ~5fb-1, so we are not using it
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 38.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2018){

    Triggers_Electron = {
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_Photon200_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 35.;
    TriggerSafePt_Muon = 52.;

  }

  //==== Z-pt rewieght
  ZPtReweight = 1.;
  ZPtReweight_Up = 1.;
  ZPtReweight_Down = 1.;

  //==== b tagging
  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  mcCorr->SetJetTaggingParameters(jtps);

  //==== Signal finder
  genFinderSig = new GenFinderForHNWRSignal();
  SignalLeptonChannel = -1;

  genFinderDY = new GenFinderForDY();

  //==== PUReweight
  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
    //==== Only for 2016 FastSim, use different one..
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUhname = " << PUhname << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

  //==== DYReshape

  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TFile *file_DYPtReweight = new TFile(datapath+"/"+TString::Itoa(DataYear,10)+"/HNWRDYPtReweight/Ratio.root");
    hist_DYPtReweight = (TH2D *)file_DYPtReweight->Get("Ratio");

    TFile *file_DYReshape = new TFile(datapath+"/"+TString::Itoa(DataYear,10)+"/HNWRDYReshape/DYReshape_"+TString::Itoa(DataYear,10)+".root");
    hist_DYReshape_Resolved_ratio_AllCh = (TH1D *)file_DYReshape->Get("Resolved_ratio_AllCh");
    hist_DYReshape_Resolved_EEOnlyRatio = (TH1D *)file_DYReshape->Get("Resolved_EEOnlyRatio");
    hist_DYReshape_Resolved_MuMuOnlyRatio = (TH1D *)file_DYReshape->Get("Resolved_MuMuOnlyRatio");

    hist_DYReshape_Boosted_ratio_AllCh = (TH1D *)file_DYReshape->Get("Boosted_ratio_AllCh");
    hist_DYReshape_Boosted_EEOnlyRatio = (TH1D *)file_DYReshape->Get("Boosted_EEOnlyRatio");
    hist_DYReshape_Boosted_MuMuOnlyRatio = (TH1D *)file_DYReshape->Get("Boosted_MuMuOnlyRatio");

  }

}

void HNWRAnalyzer::executeEvent(){

  if(BeforeRun319077){
    if(run>=319077) return;
  }

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();

/*
  //==== TEST) draw gen WR
  genFinderSig->Run(gens);
  FillHist("GenWR_Mass", genFinderSig->WR.M(), 1., 8000, 0., 8000.);
  FillHist("IsOffShellPrimaryWR", genFinderSig->IsOffShellPrimaryWR, 1., 2, 0., 2.);
  FillHist("GenlN_Mass", (genFinderSig->priLep+genFinderSig->N).M(), 1., 8000, 0., 8000.);
  return;
*/

  //=======================================================================
  //==== Main analyzer

  if(MCSample.Contains("DYJets")){
    genFinderDY->Find(gens);
    GenZParticle = genFinderDY->GenZ;
  }
  if(ApplyDYPtReweight){

    double mZ = GenZParticle.M();
    double ptZ = GenZParticle.Pt();

    if(mZ<50.) mZ=51.;
    if(mZ>=1000.) mZ=999.;
    //if(ptZ<70.) ptZ=71.;
    if(ptZ>=1000.) ptZ=999.;

    int bin_mass = hist_DYPtReweight->GetXaxis()->FindBin(mZ);
    int bin_pt   = hist_DYPtReweight->GetYaxis()->FindBin(ptZ);

    double value = hist_DYPtReweight->GetBinContent( bin_mass, bin_pt );
    double error = hist_DYPtReweight->GetBinError( bin_mass, bin_pt );

    ZPtReweight = value;
    ZPtReweight_Up = value+error;
    ZPtReweight_Down = value-error;

    //cout << GenZParticle.M() << "\t" << GenZParticle.Pt() << " -> " << value << endl;

  }

  if(Signal){

    double this_MCweight = GetEvent().MCweight();

    int genNpid = -1;
    for(unsigned int i=2; i<gens.size(); i++){
      int pid = abs( gens.at(i).PID() );
      if( pid==9900012 || pid==9900014 ){
        genNpid = pid;
        FillHist("GEN_N_Eta", gens.at(i).Eta(), 1., 60, -3., 3.);
        FillHist("GEN_N_Pt", gens.at(i).Pt(), 1., 800, 0., 8000.);
        break;
      }
    }
    //==== 0 = electron
    //==== 1 = muon
    if(genNpid==9900012){
      SignalLeptonChannel = 0;
      FillHist("SignalFlavour", 0., this_MCweight, 3, -1., 2.);
    }
    else if(genNpid==9900014){
      SignalLeptonChannel = 1;
      FillHist("SignalFlavour", 1., this_MCweight, 3, -1., 2.);
    }
    else{
      FillHist("SignalFlavour", -1., this_MCweight, 3, -1., 2.);
    }

    genFinderSig->Run(gens);
    this_kfactor = GetHNWRKFactor( (genFinderSig->priLep+genFinderSig->N).M() );

    FillHist("GEN_WR_Mass", genFinderSig->WR.M(), 1., 800, 0., 8000.);
    FillHist("GEN_WR_Mass_"+TString::Itoa(SignalLeptonChannel,10), genFinderSig->WR.M(), 1., 800, 0., 8000.);
    FillHist("GEN_LepFalv_"+TString::Itoa(SignalLeptonChannel,10)+"_Pt", genFinderSig->priLep.Pt(), 1., 400, 0., 4000.);
    FillHist("GEN_LepFalv_"+TString::Itoa(SignalLeptonChannel,10)+"_Pt", genFinderSig->secLep.Pt(), 1., 400, 0., 4000.);

    if(CalculateAverageKFactor){
      FillHist("GenlN_Mass", (genFinderSig->priLep+genFinderSig->N).M(), 1., 8000, 0., 8000.);
      double normweight = 1./sumW * this_MCweight;

      if(SignalLeptonChannel==0){
        FillHist("ElectronChannel_KFactor", 0., this_kfactor * normweight, 1, 0., 1.);
      }
      else if(SignalLeptonChannel==1){
        FillHist("MuonChannel_KFactor", 0., this_kfactor * normweight, 1, 0., 1.);
      }
      return;
    }

    if(SignalElectronOnly && SignalLeptonChannel!=0) return;
    if(SignalMuonOnly && SignalLeptonChannel!=1) return;

  }

  //==== Prefire weight

  weight_Prefire      = GetPrefireWeight( 0);
  weight_Prefire_Up   = GetPrefireWeight(+1);
  weight_Prefire_Down = GetPrefireWeight(-1);

  //==== Nvtx

  N_VTX = nPV;
  if(!IsDATA){
    //==== nPileUp starts from 0
    N_VTX = nPileUp+1;
    //==== but histogram is as a function of nPileUp not nVTX
    int nPileUpForBin = nPileUp;
    if(DataYear==2018) nPileUpForBin = max(5, nPileUpForBin);
    int bin_pu = hist_PUReweight->FindBin(nPileUpForBin);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);

  }

  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = false;

  param.Electron_Tight_ID = "HNWRTight";
  if(DataYear>=2017 && IsFastSim) param.Electron_Tight_ID = "HNWRTightFastSim";
  //==== use HEEPV70 2018Prompt for 2018
  if(DataYear==2018){
    param.Electron_Tight_ID = "HNWRTight2018Prompt";
    if(IsFastSim) param.Electron_Tight_ID = "HNWRTight2018PromptFastSim";
  }
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
  param.Muon_RECO_SF_Key = "HighPtMuonRecoSF";
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
  //AllFatJets = puppiCorr->Correct( GetAllFatJets() ); // TODO correct SD Mass here, but not sure about the systematics
  AllFatJets = GetAllFatJets();

  param.dRSeparation = 0.4;

  executeEventFromParameter(param);

  if(RunSyst){

    for(int it_syst=1; it_syst<AnalyzerParameter::NSyst; it_syst++){
      param.syst_ = AnalyzerParameter::Syst(it_syst);

      //==== ZPtRwUp and ZPtRwDown are only ran when ApplyDYPtReweight
      //==== If not, skip
      if( (param.syst_==AnalyzerParameter::ZPtRwUp || param.syst_==AnalyzerParameter::ZPtRwDown) && !ApplyDYPtReweight ) continue;

      param.Name = "Syst_"+param.GetSystType()+"_HNWR";

      executeEventFromParameter(param);
    }
  }

}

void HNWRAnalyzer::executeEventFromParameter(AnalyzerParameter param){

/*
  //==== DY LO vs NLO stduy

  vector<LHE> lhes = GetLHEs();

  int LHE_lepton_flavour = -1; // 0 : ee, 1 : mm, 2 : tt
  int nFinalParton = 0;

  for(unsigned int i=0; i<lhes.size(); i++){
    int this_ID = lhes.at(i).ID();
    int this_Status = lhes.at(i).Status();

    //==== Status 1
    if( this_Status == 1){

      //==== Check lepton flavour
      if( abs(this_ID)== 11 ){
        LHE_lepton_flavour = 0;
      }
      else if(abs(this_ID)== 13 ){
        LHE_lepton_flavour = 1;
      }
      else if(abs(this_ID)== 15 ){
        LHE_lepton_flavour = 2;
      }

      //==== final state parton
      if( abs(this_ID) <= 6 ){
        nFinalParton++;
      }
      if( abs(this_ID) == 9 || abs(this_ID) == 21 ){
        nFinalParton++;
      }

    } // END if status 1
  }
  FillHist("LHEStudy/NoMassCut__LHE_lepton_flavour", LHE_lepton_flavour, weight_norm_1invpb, 4, -1., 3.);
  FillHist("LHEStudy/NoMassCut__nFinalParton", nFinalParton, weight_norm_1invpb, 10, 0., 10.);
*/

  //=====================================================
  //==== Main Analyzer

  bool IsCentral = !( param.Name.Contains("Syst_") );

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();
  double weight = 1.;
  if(!IsDATA){

    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();

    //==== Apply Prefire
    if(param.syst_ == AnalyzerParameter::PrefireUp) weight *= weight_Prefire_Up;
    else if(param.syst_ == AnalyzerParameter::PrefireDown)  weight *= weight_Prefire_Down;
    else  weight *= weight_Prefire;

    //==== Apply ZPtReweight
    if(ApplyDYPtReweight){

      if(param.syst_ == AnalyzerParameter::ZPtRwUp) weight *= ZPtReweight_Up;
      else if(param.syst_ == AnalyzerParameter::ZPtRwDown) weight *= ZPtReweight_Down;
      else weight *= ZPtReweight;

    }

    if(Signal){
      weight *= this_kfactor/this_avg_kfactor;
    }

  }

  //=============
  //==== No Cut
  //=============

  FillCutFlow(IsCentral, "CutFlow", "NoCut_"+param.Name, weight);

  if(RunXsecSyst && param.syst_ == AnalyzerParameter::Central){
    double normweight = 1./sumW/PDFWeights_Error->at(0) * this_kfactor/this_avg_kfactor;
    for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
      FillHist("XsecSyst_Den/PDFWeights_Scale_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_Scale->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
    }
    for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
      FillHist("XsecSyst_Den/PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_Error->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
      if(PDFWeights_Error->at(i)>0){
        FillHist("XsecSyst_Den/PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den_Positive", 0., PDFWeights_Error->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
      }
      else{
        FillHist("XsecSyst_Den/PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den_Negative", 0., PDFWeights_Error->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
      }
    }
    for(unsigned int i=0; i<PDFWeights_AlphaS->size(); i++){
      FillHist("XsecSyst_Den/PDFWeights_AlphaS_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_AlphaS->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
    }
  }

  //========================
  //==== Event selecitions
  //========================

  if(!PassMETFilter()) return;

  FillCutFlow(IsCentral, "CutFlow", "METFilter_"+param.Name, weight);

  bool PassSingleElectron = ev.PassTrigger(Triggers_Electron);
  bool PassMu50 = ev.PassTrigger(Triggers_Muon);
  if(IsFastSim){
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

  int SystDir_MuonRecoSF(0);
  int SystDir_ElectronRecoSF(0);

  int SystDir_MuonIDSF(0);
  int SystDir_ElectronIDSF(0);

  int SystDir_MuonISOSF(0);

  int SystDir_MuonTriggerSF(0);
  int SystDir_ElectronTriggerSF(0);

  int SystDir_LSFSF(0);
  int SystDir_PU(0);

  int SystDir_DYReshape(0);

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
  else if(param.syst_ == AnalyzerParameter::MuonRecoSFUp){
    SystDir_MuonRecoSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown){
    SystDir_MuonRecoSF = -1;
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
  else if(param.syst_ == AnalyzerParameter::MuonISOSFUp){
    SystDir_MuonISOSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonISOSFDown){
    SystDir_MuonISOSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonTriggerSFUp){
    SystDir_MuonTriggerSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonTriggerSFDown){
    SystDir_MuonTriggerSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFUp){
    SystDir_ElectronRecoSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFDown){
    SystDir_ElectronRecoSF = -1;
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
  else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFUp){
    SystDir_ElectronTriggerSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFDown){
    SystDir_ElectronTriggerSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::LSFSFUp){
    SystDir_LSFSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::LSFSFDown){
    SystDir_LSFSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::PUUp){
    SystDir_PU = +1;
  }
  else if(param.syst_ == AnalyzerParameter::PUDown){
    SystDir_PU = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ZPtRwUp){

  }
  else if(param.syst_ == AnalyzerParameter::ZPtRwDown){

  }
  else if(param.syst_ == AnalyzerParameter::PrefireUp){

  }
  else if(param.syst_ == AnalyzerParameter::PrefireDown){

  }
  else if(param.syst_ == AnalyzerParameter::DYReshapeSystUp){
    SystDir_DYReshape = +1;
  }
  else if(param.syst_ == AnalyzerParameter::DYReshapeSystDown){
    SystDir_DYReshape = -1;
  }
  else if(param.syst_ == AnalyzerParameter::DYReshapeEEMMUp){
    SystDir_DYReshape = +2;
  }
  else if(param.syst_ == AnalyzerParameter::DYReshapeEEMMDown){
    SystDir_DYReshape = -2;
  }
  else{
    cerr << "[HNWRAnalyzer::executeEventFromParameter] Wrong syst : param.syst_ = " << param.syst_ << endl;
    exit(EXIT_FAILURE);
  }

  //==== PU reweight

  if(!IsDATA){

    double this_pureweight = 1.;
    if(SystDir_PU==0) this_pureweight = weight_PU;
    else if(SystDir_PU>0) this_pureweight = weight_PU_Up;
    else this_pureweight = weight_PU_Down;

    weight *= this_pureweight;
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

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.4);
  std::vector<FatJet>   fatjets_LSF    = SelectFatJets(this_AllFatJets, "HNLSF", 200, 2.4);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);
  std::sort(fatjets_LSF.begin(), fatjets_LSF.end(), PtComparing);

  std::vector<Jet>      jets         = SelectJets(this_AllJets, param.Jet_ID, 40., 2.4);
  std::sort(jets.begin(), jets.end(), PtComparing);

  int NBJets=0;
  for(unsigned int i=0; i<jets.size(); i++){
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,
                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<this_AllJets.size(); i++){
    HT += this_AllJets.at(i).Pt();
  }

  //===========================
  //==== Lepton scale factors
  //===========================

  double this_trigger_sf = 1.;

  //=====================
  //==== Categorization
  //=====================

  std::map<TString, bool> map_bool_To_Region;

  FatJet HNFatJet;
  Particle WRCand;
  Particle NCand;
  Particle NCand_1, NCand_2;

  //==== leps_for_plot
  //==== - If Resolved, at(0) is Tight Leading, at(1) is Tight Subleading
  //==== - If Boosted, at(0) is Tight Leading, at(1) is Loose inside the AK8jet
  vector<Lepton *> leps_for_plot;
  int NExtraLooseElectron(0),NExtraLooseMuon(0),NExtraLooseLepton(0);
  int NExtraTightElectron(0),NExtraTightMuon(0),NExtraTightLepton(0);

  //==== Check Resolved first

  bool IsResolvedEvent = false;
  bool IsResolved_SR_EE(false), IsResolved_SR_MM(false), IsResolved_SR_EM(false);
  bool IsResolved_LowWRCR_EE(false), IsResolved_LowWRCR_MM(false), IsResolved_LowWRCR_EM(false);
  bool IsResolved_DYCR_EE(false), IsResolved_DYCR_MM(false), IsResolved_DYCR_EM(false);

  if( (n_Tight_leptons==2) && (Tight_leps.at(0)->Pt()>60.) ){

    FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+param.Name, weight);

    TString Suffix = "";
    bool tmp_IsEE(false), tmp_IsMM(false), tmp_IsEM(false);
    bool this_triggerpass(false);
    if(      (Tight_electrons.size()==2) && (Tight_muons.size()==0) ){
      tmp_IsEE = true;
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;

      if(Signal){
        if(SignalLeptonChannel!=0) return;
      }

    }
    else if( (Tight_electrons.size()==0) && (Tight_muons.size()==2) ){
      tmp_IsMM = true;
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;

      if(Signal){
        if(SignalLeptonChannel!=1) return;
      }

    }
    else if( (Tight_electrons.size()==1) && (Tight_muons.size()==1) ){
      tmp_IsEM = true;
      Suffix = "EMu";
      this_triggerpass = PassMu50;
    }
    FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_"+param.Name, weight);

    if(this_triggerpass){

      FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_PassTrigger_"+param.Name, weight);

      //==== lljj

      if( jets.size()>=2 ){

        FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_TwoAK4Jets_"+param.Name, weight);

        Lepton *LeadLep = Tight_leps.at(0);
        Lepton *SubLeadLep = Tight_leps.at(1);

        bool dRLeadJetLepton    = ( jets.at(0).DeltaR( *LeadLep ) > param.dRSeparation ) && ( jets.at(0).DeltaR( *SubLeadLep ) > param.dRSeparation );
        bool dRSubLeadJetLepton = ( jets.at(1).DeltaR( *LeadLep ) > param.dRSeparation ) && ( jets.at(1).DeltaR( *SubLeadLep ) > param.dRSeparation );
        bool dRTwoLepton = (LeadLep->DeltaR( *SubLeadLep ) > param.dRSeparation);
        bool dRTwoJets = (jets.at(0).DeltaR( jets.at(1) ) > param.dRSeparation);

        if( dRLeadJetLepton && dRSubLeadJetLepton && dRTwoLepton && dRTwoJets ){

          FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_dRSeparation_"+param.Name, weight);

          IsResolvedEvent = true;

          WRCand = *LeadLep+*SubLeadLep+jets.at(0)+jets.at(1);
          NCand = *SubLeadLep+jets.at(0)+jets.at(1);
          NCand_1 = *LeadLep+jets.at(0)+jets.at(1);
          NCand_2 = *SubLeadLep+jets.at(0)+jets.at(1);

          double dilep_mass = (*LeadLep+*SubLeadLep).M();

          bool DiLepMassGT200 = (dilep_mass > 200.);
          bool DiLepMassGT400 = (dilep_mass > 400.);
          bool DiLepMassLT150 = (dilep_mass >= 60.) && (dilep_mass < 150.);

          bool DiLepMass60to100  = (dilep_mass >= 60.) && (dilep_mass < 100.);
          bool DiLepMass100to150 = (dilep_mass >= 100.) && (dilep_mass < 150.);
          bool DiLepMass200to400  = (dilep_mass >= 200.) && (dilep_mass < 400.);

          bool WRMassGT800 = ( WRCand.M() > 800. );

          //==== Now IsResolvedEvent is set
          //==== No overlap with Boosted, so we can apply SFs

          //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
          //@@@@ Apply scale factors

          double trigger_sf_SingleElectron = 1.;
          double trigger_sf_SingleMuon = 1.;

          if(!IsDATA){

            mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

            for(unsigned int i=0; i<Tight_electrons.size(); i++){
              double this_recosf = mcCorr->ElectronReco_SF(Tight_electrons.at(i)->scEta(),Tight_electrons.at(i)->Pt(), SystDir_ElectronRecoSF);
              double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, Tight_electrons.at(i)->scEta(), Tight_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
              weight *= this_recosf*this_idsf;
            }
            for(unsigned int i=0; i<Tight_muons.size(); i++){

              double MiniAODP = sqrt( Tight_muons.at(i)->MiniAODPt() * Tight_muons.at(i)->MiniAODPt() + Tight_muons.at(i)->Pz() * Tight_muons.at(i)->Pz() );

              double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, Tight_muons.at(i)->Eta(), MiniAODP, SystDir_MuonRecoSF);
              double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
              double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonISOSF);

              weight *= this_recosf*this_idsf*this_isosf;
            }

            //==== Trigger SF

            trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons, SystDir_ElectronTriggerSF);
            trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons, SystDir_MuonTriggerSF);

            //==== DYReshape for DY
            if(ApplyDYReshape){
              weight *= GetDYReshape(WRCand.M(), "Resolved", SystDir_DYReshape);
            }

          }
          if(Suffix=="SingleElectron"){
            this_trigger_sf = trigger_sf_SingleElectron;
            weight *= trigger_sf_SingleElectron;
          }
          else{
            trigger_sf_SingleElectron = trigger_sf_SingleMuon;
            weight *= trigger_sf_SingleMuon;
          }
          //@@@@
          //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

          if(DiLepMassGT200){

            FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mllGT200_"+param.Name, weight);

            if(WRMassGT800){

              FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mWRGT800_"+param.Name, weight);

              //==== Region Dictionary
              //==== - HNWR_SingleElectron_Resolved_SR : ee Resolved SR [IsResolved_SR_EE]
              //==== - HNWR_SingleMuon_Resolved_SR : mm Resolved SR [IsResolved_SR_MM]
              //==== - HNWR_EMu_Resolved_SR : em Resolved sideband [IsResolved_SR_EM]

              if(DiLepMassGT400){

                FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mWRGT800_mll400_"+param.Name, weight);

                map_bool_To_Region[Suffix+"_Resolved_SR"] = true;
                if(tmp_IsEE) IsResolved_SR_EE = true;
                else if(tmp_IsMM) IsResolved_SR_MM = true;
                else if(tmp_IsEM) IsResolved_SR_EM = true;

/*
                if( (IsResolved_SR_EE||IsResolved_SR_MM) ){
                  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                  cout << "Resolved Event" << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "tmp_IsEE = " << tmp_IsEE << endl;
                  cout << "tmp_IsMM = " << tmp_IsMM << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "[RECO]" << endl;
                  cout << "LeadLep : "; LeadLep->Print();
                  cout << "SubLeadLep : "; SubLeadLep->Print();
                  cout << "j1 : "; jets.at(0).Print();
                  cout << "j2 : "; jets.at(1).Print();
                  if(fatjets_LSF.size()>0){
                    cout << "LSF FatJet size = " << fatjets_LSF.size() << endl;
                    cout << "J1 : "; fatjets_LSF.at(0).Print();
                  }
                  cout << "-->" << endl;
                  cout << "(l1+l2+j1+j2) : "; (WRCand).Print();
                  cout << "(l2+j1+j2) : "; (NCand).Print();
                  cout << "dR(j1,j2) = " << jets.at(0).DeltaR(jets.at(1)) << endl;
                  cout << "dR(l2,j1) = " << SubLeadLep->DeltaR(jets.at(0)) << endl;
                  cout << "dR(l2,j2) = " << SubLeadLep->DeltaR(jets.at(1)) << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "[GEN]" << endl;
                  cout << "WR : "; (genFinderSig->WR).Particle::Print();
                  cout << "Primary : "; (genFinderSig->priLep).Particle::Print();
                  cout << "N : "; (genFinderSig->N).Particle::Print();
                  cout << "Secondary : ";  (genFinderSig->secLep).Particle::Print();
                  cout << "jet1 : "; (genFinderSig->jet1).Particle::Print();
                  cout << "jet2 : "; (genFinderSig->jet2).Particle::Print();
                  cout << "-->" << endl;
                  cout << "WR matching : dR = " << WRCand.DeltaR( genFinderSig->WR ) << "; pt_gen = " << (genFinderSig->WR).Pt() << ", pt_reco = " << WRCand.Pt() << endl;
                  cout << "Primary lepton matching : dR = " << LeadLep->DeltaR( genFinderSig->priLep ) << "; pt_gen = " << (genFinderSig->priLep).Pt() << ", pt_reco = " <<LeadLep->Pt() << endl;
                  cout << "Secondary lepton matching : dR = " << SubLeadLep->DeltaR( genFinderSig->secLep ) << "; pt_gen = " << (genFinderSig->secLep).Pt() << ", pt_reco = " <<SubLeadLep->Pt() << endl;
                }
*/
              }

            }
            //==== Validate EMu method here
            else{

              //==== Region Dictionary
              //==== - HNWR_SingleElectron_Resolved_LowWRCR : ee Resolved, but low m(WR) [IsResolved_LowWRCR_EE]
              //==== - HNWR_SingleMuon_Resolved_LowWRCR : mm Resolved, but low m(WR) [IsResolved_LowWRCR_MM]
              //==== - HNWR_EMu_Resolved_LowWRCR : em Resolved, but low m(WR) [IsResolved_LowWRCR_EM]

              map_bool_To_Region[Suffix+"_Resolved_LowWRCR"] = true;
              if(tmp_IsEE) IsResolved_LowWRCR_EE = true;
              else if(tmp_IsMM) IsResolved_LowWRCR_MM = true;
              else if(tmp_IsEM) IsResolved_LowWRCR_EM = true;
            }

          }

          leps_for_plot.push_back( Tight_leps.at(0) );
          leps_for_plot.push_back( Tight_leps.at(1) );

          //==== Region Dictionary
          //==== - HNWR_SingleElectron_Resolved_DYCR : ee Resolved DYCR [IsResolved_DYCR_EE]
          //==== - HNWR_SingleMuon_Resolved_DYCR : mm Resolved DYCR [IsResolved_DYCR_MM]
          //==== - HNWR_EMu_Resolved_DYCR : filled, but NOT USED [IsResolved_DYCR_EM]

          if(DiLepMassLT150 && WRMassGT800){
            map_bool_To_Region[Suffix+"_Resolved_DYCR"] = true;
            if(tmp_IsEE) IsResolved_DYCR_EE = true;
            else if(tmp_IsMM) IsResolved_DYCR_MM = true;
            else if(tmp_IsEM) IsResolved_DYCR_EM = true;

            if(DiLepMass60to100){
              map_bool_To_Region[Suffix+"_Resolved_DYCR1"] = true;
            }
            if(DiLepMass100to150){
              map_bool_To_Region[Suffix+"_Resolved_DYCR2"] = true;
            }
          }
          if(DiLepMass200to400 && WRMassGT800){
            map_bool_To_Region[Suffix+"_Resolved_DYCR3"] = true;
          }

          for(unsigned int i=0; i<Loose_leps.size(); i++){
            if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SubLeadLep){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraLooseLepton++;
            if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
            else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in RESOLVED" << endl;
              exit(EXIT_FAILURE);
            }

          }
          for(unsigned int i=0; i<Tight_leps.size(); i++){
            if(Tight_leps.at(i)==LeadLep || Tight_leps.at(i)==SubLeadLep){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraTightLepton++;
            if(Tight_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraTightElectron++;
            else if(Tight_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraTightMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra Tight lepton in RESOLVED" << endl;
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
  bool IsBoosted_LowWRCR_EE(false), IsBoosted_LowWRCR_MM(false);
  bool IsBoosted_DYCR_EE(false), IsBoosted_DYCR_MM(false);
  bool IsBoosted_CR_EMJet(false), IsBoosted_CR_MEJet(false);
  bool IsBoosted_LowWRCR_EMJet(false), IsBoosted_LowWRCR_MEJet(false);
  int JetLepFlav=-1;
  if(!IsResolvedEvent){

    FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+param.Name, weight);

    if( (n_Tight_leptons>0) && (Tight_leps.at(0)->Pt()>60.) ){

      FillCutFlow(IsCentral, "CutFlow", "NotResolved_TightLeptonExist_"+param.Name, weight);

      Lepton *LeadLep = Tight_leps.at(0);
      bool tmp_IsLeadE(false), tmp_IsLeadM(false);
      TString Suffix = "";
      bool this_triggerpass(false);
      std::vector<Electron *> ForSF_electrons;
      std::vector<Muon *> ForSF_muons;
      if(LeadLep->LeptonFlavour()==Lepton::ELECTRON){
        tmp_IsLeadE = true;
        Suffix = "SingleElectron";
        this_triggerpass = PassSingleElectron;

        ForSF_electrons.push_back( Tight_electrons.at(0) );

        if(Signal){
          if(SignalLeptonChannel!=0) return;
        }

      }
      else if(LeadLep->LeptonFlavour()==Lepton::MUON){
        tmp_IsLeadM = true;
        Suffix = "SingleMuon";
        this_triggerpass = PassMu50;

        ForSF_muons.push_back( Tight_muons.at(0) );

        if(Signal){
          if(SignalLeptonChannel!=1) return;
        }

      }
      else{
        cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong flavour : " << LeadLep->LeptonFlavour() << endl;
        exit(EXIT_FAILURE);
      }

      FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_"+param.Name, weight);

      //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
      //@@@@ Apply lepton scale factors

      if(!IsDATA){

        //==== lepton scale factors here

        mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

        for(unsigned int i=0; i<ForSF_electrons.size(); i++){
          double this_recosf = mcCorr->ElectronReco_SF(ForSF_electrons.at(i)->scEta(),ForSF_electrons.at(i)->Pt(), SystDir_ElectronRecoSF);
          double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, ForSF_electrons.at(i)->scEta(), ForSF_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
          weight *= this_recosf*this_idsf;
        }
        for(unsigned int i=0; i<ForSF_muons.size(); i++){

          double MiniAODP = sqrt( ForSF_muons.at(i)->MiniAODPt() * ForSF_muons.at(i)->MiniAODPt() + ForSF_muons.at(i)->Pz() * ForSF_muons.at(i)->Pz() );

          double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, ForSF_muons.at(i)->Eta(), MiniAODP, SystDir_MuonRecoSF);
          double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  ForSF_muons.at(i)->Eta(), ForSF_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
          double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, ForSF_muons.at(i)->Eta(), ForSF_muons.at(i)->MiniAODPt(), SystDir_MuonISOSF);

          weight *= this_recosf*this_idsf*this_isosf;
        }

      }
      //@@@@
      //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

      if(this_triggerpass){

        FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_PassTrigger_"+param.Name, weight);

        leps_for_plot.push_back( Tight_leps.at(0) );

        std::vector<Lepton *> Loose_SF_leps = tmp_IsLeadE ? Loose_leps_el : Loose_leps_mu;
        std::vector<Lepton *> Loose_OF_leps = tmp_IsLeadE ? Loose_leps_mu : Loose_leps_el;;

        //==== Check for DYCR very first time

        bool HasLowMll = false;
        double LowMllMass = -1;
        Lepton *LowMllLooseLepton;
        for(unsigned int i=0; i<Loose_SF_leps.size(); i++){
          if(Loose_SF_leps.at(i)==LeadLep){
            //cout << "--> duplicate" << endl;
            continue;
          }
          double dilep_mass  = (*(LeadLep)+*(Loose_SF_leps.at(i))).M();
          if( (dilep_mass >= 60.) && (dilep_mass < 150.) ){
            HasLowMll = true;
            LowMllMass = dilep_mass;
            LowMllLooseLepton = Loose_SF_leps.at(i);
            leps_for_plot.push_back( Loose_SF_leps.at(i) );

            if(tmp_IsLeadM){
              //==== In this case, the loose ID is HighPt ID muon.
              //==== we want to apply the lepton scale factors to these muons
              Muon *looseMuon = (Muon *)LowMllLooseLepton;
              ForSF_muons.push_back( looseMuon );
            }

            break;
          }
        }

        if(HasLowMll){

          FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasLowMll_"+param.Name, weight);

          map_bool_To_Region[Suffix+"_Boosted_LowMll"] = true;

          for(unsigned int i=0; i<fatjets.size(); i++){

            FatJet this_fatjet = fatjets.at(i);

            if( fabs( LeadLep->DeltaPhi(this_fatjet) ) > 2.0 ){

              FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasLowMll_HasBackToBackAK8Jet_"+param.Name, weight);

              map_bool_To_Region[Suffix+"_Boosted_LowMllWithAK8Jet"] = true;

              //==== Now this is the DY sideband we want

              HNFatJet = this_fatjet;
              if( this_fatjet.DeltaR( *LowMllLooseLepton ) < 0.8 ){
                NCand = HNFatJet;
              }
              else{
                NCand = HNFatJet + *(LowMllLooseLepton);
              }
              WRCand = *LeadLep+NCand;

              //==== Region Dictionary
              //==== - HNWR_SingleElectron_Boosted_DYCR : ee Boosted DYCR [IsBoosted_DYCR_EE]
              //==== - HNWR_SingleMuon_Boosted_DYCR : mm Boosted DYCR [IsBoosted_DYCR_MM]

              if( WRCand.M() > 800. ){

                FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_DYCR_"+param.Name, weight);

                //==== 60~150
                map_bool_To_Region[Suffix+"_Boosted_DYCR"] = true;
                if(tmp_IsLeadE) IsBoosted_DYCR_EE = true;
                else if(tmp_IsLeadM) IsBoosted_DYCR_MM = true;

                if( LowMllMass < 100 ){
                  map_bool_To_Region[Suffix+"_Boosted_DYCR1"] = true;
                }
                else{
                  map_bool_To_Region[Suffix+"_Boosted_DYCR2"] = true;
                }

              }

              break;

            } // END if back-to-back

          } // END loop fatjet

        } // END if HasLowMll

        //==== If not, look for merged jet
        else{

          FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoLowmll_"+param.Name, weight);

          bool HasAwayMergedFatJet = false;
          for(unsigned int i=0; i<fatjets_LSF.size(); i++){
            FatJet this_fatjet = fatjets_LSF.at(i);
            if( fabs( LeadLep->DeltaPhi(this_fatjet) ) > 2.0 ){
              HasAwayMergedFatJet = true;
              HNFatJet = this_fatjet;
              NCand = HNFatJet;
              WRCand = *LeadLep+NCand;
              break;
            }
          }

          if(HasAwayMergedFatJet){

            FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasMergedJet_"+param.Name, weight);

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

            //==== Now, veto with additoanl Tight lepton
            for(unsigned int i=0; i<Loose_leps.size(); i++){
              if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SFLooseLepton || Loose_leps.at(i)==OFLooseLepton ){
                //cout << "--> duplicate" << endl;
                continue;
              }
              NExtraLooseLepton++;
              if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
              else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
              else{
                cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in BOOSTED" << endl;
                exit(EXIT_FAILURE);
              }

            }
            for(unsigned int i=0; i<Tight_leps.size(); i++){
              if(Tight_leps.at(i)==LeadLep || Tight_leps.at(i)==SFLooseLepton || Tight_leps.at(i)==OFLooseLepton ){
                //cout << "--> duplicate" << endl;
                continue;
              }
              NExtraTightLepton++;
              if(Tight_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraTightElectron++;
              else if(Tight_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraTightMuon++;
              else{
                cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra Tight lepton in BOOSTED" << endl;
                exit(EXIT_FAILURE);
              }

            }

            bool NoExtraTightLepton = (NExtraTightLepton==0);
            bool WRMassGT800 = ( WRCand.M() > 800. );

            if(NoExtraTightLepton){

              FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoExtraTight_"+param.Name, weight);

              //=== now let's apply LSF SF w.r.t. lepton flavour
              if(Suffix=="SingleElectron"){
                if(HasSFLooseLepton) JetLepFlav = 0;
                else if(HasOFLooseLepton) JetLepFlav = 1;
              }
              else if(Suffix=="SingleMuon"){
                if(HasSFLooseLepton) JetLepFlav = 1;
                else if(HasOFLooseLepton) JetLepFlav = 0;
              }
              if(!IsDATA && JetLepFlav>=0){
                //==== LSF SF
                weight *= LSFSF(JetLepFlav, SystDir_LSFSF);
              }

              //==== 1) e+e-jet or mu+mu-jet

              if(HasSFLooseLepton){

                FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasSFLooseLepton_"+param.Name, weight);

                if(!HasOFLooseLepton){

                  FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoHasOFLooseLepton_"+param.Name, weight);

                  leps_for_plot.push_back( SFLooseLepton );
                  if(tmp_IsLeadM){
                    //==== In this case, the loose ID is HighPt ID muon.
                    //==== we want to apply the lepton scale factors to these muons
                    Muon *looseMuon = (Muon *)SFLooseLepton;
                    ForSF_muons.push_back( looseMuon );
                  }

                  if( (*LeadLep+*SFLooseLepton).M() > 200 ){

                    FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_mllGT200_"+param.Name, weight);

                    if(WRMassGT800){

                      FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_mWRGT800_"+param.Name, weight);

                      //==== Region Dictionary
                      //==== - HNWR_SingleElectron_Boosted_SR : ee Boosted SR [IsBoosted_SR_EE]
                      //==== - HNWR_SingleMuon_Boosted_SR : mm Boosted SR [IsBoosted_SR_MM]

                      map_bool_To_Region[Suffix+"_Boosted_SR"] = true;
                      if(tmp_IsLeadE) IsBoosted_SR_EE = true;
                      else if(tmp_IsLeadM) IsBoosted_SR_MM = true;

                    }
                    else{

                      //==== Region Dictionary
                      //==== - HNWR_SingleElectron_Boosted_LowWRCR : ee Boosted, but low m(WR) [IsBoosted_LowWRCR_EE]
                      //==== - HNWR_SingleMuon_Boosted_LowWRCR : mm Boosted, but low m(WR) [IsBoosted_LowWRCR_MM]

                      map_bool_To_Region[Suffix+"_Boosted_LowWRCR"] = true;
                      if(tmp_IsLeadE) IsBoosted_LowWRCR_EE = true;
                      else if(tmp_IsLeadM) IsBoosted_LowWRCR_MM = true;

                    }

                  }

                }

              }

              //==== 2) e+mu-jet or mu+e-jet

              if(!HasSFLooseLepton){

                FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_NoHasSFLooseLepton_"+param.Name, weight);

                if(HasOFLooseLepton){

                  FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_HasOFLooseLepton_"+param.Name, weight);

                  leps_for_plot.push_back( OFLooseLepton );

                  if( (*LeadLep+*OFLooseLepton).M() > 200 ){

                    FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_mllGT200_"+param.Name, weight);

                    if(WRMassGT800){

                      FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_mWRGT800_"+param.Name, weight);

                      //==== Region Dictionary
                      //==== - HNWR_SingleElectron_EMu_Boosted_CR : isolated e + mu-AK8jet (ttbar dominant) [IsBoosted_CR_EMJet]
                      //==== - HNWR_SingleMuon_EMu_Boosted_CR : isolated m + e-AK8jet (ttbar dominant) [IsBoosted_CR_MEJet]

                      map_bool_To_Region[Suffix+"_EMu_Boosted_CR"] = true;
                      if(tmp_IsLeadE) IsBoosted_CR_EMJet = true;
                      else if(tmp_IsLeadM) IsBoosted_CR_MEJet = true;

                    }
                    else{

                      //==== Region Dictionary
                      //==== - HNWR_SingleElectron_EMu_Boosted_LowWRCR : isolated e + mu-AK8jet (ttbar dominant), but low m(WR) [IsBoosted_LowWRCR_EMJet]
                      //==== - HNWR_SingleMuon_EMu_Boosted_LowWRCR : isolated m + e-AK8jet (ttbar dominant), but low m(WR) [IsBoosted_LowWRCR_MEJet]

                      map_bool_To_Region[Suffix+"_EMu_Boosted_LowWRCR"] = true;
                      if(tmp_IsLeadE) IsBoosted_LowWRCR_EMJet = true;
                      else if(tmp_IsLeadM) IsBoosted_LowWRCR_MEJet = true;

                    }

                  }

                }

              }

            } // END No Extra Tight Lepton

          } // END If has merged jet

        } // END If no m(ll)<150 pair

        //==== Now we can apply trigger SFs, because for mm case, we selected correct loose muons

        double trigger_sf_SingleElectron = 1.;
        double trigger_sf_SingleMuon = 1.;
        if(!IsDATA){
          //==== Trigger SF
          trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons, SystDir_ElectronTriggerSF);
          trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, ForSF_muons, SystDir_MuonTriggerSF); // notice, we used ForSF_muons instead of Tight_muons
        }
        if(Suffix=="SingleElectron"){
          trigger_sf_SingleMuon = trigger_sf_SingleElectron;
          weight *= trigger_sf_SingleElectron;
        }
        else{
          trigger_sf_SingleElectron = trigger_sf_SingleMuon;
          weight *= trigger_sf_SingleMuon;
        }

        //==== DYReshape for DY
        if(ApplyDYReshape){
          weight *= GetDYReshape(WRCand.M(), "Boosted", SystDir_DYReshape);
        }

      } // END If trigger fired

    } // END tight lepton exist

  } // END If not resolved

  if( map_bool_To_Region.size() == 0 ) return;

  //================================================
  //==== Double counting check
  //==== Tight electron and Tight muon near
  //==== HNFatJet
  //==== Tight_electrons, Tight_muons

  if(IsBoosted_SR_EE){
    for(unsigned int i=0; i<Tight_muons.size(); i++){
      if( fabs(Tight_muons.at(i)->DeltaPhi(HNFatJet)) > 2.0){
        FillHist((param.Name)+"/IsBoosted_SR_EE_But_HasTightMuon_"+(param.Name), 0., weight, 1, 0., 1.);
        break;
      }
    }
  }
  if(IsBoosted_SR_MM){
    for(unsigned int i=0; i<Tight_electrons.size(); i++){
      if( fabs(Tight_electrons.at(i)->DeltaPhi(HNFatJet)) > 2.0){
        FillHist((param.Name)+"/IsBoosted_SR_MM_But_HasTightElectron_"+(param.Name), 0., weight, 1, 0., 1.);
        break;

      }
    }
  }
  //================================================

/*
  //==== DY LO vs NLO stduy
  bool IsHighWRMassResolvedDYEvent = (IsResolved_DYCR_EE||IsResolved_DYCR_MM) && (WRCand.M()>3000.);
  bool IsHighWRMassBoostedDYEvent = (IsBoosted_DYCR_EE || IsBoosted_DYCR_MM) && (WRCand.M()>3000.);
  //cout << "@@@@ High mass m(lljj) DY events found" << endl;
  //cout << "m(lljj) = " << WRCand.M() << endl;
  //cout << "m(ll) = " << ( (*leps_for_plot.at(0))+(*leps_for_plot.at(1)) ).M() << endl;

  if(IsHighWRMassResolvedDYEvent){

    cout << "@@@@ IsHighWRMassResolvedDYEvent " << endl;
    cout << "[RECO]" << endl;
    cout << "lep1 : "; leps_for_plot.at(0)->Print();
    cout << "lep2 : "; leps_for_plot.at(1)->Print();
    cout << "jet1 : "; jets.at(0).Print();
    cout << "jet2 : "; jets.at(1).Print();

    cout << "[LHE]" << endl;
    for(unsigned int i=0; i<lhes.size(); i++){
      lhes.at(i).Print();
      int this_ID = lhes.at(i).ID();
      int this_Status = lhes.at(i).Status();
    }

    FillHist("LHEStudy/IsHighWRMassResolvedDYEvent__LHE_lepton_flavour", LHE_lepton_flavour, weight_norm_1invpb, 4, -1., 3.);
    FillHist("LHEStudy/IsHighWRMassResolvedDYEvent__nFinalParton", nFinalParton, weight_norm_1invpb, 10, 0., 10.);
  }
  if(IsHighWRMassBoostedDYEvent){
    FillHist("LHEStudy/IsHighWRMassBoostedDYEvent__LHE_lepton_flavour", LHE_lepton_flavour, weight_norm_1invpb, 4, -1., 3.);
    FillHist("LHEStudy/IsHighWRMassBoostedDYEvent__nFinalParton", nFinalParton, weight_norm_1invpb, 10, 0., 10.);
  }

  return;
*/

  //==== Printing data info
  if(IsDATA){

    if(IsResolved_SR_EE || IsResolved_SR_MM){
      if(WRCand.M() >= 3200.){
        if(IsResolved_SR_EE) cout << "[HighMassDataEvent][IsResolved_SR_EE] " << run << ":" << lumi << ":" << event << "\t" << WRCand.M() << endl;
        if(IsResolved_SR_MM) cout << "[HighMassDataEvent][IsResolved_SR_MM] " << run << ":" << lumi << ":" << event << "\t" << WRCand.M() << endl;
      }
      leps_for_plot.at(0)->Print();
      leps_for_plot.at(1)->Print();
      jets.at(0).Print();
      jets.at(1).Print();
    }
    if(IsBoosted_SR_EE || IsBoosted_SR_MM){
      if(WRCand.M() >= 1800.){
        if(IsBoosted_SR_EE) cout << "[HighMassDataEvent][IsBoosted_SR_EE] " << run << ":" << lumi << ":" << event << "\t" << WRCand.M() << endl;
        if(IsBoosted_SR_MM) cout << "[HighMassDataEvent][IsBoosted_SR_MM] " << run << ":" << lumi << ":" << event << "\t" << WRCand.M() << endl;
      }
      leps_for_plot.at(0)->Print();
      leps_for_plot.at(1)->Print();
      HNFatJet.Print();
    }
  }

  for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

    TString this_region = it_map->first;
    this_region = (param.Name)+"_"+this_region;

    if(it_map->second){

      if(RunXsecSyst && param.syst_ == AnalyzerParameter::Central){

        double normweight = 1./sumW/PDFWeights_Error->at(0) * this_kfactor/this_avg_kfactor;
        for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
          //FillHist(this_region+"/WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
          FillHist("XsecSyst_Num_"+this_region+"/PDFWeights_Scale_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_Scale->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }
        for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
          //FillHist(this_region+"/WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

          FillHist("XsecSyst_Num_"+this_region+"/PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_Error->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }
        for(unsigned int i=0; i<PDFWeights_AlphaS->size(); i++){
          //FillHist(this_region+"/WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

          FillHist("XsecSyst_Num_"+this_region+"/PDFWeights_AlphaS_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_AlphaS->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }

      }

      FillHist(this_region+"/NEvent_"+this_region, 0., weight, 1, 0., 1.);
      FillHist(this_region+"/MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

      FillHist(this_region+"/nPileUp_"+this_region, nPileUp, weight, 200., 0., 200.);
      FillHist(this_region+"/nPV_"+this_region, nPV, weight, 200., 0., 200.);
      FillHist(this_region+"/N_VTX_"+this_region, N_VTX, weight, 200., 0., 200.);

      FillHist(this_region+"/Lepton_Size_"+this_region, leps_for_plot.size(), weight, 10, 0., 10.);

      FillHist(this_region+"/NExtraLooseElectron_"+this_region, NExtraLooseElectron, weight, 10, 0., 10.);
      FillHist(this_region+"/NExtraLooseMuon_"+this_region, NExtraLooseMuon, weight, 10, 0., 10.);
      FillHist(this_region+"/NExtraLooseLepton_"+this_region, NExtraLooseLepton, weight, 10, 0., 10.);

      FillHist(this_region+"/NExtraTightElectron_"+this_region, NExtraTightElectron, weight, 10, 0., 10.);
      FillHist(this_region+"/NExtraTightMuon_"+this_region, NExtraTightMuon, weight, 10, 0., 10.);
      FillHist(this_region+"/NExtraTightLepton_"+this_region, NExtraTightLepton, weight, 10, 0., 10.);

      FillHist(this_region+"/FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
      FillHist(this_region+"/LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      FillHist(this_region+"/FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      FillHist(this_region+"/Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

      FillHist(this_region+"/NBJets_"+this_region, NBJets, weight, 10, 0., 10.);
      FillHist(this_region+"/HT_"+this_region, HT, weight, 4000, 0., 4000.);

      FillHist(this_region+"/PrefireRwg_"+this_region, 0, weight_Prefire, 1, 0., 1.);

      if(this_region.Contains("Boosted")){
        FillHist(this_region+"/dPhi_LeadlJ_"+this_region, fabs( leps_for_plot.at(0)->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);
        FillHist(this_region+"/dR_SubleadlJ_"+this_region, fabs( leps_for_plot.at(1)->DeltaR(HNFatJet) ), weight, 40, 0., 4.);
        FillHist(this_region+"/HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 2000, 0., 2000.);
        FillHist(this_region+"/HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
        FillHist(this_region+"/HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
        FillHist(this_region+"/HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
        FillHist(this_region+"/HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        FillHist(this_region+"/HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        FillHist(this_region+"/HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);
        FillHist(this_region+"/HNFatJet_LSF_"+this_region, HNFatJet.LSF(), weight, 100, 0., 1.);

        if(HEM1516){
          //==== HEM1516
          int sign_eta = HNFatJet.Eta()<0 ? -1 : +1;
          int sign_phi = HNFatJet.Phi()<0 ? -1 : +1;
          TString this_section = "Eta"+TString::Itoa(sign_eta,10)+"_Phi"+TString::Itoa(sign_phi,10);
          FillHist(this_region+"/WRCand_Mass_"+this_section+"_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
        }

      }

      if( leps_for_plot.size()>=2 ){
        FillHist(this_region+"/ZCand_Mass_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).M(), weight, 2000, 0., 2000.);
        FillHist(this_region+"/ZCand_Pt_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt(), weight, 2000, 0., 2000.);
        FillHist(this_region+"/dPhi_ll_"+this_region, fabs((*leps_for_plot.at(0)).DeltaPhi(*leps_for_plot.at(1))), weight, 40, 0., 4.);
      }

      double this_mWR = WRCand.M();
      if(this_mWR<800.) this_mWR = 800.+1.;
      if(this_mWR>=8000. && Signal) this_mWR = 7999.;
      FillHist(this_region+"/WRCand_Mass_"+this_region, this_mWR, weight, 800, 0., 8000.);

      FillHist(this_region+"/WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

      FillHist(this_region+"/NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
      FillHist(this_region+"/NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
      FillHist(this_region+"/NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
      FillHist(this_region+"/NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
      FillHist(this_region+"/NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
      FillHist(this_region+"/NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);

      if(Signal){
        FillHist(this_region+"/TriggerEff_"+this_region, 0., this_trigger_sf, 1, 0., 1.);
      }

      FillLeptonPlots(leps_for_plot, this_region, weight);
      FillJetPlots(jets, fatjets_LSF, this_region, weight);

      if(jets.size()>=2){
        FillHist(this_region+"/DiJet_Mass_"+this_region, (jets.at(0)+jets.at(1)).M(), weight, 400, 0., 4000.);
        FillHist(this_region+"/DiJet_Pt_"+this_region, (jets.at(0)+jets.at(1)).Pt(), weight, 400, 0., 4000.);
      }

      //==== For HEM15/16
      if(HEM1516){
        for(unsigned int i=0; i<fatjets.size(); i++){

          TString this_itoa = TString::Itoa(i,10);
          int sign_eta = fatjets.at(i).Eta()<0 ? -1 : +1;
          int sign_phi = fatjets.at(i).Phi()<0 ? -1 : +1;
          TString this_section = "Eta"+TString::Itoa(sign_eta,10)+"_Phi"+TString::Itoa(sign_phi,10);

          FillHist(this_region+"/FatJet_"+this_itoa+"_Energy_"+this_section+"_"+this_region, fatjets.at(i).E(), weight, 3000, 0., 3000.);
          FillHist(this_region+"/FatJet_"+this_itoa+"_Pt_"+this_section+"_"+this_region, fatjets.at(i).Pt(), weight, 3000, 0., 3000.);
          FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_"+this_section+"_"+this_region, fatjets.at(i).Eta(), weight, 60, -3., 3.);
          FillHist(this_region+"/FatJet_"+this_itoa+"_Mass_"+this_section+"_"+this_region, fatjets.at(i).M(), weight, 3000, 0., 3000.);
          FillHist(this_region+"/FatJet_"+this_itoa+"_SDMass_"+this_section+"_"+this_region, fatjets.at(i).SDMass(), weight, 3000, 0., 3000.);
          FillHist(this_region+"/FatJet_"+this_itoa+"_LSF_"+this_section+"_"+this_region, fatjets.at(i).LSF(), weight, 100, 0., 1.);

        }
      }

      //==== 2020/11/24 : DY Boosted study

      int nAwayFatJet(0);
      for(unsigned int i=0; i<fatjets.size(); i++){
        TString this_itoa = TString::Itoa(i,10);
        double dPhi_Lead = fabs( leps_for_plot.at(0)->DeltaPhi( fatjets.at(i) ) );
        if(dPhi_Lead<0.8) continue;
        nAwayFatJet++;
        FillHist(this_region+"/AwayFatJet_dPhi_LeadlJ_"+this_itoa+"_"+this_region, fabs( leps_for_plot.at(0)->DeltaPhi( fatjets.at(i) ) ), weight, 40, 0., 4.);
        FillHist(this_region+"/AwayFatJet_partonFlavour_"+this_itoa+"_"+this_region, fatjets.at(i).partonFlavour(), weight, 60, -30., 30.);
        FillHist(this_region+"/AwayFatJet_hadronFlavour_"+this_itoa+"_"+this_region, fatjets.at(i).hadronFlavour(), weight, 6, 0., 6.);

        FillHist(this_region+"/AwayFatJet_dPhi_LeadlJ_"+this_region, fabs( leps_for_plot.at(0)->DeltaPhi( fatjets.at(i) ) ), weight, 40, 0., 4.);
        FillHist(this_region+"/AwayFatJet_partonFlavour_"+this_region, fatjets.at(i).partonFlavour(), weight, 60, -30., 30.);
        FillHist(this_region+"/AwayFatJet_hadronFlavour_"+this_region, fatjets.at(i).hadronFlavour(), weight, 6, 0., 6.);

      }
      FillHist(this_region+"/AwayFatJet_Size_"+this_region, nAwayFatJet, weight, 10, 0., 10.);
      for(unsigned int i=0; i<fatjets.size(); i++){

        TString this_itoa = TString::Itoa(i,10);
        FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_"+this_region, fatjets.at(i).Eta(), weight, 60, -3., 3.);
        FillHist(this_region+"/FatJet_"+this_itoa+"_Phi_"+this_region, fatjets.at(i).Phi(), weight, 80, -4., 4.);
        FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_vs_Phi_"+this_region, fatjets.at(i).Eta(), fatjets.at(i).Phi(), weight, 60, -3., 3., 80, -4., 4.);

      }


      if(jets.size()>=2){
        FillHist(this_region+"/dRj1j2_"+this_region, jets.at(0).DeltaR( jets.at(1) ), weight, 60., 0., 6.);
      }

      if( (MCSample.Contains("DYJets")) && IsCentral){
        FillHist(this_region+"/ZPtReweight_"+this_region, ZPtReweight, 1., 40, 0., 4.);
        FillHist(this_region+"/GenZ_Mass_"+this_region, GenZParticle.M(), weight, 800, 0., 8000.);
        FillHist(this_region+"/GenZ_Pt_"+this_region, GenZParticle.Pt(), weight, 800, 0., 8000.);
      }

      FillHist(this_region+"/JetLepFlav_"+this_region, JetLepFlav, 1., 3, -1., 2.);

    } // END if(pass Region)

  } // END Loop mapRegionBool

}

HNWRAnalyzer::HNWRAnalyzer(){

  RunFake = false;

}

HNWRAnalyzer::~HNWRAnalyzer(){

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
    cerr << "[HNWRAnalyzer::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}

void HNWRAnalyzer::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double HNWRAnalyzer::LSFSF(int lepflav, int dir){

  //==== lepflav = 0 : electron-jet
  //==== lepflav = 1 : mu-jet

  //==== 2016
  double LSFSF_EJet_Central = 1.04;
  double LSFSF_EJet_Error_Up = 0.09;
  double LSFSF_EJet_Error_Down = 0.08;

  double LSFSF_MJet_Central = 1.01;
  double LSFSF_MJet_Error_Up = 0.06;
  double LSFSF_MJet_Error_Down = 0.06;

  if(DataYear==2017){

    LSFSF_EJet_Central = 1.02;
    LSFSF_EJet_Error_Up = 0.08;
    LSFSF_EJet_Error_Down = 0.08;

    LSFSF_MJet_Central = 0.98;
    LSFSF_MJet_Error_Up = 0.07;
    LSFSF_MJet_Error_Down = 0.07;

  }
  if(DataYear==2018){

    LSFSF_EJet_Central = 1.11;
    LSFSF_EJet_Error_Up = 0.08;
    LSFSF_EJet_Error_Down = 0.07;

    LSFSF_MJet_Central = 1.06;
    LSFSF_MJet_Error_Up = 0.06;
    LSFSF_MJet_Error_Down = 0.05;

  }

  if(lepflav==0){
    if(dir==0)     return LSFSF_EJet_Central;
    else if(dir>0) return LSFSF_EJet_Central+LSFSF_EJet_Error_Up;
    else           return LSFSF_EJet_Central-LSFSF_EJet_Error_Down;
  }
  else if(lepflav==1){
    if(dir==0)     return LSFSF_MJet_Central;
    else if(dir>0) return LSFSF_MJet_Central+LSFSF_MJet_Error_Up;
    else           return LSFSF_MJet_Central-LSFSF_MJet_Error_Down;
  }
  else{
    cerr << "[HNWRAnalyzer::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}

double HNWRAnalyzer::GetDYReshape(double mwr, TString region, int SystType){

  if(mwr>=8000.) mwr=8000.;

  

  int this_bin = -1;
  if(region=="Resolved"){
    this_bin = hist_DYReshape_Resolved_ratio_AllCh->FindBin(mwr);
  }
  else if(region=="Boosted"){
    this_bin = hist_DYReshape_Boosted_ratio_AllCh->FindBin(mwr);
  }
  else{
    cout << "[HNWRAnalyzer::GetDYReshape] Wrong regoin : " << region << endl;
  }

  //==== central
  if(SystType==0){
    if(region=="Resolved"){
      return hist_DYReshape_Resolved_ratio_AllCh->GetBinContent(this_bin);
    }
    else{
      return hist_DYReshape_Boosted_ratio_AllCh->GetBinContent(this_bin);
    }
  }
  //==== stat up
  else if(SystType==+1){
    if(region=="Resolved"){
      return hist_DYReshape_Resolved_ratio_AllCh->GetBinContent(this_bin) + hist_DYReshape_Resolved_ratio_AllCh->GetBinError(this_bin);
    }
    else{
      return hist_DYReshape_Boosted_ratio_AllCh->GetBinContent(this_bin) + hist_DYReshape_Boosted_ratio_AllCh->GetBinError(this_bin);
    }
  }
  //==== stat down
  else if(SystType==-1){
    if(region=="Resolved"){
      return hist_DYReshape_Resolved_ratio_AllCh->GetBinContent(this_bin) - hist_DYReshape_Resolved_ratio_AllCh->GetBinError(this_bin);
    }
    else{
      return hist_DYReshape_Boosted_ratio_AllCh->GetBinContent(this_bin) - hist_DYReshape_Boosted_ratio_AllCh->GetBinError(this_bin);
    }
  }
  //==== ee-only
  else if(SystType==+2){
    if(region=="Resolved"){
      return hist_DYReshape_Resolved_EEOnlyRatio->GetBinContent(this_bin);
    }
    else{
      return hist_DYReshape_Boosted_EEOnlyRatio->GetBinContent(this_bin);
    }
  }
  //==== mm-only
  else if(SystType==-2){
    if(region=="Resolved"){
      return hist_DYReshape_Resolved_MuMuOnlyRatio->GetBinContent(this_bin);
    }
    else{
      return hist_DYReshape_Boosted_MuMuOnlyRatio->GetBinContent(this_bin);
    }
  }
  else{
    cout << "[HNWRAnalyzer::GetDYReshape] Wrong input; mwr = " << mwr << ", region = " << region << ", SystType = " << SystType << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}

double HNWRAnalyzer::GetHNWRKFactor(double mlN){

  if(mlN<200.) mlN = 200.+1.;
  int this_bin = hist_kfactor->FindBin(mlN);
  return hist_kfactor->GetBinContent(this_bin);

}

