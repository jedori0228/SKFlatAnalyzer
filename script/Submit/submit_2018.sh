#!/bin/bash

echo "@@ Submitting HNWRAnalyzer, 2018...."

#### 2018 ####
SKFlat.py -a HNWRAnalyzer -l samples_DATA_SingleMuon_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname SingleMuon_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DATA_EGamma_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname EGamma_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_long_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname MC_long_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_SkimTree_LRSMHighPt_MC_short_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname MC_short_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_WHT_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname WHT_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst --nmax 50 --batchname DYMG_2018 &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst,ApplyDYPtReweight,QCDError  --nmax 50 --batchname DYMG_2018_DYPtRew &
sleep 10
SKFlat.py -a HNWRAnalyzer -l samples_DYMG_2018.txt -n 10  --skim SkimTree_LRSMHighPt -y 2018 --userflags RunSyst,ApplyDYPtReweight,QCDError,ApplyDYReshape,JetPt  --nmax 50 --batchname DYMG_2018_DYPtRewReshape &
sleep 10

echo "@@ --> HNWRAnalyzer, 2018.... done"
