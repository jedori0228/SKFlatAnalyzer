#ifndef GenFinderForDY_h
#define GenFinderForDY_h

#include "TString.h"
#include <iostream>
#include "Gen.h"

using namespace std;

class GenFinderForDY{

public:

  GenFinderForDY();
  ~GenFinderForDY();

  void Run(vector<Gen> gens);

};

#endif
