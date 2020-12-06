import os

Years = [
"2016",
"2017",
"2018",
]

MakeRunScript = True

massPoints = '500to3500'
massPoints = '3500to5000'
massPoints = '5000to6000'

NJobPerMass = 20
NMAX = 100

for Year in Years:

  os.system('mkdir -p '+Year+'/ForSNU')
  os.system('mkdir -p '+Year+'/CommonSampleInfo')

  listname = 'list_'+massPoints+'.txt'
  listname = open(listname).readlines()

  counter = 0
  for mass in listname:

    counter += 1

    words = mass.strip('\n').split('_')
    mWR = words[0].replace('WR','')
    mN  = words[1].replace('N','')

    str_mass = 'WR%sN%s'%(mWR,mN)

    batchname = '--batchname Skim_'+Year

    #### Skim submission command

    outDest = '/gv0/DATA/SKFlat/Run2Legacy_v4/'+Year+'/PrivateMC/WR_MG_LO_FullSim_Official/WR'+mWR+'_N'+mN+'/'

    if MakeRunScript:

      cmd = 'SKFlat.py -a SkimTree_WROfficial -i TEST_MWR'+massPoints+' -n '+str(NJobPerMass)+' -y '+Year+' --nmax '+str(NMAX)+' -o '+outDest+' --userflags '+mWR+','+mN+' --IsWROfficial '+batchname+' --no_monit &'
      print cmd

      if counter%5==0:
        print 'sleep 30'

    SKFlatSampleAlias = 'Official_FullSim_WR'+mWR+'_N'+mN

    #### make CommonSampleInfo command
    out_CommonSampleInfo = open(Year+'/CommonSampleInfo/'+SKFlatSampleAlias+'.txt','w')
    out_CommonSampleInfo.write('# alias PD xsec nmc sumw\n')
    out_CommonSampleInfo.write(SKFlatSampleAlias+'\t'+SKFlatSampleAlias+'\t1.\t1\t1\n')
    out_CommonSampleInfo.close()

    #### make filelist command
    os.system('ls -1 '+outDest+'/*.root &> '+Year+'/ForSNU/'+SKFlatSampleAlias+'.txt')

    if counter==1:
      print 'mv '+Year+'/CommonSampleInfo/* /data6/Users/jskim/SKFlatAnalyzer/data/Run2Legacy_v4/'+Year+'/Sample/CommonSampleInfo/'
      print 'mv '+Year+'/ForSNU/* /data6/Users/jskim/SKFlatAnalyzer/data/Run2Legacy_v4/'+Year+'/Sample/ForSNU/'

    #[jskim@tamsa2 SkimTree_WROfficial]$ mkdir ForSNU
    #[jskim@tamsa2 SkimTree_WROfficial]$ mkdir CommonSampleInfo
