#!/bin/bash

#for Year in 2016 2017 2018
for Year in 2016
do

  SKFlat.py -a HNWRAnalyzer -l samples_WR_Official.txt -n 1 -y ${Year} --userflags RunSyst,Signal,SignalElectronOnly,RunXsecSyst,WROfficial --batchname Signal_${Year}_EE &
  sleep 150
  SKFlat.py -a HNWRAnalyzer -l samples_WR_Official.txt -n 1 -y ${Year} --userflags RunSyst,Signal,SignalMuonOnly,RunXsecSyst,WROfficial --batchname Signal_${Year}_MuMu &
  sleep 150

  #SKFlat.py -a HNWRAnalyzer -l samples_WR_Official.txt -n 1 -y ${Year} --userflags HEMElectronVeto,RunSyst,Signal,SignalElectronOnly,RunXsecSyst,WROfficial --batchname Signal_${Year}_EE &
  #sleep 150
  #SKFlat.py -a HNWRAnalyzer -l samples_WR_Official.txt -n 1 -y ${Year} --userflags HEMElectronVeto,RunSyst,Signal,SignalMuonOnly,RunXsecSyst,WROfficial --batchname Signal_${Year}_MuMu &
  #sleep 150

done
