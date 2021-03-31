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

  vector<double> vec_ptBins = {0, 2, 4, 6, 8, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 225, 250, 350, 500, 1000, 2000, 8000};

  const int NptBin = vec_ptBins.size()-1;

  double ptBins[NptBin+1];

  for(unsigned int i=0; i<vec_ptBins.size(); i++) ptBins[i] = vec_ptBins.at(i);

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

/*
  if(DoLowPt){
    if(ptZ>=200.) return;
  }
  else{
    if(ptZ<200.) return;
  }
*/
 
  //==== Central
  FillHist("Pt_Central", ptZ, this_MCweight, NptBin, ptBins);
  //==== Scale
  for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
    double this_weight = PDFWeights_Scale->at(i);
    if(this_weight!=this_weight) this_weight = 1.;
    FillHist("Pt_Scale"+TString::Itoa(i,10), ptZ, this_MCweight * this_weight, NptBin, ptBins);
  }
  //==== PDF Error
  for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
    double this_weight = PDFWeights_Error->at(i);
    if(this_weight!=this_weight) this_weight = 1.;
    FillHist("Pt_PDFError"+TString::Itoa(i,10), ptZ, this_MCweight * this_weight, NptBin, ptBins);
  }
  //==== AlphaS
  for(unsigned int i=0; i<PDFWeights_AlphaS->size(); i++){
    double this_weight = PDFWeights_AlphaS->at(i);
    if(this_weight!=this_weight) this_weight = 1.;
    FillHist("Pt_AlphaS"+TString::Itoa(i,10), ptZ, this_MCweight * this_weight, NptBin, ptBins);
  }

}

GetZPtReweight::GetZPtReweight(){

}

GetZPtReweight::~GetZPtReweight(){

}


