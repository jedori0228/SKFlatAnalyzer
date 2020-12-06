#include "SkimTree_WROfficial.h"

void SkimTree_WROfficial::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_WROfficial::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  LHAPDFHandler_.CentralPDFName = "NNPDF31_nnlo_hessian_pdfas";
  LHAPDFHandler_.ErrorSetMember_Start = 1;
  LHAPDFHandler_.ErrorSetMember_End = 100;
  LHAPDFHandler_.AlphaSMember_Down = 101;
  LHAPDFHandler_.AlphaSMember_Up = 102;
  LHAPDFHandler_.init();

}

void SkimTree_WROfficial::executeEvent(){

  FillHist("mWR", genMWR, 1., 600, 0., 6000);

  TString targetMWR = Userflags.at(0);
  TString targetmN  = Userflags.at(1);

  if( targetMWR != TString::Itoa(genMWR,10) ) return;
  if( targetmN  != TString::Itoa(genMN, 10) ) return;

  double pdf_Central = LHAPDFHandler_.PDFCentral->xfxQ(genWeight_id1, genWeight_X1, genWeight_Q);
  PDFWeights_Error->clear();
  for(int i=0; i<100; i++){
    double this_pdf = LHAPDFHandler_.PDFErrorSet.at(i)->xfxQ(genWeight_id1, genWeight_X1, genWeight_Q);
    double thie_rwg = this_pdf/pdf_Central;
    PDFWeights_Error->push_back(thie_rwg);
  }
  double this_pdfAs_Up = LHAPDFHandler_.PDFAlphaSUp->xfxQ(genWeight_id1, genWeight_X1, genWeight_Q);
  double this_pdfAs_Dn = LHAPDFHandler_.PDFAlphaSDown->xfxQ(genWeight_id1, genWeight_X1, genWeight_Q);

  double this_AsUpRwg = 1.+(this_pdfAs_Up - pdf_Central)/pdf_Central * 0.75;
  double this_AsDnRwg = 1.+(this_pdfAs_Dn - pdf_Central)/pdf_Central * 0.75;

  PDFWeights_AlphaS->push_back(this_AsDnRwg);
  PDFWeights_AlphaS->push_back(this_AsUpRwg);

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_WROfficial::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_WROfficial::SkimTree_WROfficial(){

  newtree = NULL;

}

SkimTree_WROfficial::~SkimTree_WROfficial(){

}

void SkimTree_WROfficial::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


