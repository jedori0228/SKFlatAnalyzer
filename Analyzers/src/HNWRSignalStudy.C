#include "HNWRSignalStudy.h"

void HNWRSignalStudy::initializeAnalyzer(){

  RunNewPDF = HasFlag("RunNewPDF");
  cout << "[ExampleRun::initializeAnalyzer] RunNewPDF = " << RunNewPDF << endl;
  if(RunNewPDF && !IsDATA){

    LHAPDFHandler LHAPDFHandler_Prod;
    LHAPDFHandler_Prod.CentralPDFName = "NNPDF23_lo_as_0130_qed";
    LHAPDFHandler_Prod.init();

    LHAPDFHandler LHAPDFHandler_New;
    LHAPDFHandler_New.CentralPDFName = "NNPDF31_nnlo_hessian_pdfas";
    LHAPDFHandler_New.init();

    pdfReweight->SetProdPDF( LHAPDFHandler_Prod.PDFCentral );
    pdfReweight->SetNewPDF( LHAPDFHandler_New.PDFCentral );

  }

  genFinderSig = new GenFinderForHNWRSignal();

}

void HNWRSignalStudy::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void HNWRSignalStudy::executeEventFromParameter(AnalyzerParameter param){

  Event ev = GetEvent();
  double this_weight = GetEvent().MCweight();
  FillHist("NoCut", 0., 1., 1, 0., 1.);
  vector<Gen> gens = GetGens();

  //==== Find Gen objects

  genFinderSig->Run(gens);

  FillHist("M_gen_WR", genFinderSig->WR.M(), 1., 6000, 0., 6000.);
  FillHist("Pt_gen_WR", genFinderSig->WR.Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_WR", genFinderSig->WR.Eta(), 1., 120, -6., 6.);

  FillHist("M_gen_N", genFinderSig->N.M(), 1., 12000, 0., 1200.);
  FillHist("Pt_gen_N", genFinderSig->N.Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_N", genFinderSig->N.Eta(), 1., 120, -6., 6.);
  FillHist("genFinderSig->SignalLeptonChannel", genFinderSig->SignalLeptonChannel, 1., 2, 0., 2.);

  Particle gen_priLepAndN = genFinderSig->N + genFinderSig->priLep;
  FillHist("M_gen_priLepAndN", gen_priLepAndN.M(), 1., 6000, 0., 6000.);
  FillHist("Pt_gen_priLepAndN", gen_priLepAndN.Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_priLepAndN", gen_priLepAndN.Eta(), 1., 120, -6., 6.);

  FillHist("Pt_gen_priLep", genFinderSig->priLep.Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_priLep", genFinderSig->priLep.Eta(), 1., 120, -6., 6.);

  FillHist("Pt_gen_secLep", genFinderSig->secLep.Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_secLep", genFinderSig->secLep.Eta(), 1., 120, -6., 6.);

  vector<Gen> tmp_gen_jets; //genFinderSig->jet1, genFinderSig->jet2;
  tmp_gen_jets.push_back( genFinderSig->jet1 );
  tmp_gen_jets.push_back( genFinderSig->jet2 );

  genFinderSig->jet1 = tmp_gen_jets.at(0);
  genFinderSig->jet2 = tmp_gen_jets.at(1);
  FillHist("M_gen_WRStar", (tmp_gen_jets.at(0)+tmp_gen_jets.at(1)).M(), 1., 6000, 0., 6000.);
  FillHist("Pt_gen_WRStar", (tmp_gen_jets.at(0)+tmp_gen_jets.at(1)).Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_WRStar", (tmp_gen_jets.at(0)+tmp_gen_jets.at(1)).Eta(), 1., 120, -6., 6.);

  FillHist("Pt_gen_LeadJet", tmp_gen_jets.at(0).Pt(), 1., 6000, 0., 6000.);
  FillHist("Pt_gen_SubleadJet", tmp_gen_jets.at(1).Pt(), 1., 6000, 0., 6000.);
  FillHist("Eta_gen_LeadJet", tmp_gen_jets.at(0).Eta(), 1., 120, -6., 6.);
  FillHist("Eta_gen_SubleadJet", tmp_gen_jets.at(1).Eta(), 1., 120, -6., 6.);

  FillHist("dR_gen_Jet_gen_secLep", tmp_gen_jets.at(0).DeltaR( genFinderSig->secLep ), 1., 60, 0., 6.);
  FillHist("dR_gen_Jet_gen_secLep", tmp_gen_jets.at(1).DeltaR( genFinderSig->secLep ), 1., 60, 0., 6.);
  FillHist("dR_gen_WRStar", tmp_gen_jets.at(0).DeltaR( tmp_gen_jets.at(1) ), 1., 60, 0., 6.);
  FillHist("dR_gen_WRStar_gen_secLep", (tmp_gen_jets.at(0)+tmp_gen_jets.at(1)).DeltaR( genFinderSig->secLep ), 1., 60, 0., 6.);

  FillHist("GenAllFound", genFinderSig->GenAllFound, 1., 2, 0., 2.);
  FillHist("Is_gen_priLep_Lead", genFinderSig->priLep.Pt() > genFinderSig->secLep.Pt(), 1., 2, 0., 2.);

  //==========
  //=== MAIN
  //==========

  //==== RECO vs GEN matching

  vector<Electron> allElectrons = GetElectrons("HNWRTight", 53., 2.4);
  vector<Muon> allMuons = GetMuons("HNWRTight", 53., 2.4);
  vector<Jet> allJets = GetJets("HN", 40., 2.4);
  vector<FatJet> allFatjets = GetFatJets("HN", 200., 2.4);
  vector<FatJet> fatjets_LSF = GetFatJets("HNLSF", 200., 2.4);

  vector<Electron> looseElectrons = GetElectrons("HNWRLoose", 53., 2.4);
  vector<Muon> looseMuons = GetMuons("HNWRLoose", 53., 2.4);

  double dRmin = 0.1;

  bool genLepCh = genFinderSig->SignalLeptonChannel;

  TString str_LeptonFlav = "";
  TString GENLepChannel = "";
  std::vector<Lepton *> recoLeps;
  vector<Lepton *> looseLeps;
  if(genLepCh==0){
    str_LeptonFlav = "Electron";
    GENLepChannel = "EE";
    recoLeps = MakeLeptonPointerVector(allElectrons);
    looseLeps = MakeLeptonPointerVector(looseElectrons);
  }
  else{
    str_LeptonFlav = "Muon";
    GENLepChannel = "MuMu";
    recoLeps = MakeLeptonPointerVector(allMuons);
    looseLeps = MakeLeptonPointerVector(looseMuons);
  }
  FillHist(GENLepChannel+"/NoCut_"+GENLepChannel, 0., this_weight, 1, 0., 1.);

  //==== find prilep matched reco
  Lepton matched_priLep;
  int index_priLep;
  bool found_priLep = false;
  for(unsigned int i=0; i<recoLeps.size(); i++){
    if(recoLeps.at(i)->DeltaR( genFinderSig->priLep ) < dRmin){
      dRmin = recoLeps.at(i)->DeltaR( genFinderSig->priLep );
      matched_priLep = *(recoLeps.at(i));
      index_priLep = i;
      found_priLep = true;
    }
  }
  if(found_priLep){
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_priLep__RelIso_"+GENLepChannel, matched_priLep.RelIso(), this_weight, 100, 0., 1.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_priLep__Pt_"+GENLepChannel, matched_priLep.Pt(), this_weight, 5000, 0., 5000.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_priLep__Eta_"+GENLepChannel, matched_priLep.Eta(), this_weight, 60, -3., 3.);
  }

  //==== find seclep matched reco
  dRmin = 0.1;
  Lepton matched_secLep;
  int index_secLep;
  bool found_secLep = false;
  for(unsigned int i=0; i<recoLeps.size(); i++){
    if(recoLeps.at(i)->DeltaR( genFinderSig->secLep ) < dRmin){
      dRmin = recoLeps.at(i)->DeltaR( genFinderSig->secLep );
      matched_secLep = *(recoLeps.at(i));
      index_secLep = i;
      found_secLep = true;
    }
  }
  if(found_secLep){
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_secLep__RelIso_"+GENLepChannel, matched_secLep.RelIso(), this_weight, 100, 0., 1.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_secLep__Pt_"+GENLepChannel, matched_secLep.Pt(), this_weight, 5000, 0., 5000.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_secLep__Eta_"+GENLepChannel, matched_secLep.Eta(), this_weight, 60, -3., 3.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_secLep__dR_gen_jets_"+GENLepChannel, matched_secLep.DeltaR( tmp_gen_jets.at(0) ), this_weight, 60, 0., 6.);
    FillHist(GENLepChannel+"/GenStudy__lep_mathced_gen_secLep__dR_gen_jets_"+GENLepChannel, matched_secLep.DeltaR( tmp_gen_jets.at(1) ), this_weight, 60, 0., 6.);
  }

  //==== find AK4 jet
  dRmin = 0.1;
  Jet matched_jet1;
  bool found_jet1 = false;
  for(unsigned int i=0; i<allJets.size(); i++){
    if(allJets.at(i).DeltaR( genFinderSig->jet1 ) < dRmin){
      dRmin = allJets.at(i).DeltaR( genFinderSig->jet1 );
      matched_jet1 = allJets.at(i);
      found_jet1 = true;
    }
  }
  if(found_jet1){
    FillHist(GENLepChannel+"/GenStudy__jet_mathced_gen_Jet__Pt_"+GENLepChannel, matched_jet1.Pt(), this_weight, 5000, 0., 5000.);
    FillHist(GENLepChannel+"/GenStudy__jet_mathced_gen_Jet__Eta_"+GENLepChannel, matched_jet1.Eta(), this_weight, 60, -3., 3.);
  }

  dRmin = 0.1;
  Jet matched_jet2;
  bool found_jet2 = false;
  for(unsigned int i=0; i<allJets.size(); i++){
    if(allJets.at(i).DeltaR( genFinderSig->jet2 ) < dRmin){
      dRmin = allJets.at(i).DeltaR( genFinderSig->jet2 );
      matched_jet2 = allJets.at(i);
      found_jet2 = true;
    }
  }
  if(found_jet2){
    FillHist(GENLepChannel+"/GenStudy__jet_mathced_gen_Jet__Pt_"+GENLepChannel, matched_jet2.Pt(), this_weight, 5000, 0., 5000.);
    FillHist(GENLepChannel+"/GenStudy__jet_mathced_gen_Jet__Eta_"+GENLepChannel, matched_jet2.Eta(), this_weight, 60, -3., 3.);
  }

  //=== AK8jet, but without LSF
  dRmin = 0.15;
  FatJet matched_fatjet;
  bool found_fatjet = false;
  for(unsigned int i=0; i<allFatjets.size(); i++){
    if(allFatjets.at(i).DeltaR( genFinderSig->N ) < dRmin){
      dRmin = allFatjets.at(i).DeltaR( genFinderSig->N );
      matched_fatjet = allFatjets.at(i);
      found_fatjet = true;
    }
  }

  //==== gen-boosted
  //==== Gen gen_WR, genFinderSig->N, genFinderSig->priLep, genFinderSig->secLep, genFinderSig->jet1, genFinderSig->jet2;
  bool IsGenResolved = genFinderSig->secLep.DeltaR( genFinderSig->jet1 ) > 0.4 && genFinderSig->secLep.DeltaR( genFinderSig->jet2 ) > 0.4 && genFinderSig->jet1.DeltaR( genFinderSig->jet2 ) > 0.4;
  bool GenAcceptance =    ( genFinderSig->priLep.Pt() >= 53. && fabs(genFinderSig->priLep.Eta())<2.4 ) 
                       && ( genFinderSig->secLep.Pt() >= 53. && fabs(genFinderSig->secLep.Eta())<2.4 )
                       && ( genFinderSig->N.Pt() > 200. && fabs(genFinderSig->N.Eta()) < 2.4 )
                       && ( genFinderSig->N.DeltaR( genFinderSig->secLep ) < 0.8 );

  FillHist("IsGenResolved", IsGenResolved, this_weight, 2, 0., 2.);
  if(found_fatjet && !IsGenResolved && GenAcceptance){

    //==== check loose lepton exist
    bool looseLepFound = false;
    for(unsigned int i=0; i<looseLeps.size(); i++){
      if( matched_fatjet.DeltaR( *looseLeps.at(i) ) < 0.8 ){
        looseLepFound = true;
        break;
      }
    }

    //looseLepFound = true;

    if(found_secLep) FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__dR_lep_mathced_gen_secLep_"+GENLepChannel, matched_fatjet.DeltaR(matched_secLep), this_weight, 60, 0., 6.);

    if(looseLepFound){

      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__NotEmpty_"+GENLepChannel, 0., this_weight, 1, 0., 1.);
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__SDMass_"+GENLepChannel, matched_fatjet.SDMass(), this_weight, 6000, 0., 6000.);
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__LSF_"+GENLepChannel, matched_fatjet.LSF(), this_weight, 100, 0., 1.);
      double reldpf = fabs( matched_fatjet.Pt() - genFinderSig->N.Pt() )/ genFinderSig->N.Pt();
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__reldpt_"+GENLepChannel, reldpf, this_weight, 200, 0., 2.);
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__Pt_"+GENLepChannel, matched_fatjet.Pt(), this_weight, 5000, 0., 5000.);
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__Eta_"+GENLepChannel, matched_fatjet.Eta(), this_weight, 60, -3., 3.);
      FillHist(GENLepChannel+"/GenStudy__fatjet_matched_gen_N__dRgen_"+GENLepChannel, matched_fatjet.DeltaR(genFinderSig->N), this_weight, 60, 0., 6.);

    }

  } // FatJet Study


}

HNWRSignalStudy::HNWRSignalStudy(){

}

HNWRSignalStudy::~HNWRSignalStudy(){

}


