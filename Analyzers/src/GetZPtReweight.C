#include "GetZPtReweight.h"

void GetZPtReweight::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

  DoLowPt = HasFlag("DoLowPt");


  //==== Test new PDF

  NewPDF = HasFlag("NewPDF");  

  LHAPDFHandler LHAPDFHandler_Prod;
  LHAPDFHandler_Prod.CentralPDFName = "NNPDF30_lo_as_0130";
  LHAPDFHandler_Prod.init();

  LHAPDFHandler LHAPDFHandler_New;
  LHAPDFHandler_New.CentralPDFName = "NNPDF31_nnlo_hessian_pdfas";
  LHAPDFHandler_New.init();

  pdfReweight->SetProdPDF( LHAPDFHandler_Prod.PDFCentral );
  pdfReweight->SetNewPDF( LHAPDFHandler_New.PDFCentral );

}

void GetZPtReweight::executeEvent(){

  AnalyzerParameter param;

  Event ev = GetEvent();
  double this_MCweight = GetEvent().MCweight() * weight_norm_1invpb;
  if(NewPDF){
    double PDFReweight = GetPDFReweight();
    //cout << "PDFReweight = " << PDFReweight << endl;
    this_MCweight *= PDFReweight;
  }

  //vector<double> vec_massBins = {50, 60, 70, 80, 85, 86, 87, 88, 88.5, 89, 89.2, 89.4, 89.6, 89.8, 90, 90.2, 90.4, 90.6, 90.8, 91, 91.2, 91.4, 91.6, 91.8, 92, 92.2, 92.4, 92.6, 92.8, 93, 93.2, 93.4, 93.6, 93.8, 94, 95, 100, 110, 120, 180, 250, 500, 1000};
  vector<double> vec_massBins = {50, 60, 70, 80, 85, 86, 87, 88, 88.5, 89, 89.2, 89.4, 89.6, 89.8, 90, 90.2, 90.4, 90.6, 90.8, 91, 91.2, 91.4, 91.6, 91.8, 92, 92.2, 92.4, 92.6, 92.8, 93, 93.2, 93.4, 93.6, 93.8, 94, 95, 100, 110, 120, 180, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000};
  vector<double> vec_ptBins = {0, 2, 4, 6, 8, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 225, 250, 350, 500, 1000, 2000, 8000};

  const int NmassBin = vec_massBins.size()-1;
  const int NptBin = vec_ptBins.size()-1;

  double massBins[NmassBin+1];
  double ptBins[NptBin+1];

  for(unsigned int i=0; i<vec_massBins.size(); i++) massBins[i] = vec_massBins.at(i);
  for(unsigned int i=0; i<vec_ptBins.size(); i++) ptBins[i] = vec_ptBins.at(i);

  //==== Check LHE

  vector<LHE> lhes = GetLHEs();

  int LHE_lepton_flavour = -1; // 0 : ee, 1 : mm, 2 : tt
  int nFinalParton = 0;

  vector<Particle> lhe_leps;
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
      else{
        continue;
      }

      lhe_leps.push_back( lhes.at(i) );

    } // END if status 1
  }

  Particle lhe_Z;
  if(lhe_leps.size()==2){
    lhe_Z = lhe_leps.at(0) + lhe_leps.at(1);
    FillHist("LHE_Mass", lhe_Z.M(), this_MCweight, 200, 0., 2000.);
    FillHist("LHE_Pt", lhe_Z.Pt(), this_MCweight, 200, 0., 2000.);
    FillHist("LHE_Eta", lhe_Z.Eta(), this_MCweight, 60, -3, 3.);
  }
  else{
    FillHist("NoLHEDiLepFound", 0., 1., 1, 0., 1.);
  }

  vector<Gen> gens = GetGens();

  genFinderDY->Find(gens);
  Particle genZ = genFinderDY->GenZ;

  double mZ = genZ.M();
  double ptZ = genZ.Pt();

  FillHist("Gen_Q", genWeight_Q, this_MCweight, 200., 0., 2000.);

  FillHist("GenZ_Mass", mZ, this_MCweight, 200., 0., 2000.);
  FillHist("GenZ_Pt", ptZ, this_MCweight, 200., 0., 2000.);
  FillHist("GenZ_Eta", genZ.Eta(), this_MCweight, 60, -3, 3.);

  if(mZ<50.) return;

  if(mZ>=1000.){
    //mZ=999.;
    //return;
  }
  if(ptZ>=1000.){
    //ptZ=999.;
    //return;
  }

  if(DoLowPt){
    if(ptZ>=200.) return;
  }
  else{
    if(ptZ<200.) return;
  }

  FillHist("MassVSPt",     mZ,        ptZ,        this_MCweight,NmassBin, massBins, NptBin, ptBins);
  FillHist("LHE_MassVSPt", lhe_Z.M(), lhe_Z.Pt(), this_MCweight,NmassBin, massBins, NptBin, ptBins);

}

GetZPtReweight::GetZPtReweight(){

}

GetZPtReweight::~GetZPtReweight(){

}


