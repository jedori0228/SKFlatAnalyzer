#!/bin/bash

echo "@@ Submitting HNWRAnalyzer, 2016...."

#### 2016 ####
SKFlat.py -a HNWRAnalyzer -l samples_DATA_SingleMuon_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname SingleMuon_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DATA_SingleElectron_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname SingleElectron_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_long_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname MC_long_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_short_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname MC_short_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_WHT_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname WHT_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst --nmax 50 --batchname DYMG_2016 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst,ApplyDYPtReweight,QCDError  --nmax 50 --batchname DYMG_2016_DYPtRew &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2016.txt -n 10  --skim SkimTree_LRSMHighPt -y 2016 --userflags RunSyst,ApplyDYPtReweight,QCDError,ApplyDYReshape,JetPt  --nmax 50 --batchname DYMG_2016_DYPtRewReshape &
sleep 10

echo "@@ --> HNWRAnalyzer, 2016.... done" 
