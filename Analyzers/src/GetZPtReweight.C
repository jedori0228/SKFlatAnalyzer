#include "GetZPtReweight.h"

void GetZPtReweight::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

  DoLowPt = HasFlag("DoLowPt");

}

void GetZPtReweight::executeEvent(){


  AnalyzerParameter param;

  Event ev = GetEvent();
  double this_MCweight = GetEvent().MCweight() * weight_norm_1invpb;

  vector<Gen> gens = GetGens();

  genFinderDY->Find(gens);
  Particle genZ = genFinderDY->GenZ;

  double mZ = genZ.M();
  double ptZ = genZ.Pt();

  FillHist("GenZ_Mass", mZ, this_MCweight, 200., 0., 2000.);
  FillHist("GenZ_Pt", ptZ, this_MCweight, 200., 0., 2000.);

  //vector<double> vec_massBins = {50, 60, 70, 80, 85, 86, 87, 88, 88.5, 89, 89.2, 89.4, 89.6, 89.8, 90, 90.2, 90.4, 90.6, 90.8, 91, 91.2, 91.4, 91.6, 91.8, 92, 92.2, 92.4, 92.6, 92.8, 93, 93.2, 93.4, 93.6, 93.8, 94, 95, 100, 110, 120, 180, 250, 500, 1000};
  vector<double> vec_massBins = {50, 60, 70, 80, 85, 86, 87, 88, 88.5, 89, 89.2, 89.4, 89.6, 89.8, 90, 90.2, 90.4, 90.6, 90.8, 91, 91.2, 91.4, 91.6, 91.8, 92, 92.2, 92.4, 92.6, 92.8, 93, 93.2, 93.4, 93.6, 93.8, 94, 95, 100, 110, 120, 180, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000};
  vector<double> vec_ptBins = {0, 2, 4, 6, 8, 10, 12, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 225, 250, 350, 500, 1000, 2000, 8000};

  const int NmassBin = vec_massBins.size()-1;
  const int NptBin = vec_ptBins.size()-1;

  double massBins[NmassBin+1];
  double ptBins[NptBin+1];

  for(unsigned int i=0; i<vec_massBins.size(); i++) massBins[i] = vec_massBins.at(i);
  for(unsigned int i=0; i<vec_ptBins.size(); i++) ptBins[i] = vec_ptBins.at(i);

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

  FillHist("MassVSPt", mZ, ptZ, this_MCweight,NmassBin, massBins, NptBin, ptBins);

}

GetZPtReweight::GetZPtReweight(){

}

GetZPtReweight::~GetZPtReweight(){

}


