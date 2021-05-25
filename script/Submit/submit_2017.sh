#!/bin/bash

echo "@@ Submitting HNWRAnalyzer, 2017...."

#### 2017 ####
SKFlat.py -a HNWRAnalyzer -l samples_DATA_SingleMuon_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname SingleMuon_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DATA_SingleElectron_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname SingleElectron_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_long_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname MC_long_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_short_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname MC_short_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_WHT_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname WHT_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst --nmax 50 --batchname DYMG_2017 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst,ApplyDYPtReweight,QCDError  --nmax 50 --batchname DYMG_2017_DYPtRew &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2017.txt -n 10  --skim SkimTree_LRSMHighPt -y 2017 --userflags RunSyst,ApplyDYPtReweight,QCDError,ApplyDYReshape,JetPt  --nmax 50 --batchname DYMG_2017_DYPtRewReshape &
sleep 10

echo "@@ --> HNWRAnalyzer, 2017.... done" 
