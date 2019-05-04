#include "GenFinderForDY.h"

GenFinderForDY::GenFinderForDY(){

  FoundGenZ = false;

}

GenFinderForDY::~GenFinderForDY(){
}

Particle GenFinderForDY::Find(vector<Gen>& gens){

  FoundGenZ = false;
  MethodUsed = -1;

  //==== Try to find Z boson directly by PID

  Gen gen_Z;
  Particle ptl_Z;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if( abs(gen.PID())==23 ){
      //==== nan check
      //==== index PID Status  MIdx  MPID  Start Pt  Eta Phi M
      //==== 4 23  22  2 -1  4 0.00  -100000000000.00  0.00  -nan
      //==== 5 23  44  4 23  4 1.82  -7.33 -1.06 85.88
      //==== 6 23  62  5 23  4 2.09  -7.19 0.61  85.88
      if( gen.M()==gen.M() ){
        gen_Z = gen;
        ptl_Z = gen;
        MethodUsed = 0;
        FoundGenZ = true;
        break;
      }
    }
  }

  vector<Gen> gen_leptons;

  //=== If Z found
  if(!gen_Z.IsEmpty()){

    //==== find the two lepton whos mother is Z boson found
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at( gen.MotherIndex() );
      if( abs(mother.PID())==gen_Z.PID() ){
        if( IsLepton( gen.PID() ) ){
          gen_leptons.push_back( gen );
        }
      }
    }
    //==== safe guard; make sure we have to leptons
    if(gen_leptons.size()!=2){
      FoundGenZ = false;
      //FillHist("ZFound_notTwolepton", 1., 0., 1, 0., 1.);
    }
    else{
      //==== safe guard; make sure both lepton has same flavour
      if(gen_leptons.at(0).PID()+gen_leptons.at(1).PID()==0){
        MethodUsed = 1;
        FoundGenZ = true;
        LeptonPID = abs( gen_leptons.at(0).PID() );
        ptl_Z = gen_leptons.at(0)+gen_leptons.at(1);
      }
      else{
        FoundGenZ = false;
      }
    }

  }

  //==== Still no?

  //==== Case 1 )
  //==== RunNumber:EventNumber = 1:101380086
  //==== index PID Status  MIdx  MPID  Start Pt  Eta Phi M
  //==== 2 1 21  0 2212  2 0.00  -100000000000.00  0.00  -nan
  //==== 3 -1  21  1 2212  3 0.00  -100000000000.00  0.00  -nan
  //==== 4 -13 23  2 1 4 21.58 1.43  3.01  0.11
  //==== 5 13  23  2 1 5 21.58 0.06  -0.13 0.11
  //==== 6 13  44  5 13  5 17.61 -0.21 0.20  0.11
  //==== Somtines it's gamma

  if(!FoundGenZ){

    gen_leptons.clear();
    for(unsigned int i=2; i<gens.size(); i++){
      if(gen_leptons.size()==2) break;
      Gen gen = gens.at(i);
      Gen mother = gens.at( gen.MotherIndex() );
      if( IsLepton( gen.PID() ) ){
        gen_leptons.push_back( gen );
      }
    }

    //==== safe guard; make sure we have to leptons
    if(gen_leptons.size()!=2){
      FoundGenZ = false;
      //FillHist("ZFound_notTwolepton", 1., 0., 1, 0., 1.);
    }
    else{
      //==== safe guard; make sure both lepton has same flavour
      if(gen_leptons.at(0).PID()+gen_leptons.at(1).PID()==0){
        MethodUsed = 2;
        FoundGenZ = true;
        LeptonPID = abs( gen_leptons.at(0).PID() );
        ptl_Z = gen_leptons.at(0)+gen_leptons.at(1);
      }
      else{
        FoundGenZ = false;
      }
    }

  }


  return ptl_Z;

}

bool GenFinderForDY::IsLepton(int pid){

  return ( abs(pid)==11 || abs(pid)==13 || abs(pid)==15 );

}


