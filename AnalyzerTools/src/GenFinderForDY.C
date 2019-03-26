#include "GenFinderForDY.h"

GenFinderForDY::GenFinderForDY(){

}

GenFinderForDY::~GenFinderForDY(){
}

void GenFinderForDY::Run(vector<Gen> gens){

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
        break;
      }
    }
  }

  int LeptonPID = -1;
  vector<Gen> gen_leptons;

  //=== If Z found
  if(!gen_Z.IsEmpty()){

    //==== find the two lepton whos mother is Z boson found
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at( gen.MotherIndex() );
      if( abs(mother.PID())==23 ){
        if( abs(gen.PID())==11 || abs(gen.PID())==13 || abs(gen.PID())==15 ){
          gen_leptons.push_back( gen );
        }
      }
    }
    //==== safe guard; make sure we have to leptons
    if(gen_leptons.size()!=2){
      //FillHist("ZFound_notTwolepton", 1., 0., 1, 0., 1.);
    }
    else{
      //==== safe guard; make sure both lepton has same flavour
      if(gen_leptons.at(0).PID()+gen_leptons.at(1).PID()==0){
        LeptonPID = abs( gen_leptons.at(0).PID() );
      }
    }

  }

}
