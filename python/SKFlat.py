import os,sys,time
import argparse
import datetime
from CheckJobStatus import *
from TimeTools import *
import random

parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-a', dest='Analyzer', default="")
parser.add_argument('-i', dest='InputSample', default="")
parser.add_argument('-l', dest='InputSampleList', default="")
parser.add_argument('-p', dest='DataPeriod', default="ALL")
parser.add_argument('-n', dest='NJobs', default=1, type=int)
parser.add_argument('-o', dest='Outputdir', default="")
parser.add_argument('-q', dest='Queue', default="fastq")
parser.add_argument('--no_exec', action='store_true')
parser.add_argument('--userflags', dest='Userflags', default="")
args = parser.parse_args()

## make flags
Userflags = (args.Userflags).split(',')

## TimeStamp

# 1) dir/file name style
JobStartTime = datetime.datetime.now()
timestamp =  JobStartTime.strftime('%Y_%m_%d_%H%M%S')
# 2) log style
JobStartTime = datetime.datetime.now()
string_JobStartTime =  JobStartTime.strftime('%Y-%m-%d %H:%M:%S')
string_ThisTime = ""

## Environment Variables

SKFlat_WD = os.environ['SKFlat_WD']
SKFlatV = os.environ['SKFlatV']
SKFlatAnV = os.environ['SKFlatAnV']
DATA_DIR = os.environ['SAMPLE_DATA_DIR']
SKFlatRunlogDir = os.environ['SKFlatRunlogDir']
SKFlatOutputDir = os.environ['SKFlatOutputDir']
SKFlatSEDir = os.environ['SKFlatSEDir']
SKFlat_LIB_PATH = os.environ['SKFlat_LIB_PATH']
HOSTNAME = os.environ['HOSTNAME']
UID = str(os.getuid())
IsKISTI = ("ui10.sdfarm.kr" in HOSTNAME)

## Make Sample List

InputSample_Data = ["DoubleMuon", "DoubleEG", "SingleMuon", "SingleElectron"]
AvailableDataPeriods = ["B","C","D","E","F"]
InputSamples = []
StringForHash = ""

## When using list
if args.InputSampleList is not "":
  lines = open(args.InputSampleList)
  for line in lines:
    if "#" in line:
      continue
    line = line.strip('\n')
    InputSamples.append(line)
    StringForHash += line
else:
  if args.InputSample in InputSample_Data:
    if args.DataPeriod=="ALL":
      for period in AvailableDataPeriods:
        InputSamples.append(args.InputSample+":"+period)
        StringForHash += line
    elif args.DataPeriod in AvailableDataPeriods:
      InputSamples.append(args.InputSample+":"+args.DataPeriod)
      StringForHash += args.InputSample+":"+args.DataPeriod
  else:
    InputSamples.append(args.InputSample)
    StringForHash += args.InputSample
FileRangesForEachSample = []

## Get Random Number for webdir

random.seed(StringForHash)
RandomNumber = random.random()
str_RandomNumber = str(RandomNumber).replace('0.','')
webdirname = timestamp+"_"+str_RandomNumber
webdirpathbase = SKFlatRunlogDir+'/www/SKFlatAnalyzerJobLogs/'+webdirname

## Loop over samples
SampleFinishedForEachSample = []
PostJobFinishedForEachSample = []
BaseDirForEachSample = []
for InputSample in InputSamples:

  NJobs = args.NJobs

  SampleFinishedForEachSample.append(False)
  PostJobFinishedForEachSample.append(False)

  ## Global Varialbes

  IsDATA = False
  DataPeriod = ""
  if ":" in InputSample:
    IsDATA = True
    tmp = InputSample
    InputSample = tmp.split(":")[0]
    DataPeriod = tmp.split(":")[1]

  ## Prepare output

  base_rundir = SKFlatRunlogDir+'/'+args.Analyzer+'__'+timestamp+'__'+InputSample
  if IsDATA:
    base_rundir = base_rundir+'__period'+DataPeriod+'/'

  os.system('mkdir -p '+base_rundir)
  os.system('mkdir -p '+base_rundir+'/output/')

  ## Copy shared library file

  os.system('mkdir -p '+base_rundir+'/lib/')
  os.system('cp '+SKFlat_LIB_PATH+'/* '+base_rundir+'/lib')
  if IsKISTI:
    os.chdir(SKFlat_WD)
    os.system('tar -czf data.tar.gz data')
    os.system('mv data.tar.gz '+base_rundir)
    cwd = os.getcwd()
    os.chdir(base_rundir)
    os.system('tar -czf lib.tar.gz lib/*')
    os.chdir(cwd)

  ## Create webdir

  this_webdir = webdirpathbase+'/'+base_rundir.replace(SKFlatRunlogDir,'')
  os.system('mkdir -p '+this_webdir)
  #os.system('scp -r '+this_webdir+' jskim@147.47.242.71:/var/www/html/SKFlatAnalyzerJobLogs/')
  #os.system('ssh -Y jskim@147.47.242.71 chmod -R 777 /var/www/html/SKFlatAnalyzerJobLogs/'+base_rundir.replace(SKFlatRunlogDir,''))

  ## Get Sample Path

  lines_files = []
  if IsKISTI:
    tmpfilepath = DATA_DIR+'/Sample/ForKISTI/'+InputSample+'.txt'
    if IsDATA:
      tmpfilepath = DATA_DIR+'/Sample/ForKISTI/'+InputSample+'_'+DataPeriod+'.txt'
    lines_files = open(tmpfilepath).readlines()
    os.system('cp '+tmpfilepath+' '+base_rundir+'/input_filelist.txt')

  else:
    lines_SamplePath = open(DATA_DIR+"/Sample/SamplePath.txt").readlines()
    Samplepath_Section="Alias"
    SampleBaseDirs = []
    NtupleFilePath = ""
    for line in lines_SamplePath:

      if len(line.split())==0:
        continue

      words = line.split()

      if "Section:" in line:
        Samplepath_Section = words[1]
        continue

      if Samplepath_Section=="Alias":
        SampleBaseDirs.append( (words[0], words[1]) )


      if IsDATA and Samplepath_Section=="DATA":
        if words[0]==InputSample:
          NtupleFilePath = words[1]
          break

      if not IsDATA and Samplepath_Section=="MC":
        if words[0]==InputSample:
          NtupleFilePath = words[1]
          break
        
    for BaseDir in SampleBaseDirs:
      alias = BaseDir[0]
      path = BaseDir[1]
      if alias in NtupleFilePath:
        if IsKISTI:
          NtupleFilePath = NtupleFilePath.replace(alias,path)
          NtupleFilePath = NtupleFilePath.replace('<SKFlatV>','SKFlat_'+SKFlatV)
        else:
          NtupleFilePath = NtupleFilePath.replace(alias,path+'/'+SKFlatV+'/')
    if IsDATA:
      NtupleFilePath = NtupleFilePath+'period'+DataPeriod+'/'

    os.system('ls -1 '+NtupleFilePath+'/*.root > '+base_rundir+'/input_filelist.txt')
    lines_files = open(base_rundir+'/input_filelist.txt').readlines()

  NTotalFiles = len(lines_files)

  if NJobs>NTotalFiles:
    NJobs = NTotalFiles

  SubmitOutput = open(base_rundir+'/SubmitOutput.log','w')

  SubmitOutput.write("<SKFlat> NTotalFiles = "+str(NTotalFiles)+'\n')
  SubmitOutput.write("<SKFlat> NJobs = "+str(NJobs)+'\n')
  nfilepjob = int(NTotalFiles/NJobs)
  SubmitOutput.write("<SKFlat> --> # of files per job = "+str(nfilepjob)+'\n')
  nfilepjob_remainder = NTotalFiles-(NJobs)*(nfilepjob)
  if nfilepjob_remainder>=(NJobs):
    SubmitOutput.write('nfilepjob_remainder = '+str(nfilepjob_remainder)+'\n')
    SubmitOutput.write('while, NJobs = '+str(NJobs)+'\n')
    SubmitOutput.write('--> exit'+'\n')
    sys.exit()

  FileRanges = []
  temp_end_largerjob = 0
  nfile_checksum = 0
  ## First nfilepjob_remainder jobs will have (nfilepjob+1) files per job
  for it_job in range(0,nfilepjob_remainder):
    FileRanges.append(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
    temp_end_largerjob = (it_job+1)*(nfilepjob+1)
    nfile_checksum += len(range(it_job*(nfilepjob+1),(it_job+1)*(nfilepjob+1)))
  ## Remaining NJobs-nfilepjob_remainder jobs will have (nfilepjob) files per job
  for it_job in range(0,NJobs-nfilepjob_remainder):
    FileRanges.append(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
    nfile_checksum += len(range(temp_end_largerjob+(it_job*nfilepjob),temp_end_largerjob+((it_job+1)*nfilepjob) ))
  SubmitOutput.write('nfile_checksum = '+str(nfile_checksum)+'\n')
  SubmitOutput.write('NTotalFiles = '+str(NTotalFiles)+'\n')
  FileRangesForEachSample.append(FileRanges)

  ## Write run script

  if IsKISTI:

    commandsfilename = args.Analyzer+'_'+InputSample
    if IsDATA:
      commandsfilename += '_'+DataPeriod
    run_commands = open(base_rundir+'/'+commandsfilename+'.sh','w')
    print>>run_commands,'''#!/bin/bash
SECTION=`printf %03d $1`
WORKDIR=`pwd`
echo "#### Extracting libraries ####"
tar -zxvf lib.tar.gz
echo "#### Extracting run files ####"
tar -zxvf runFile.tar.gz
echo "#### Extracting data files ####"
tar -zxvf data.tar.gz
echo "#### cmsenv ####"
export CMS_PATH=/cvmfs/cms.cern.ch
source $CMS_PATH/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc630
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
eval `scramv1 runtime -sh`
cd -
echo "#### setup root ####"
source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

NoAuthError=999
Trial=0

while [ "$NoAuthError" -ne 0 ]; do

  if [ "$Trial" -gt 9999 ]; then
    break
  fi

  echo "#### running ####"
  echo "root -l -b -q run_${SECTION}.C"
  root -l -b -q run_${SECTION}.C 2> err.log
  NoAuthError=`grep "Error in <TNetXNGFile::Open>" err.log -R | wc -l`

  if [ "$NoAuthError" -ne 0 ]; then
    echo "NoAuthError="$NoAuthError
    echo "AUTH error occured.. running again in 30 seconds.."
    Trial=$((Trial+=1))
    sleep 30
  fi

done

cat err.log >&2
'''
    run_commands.close()

    submit_command = open(base_rundir+'/submit.jds','w')
    print>>submit_command,'''executable = {3}.sh
universe   = vanilla
arguments  = $(Process)
requirements = OpSysMajorVer == 6
log = condor.log
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
output = job_$(Process).log
error = job_$(Process).err
transfer_input_files = {0}, {1}, {4}
use_x509userproxy = true
transfer_output_remaps = "hists.root = output/hists_$(Process).root"
queue {2}
'''.format(base_rundir+'/runFile.tar.gz', base_rundir+'/lib.tar.gz',str(NJobs), commandsfilename, base_rundir+'/data.tar.gz')
    submit_command.close()

  CheckTotalNFile=0
  for it_job in range(0,len(FileRanges)):

    #print "["+str(it_job)+"th]",
    #print FileRanges[it_job],
    #print " --> "+str(len(FileRanges[it_job]))

    CheckTotalNFile = CheckTotalNFile+len(FileRanges[it_job])

    thisjob_dir = base_rundir+'/job_'+str(it_job)+'/'

    runfunctionname = "run"
    libdir = (base_rundir+'/lib').replace('///','/').replace('//','/')
    runCfileFullPath = ""
    if IsKISTI:
      libdir = './lib'
      runfunctionname = "run_"+str(it_job).zfill(3)
      runCfileFullPath = base_rundir+'/run_'+str(it_job).zfill(3)+'.C'
    else:
      os.system('mkdir -p '+thisjob_dir)
      runCfileFullPath = thisjob_dir+'run.C'

    out = open(runCfileFullPath, 'w')
    print>>out,'''R__LOAD_LIBRARY({1}/{0}_C.so)

void {2}(){{

  {0} m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");
'''.format(args.Analyzer, libdir, runfunctionname)

    if IsDATA:
      out.write('  m.IsDATA = true;\n')
      out.write('  m.DataStream = "'+InputSample+'";\n')
    else:
      out.write('  m.MCSample = "'+InputSample+'";\n');
      out.write('  m.IsDATA = false;\n')

    if len(Userflags)>0:
      out.write('  m.Userflags = {\n')
      for flag in Userflags:
        out.write('    "'+flag+'",\n')
      out.write('  };\n')

    if IsKISTI:
      out.write('  m.SetOutfilePath("hists.root");\n')
    else:
      out.write('  m.SetOutfilePath("'+thisjob_dir+'/hists.root");\n')

    for it_file in FileRanges[it_job]:
      thisfilename = lines_files[it_file].strip('\n')
      out.write('  m.AddFile("'+thisfilename+'");\n')

    print>>out,'''  m.Init();

  m.Loop();

  m.WriteHist();

}'''
    out.close()

    if not IsKISTI:
      run_commands = open(thisjob_dir+'commands.sh','w')
      print>>run_commands,'''echo "[SKFlat.py] cmsset_default.sh"
source /cvmfs/cms.cern.ch/cmsset_default.sh
echo "[SKFlat.py] Going to $CMSSW_BASE/src"
cd /data7/Users/jskim/CMSSW_9_4_4/src/
export SCRAM_ARCH=slc6_amd64_gcc630
echo "[SKFlat.py] Trying cmsenv"
cmsenv
cd {0}
echo "[SKFlat.py] Okay, let's run the analysis"
root -l -b -q run.C
echo "[SKFlat.py] JOB FINISHED!!"
'''.format(thisjob_dir)
      run_commands.close()

      jobname = 'job_'+str(it_job)+'_'+args.Analyzer
      cmd = 'qsub -V -q '+args.Queue+' -N '+jobname+' -wd '+thisjob_dir+' commands.sh '

      if not args.no_exec:
        cwd = os.getcwd()
        os.chdir(thisjob_dir)
        os.system(cmd+' > submitlog.log')
        os.chdir(cwd)
      sublog = open(thisjob_dir+'/submitlog.log','a')
      sublog.write('\nSubmission command was : '+cmd+'\n')
      sublog.close()

  if IsKISTI:

    cwd = os.getcwd()
    os.chdir(base_rundir)
    os.system('tar -czf runFile.tar.gz run_*.C')
    if not args.no_exec:
      os.system('condor_submit submit.jds')
    os.chdir(cwd)

  else:

    ## Write Kill Command

    KillCommand = open(base_rundir+'/Script_JobKill.sh','w')
    for it_job in range(0,len(FileRanges)):
      thisjob_dir = base_rundir+'/job_'+str(it_job)+'/'
      jobid = GetJobID(thisjob_dir, args.Analyzer, it_job)
      KillCommand.write('qdel '+jobid+' ## job_'+str(it_job)+' ##\n')
    KillCommand.close()

##########################
## Submittion all done. ##
## Now monitor job      ##
##########################

## Loop over samples again

AllSampleFinished = False
GotError = False
ErrorLog = ""

try:
  while not AllSampleFinished:

    if GotError:
      break

    AllSampleFinished = True

    for it_sample in range(0,len(InputSamples)):

      InputSample = InputSamples[it_sample]
      SampleFinished = SampleFinishedForEachSample[it_sample]
      PostJobFinished = PostJobFinishedForEachSample[it_sample]

      if PostJobFinished:
        continue
      else:
        AllSampleFinished = False

      ## Global Varialbes

      IsDATA = False
      DataPeriod = ""
      if ":" in InputSample:
        IsDATA = True
        tmp = InputSample
        InputSample = tmp.split(":")[0]
        DataPeriod = tmp.split(":")[1]

      ## Prepare output

      base_rundir = SKFlatRunlogDir+'/'+args.Analyzer+'__'+timestamp+'__'+InputSample
      if IsDATA:
        base_rundir = base_rundir+'__period'+DataPeriod+'/'
      this_webdir = webdirpathbase+'/'+base_rundir.replace(SKFlatRunlogDir,'')

      if not SampleFinished:

        ## This sample was not finished in the previous monitoring
        ## Monitor again this time

        ThisSampleFinished = True

        ## Write Job status until it's done

        statuslog = open(base_rundir+'/JobStatus.log','w')
        statuslog.write('Job submitted at '+string_JobStartTime+'\n')
        statuslog.write('JobNumber\t| Status\n')

        ToStatuslog = []
        n_eventran = 0
        finished = []
        EventDone = 0
        EventTotal = 0

        TotalEventRunTime = 0
        MaxTimeLeft = 0
        MaxEventRunTime = 0

        FileRanges = FileRangesForEachSample[it_sample]

        for it_job in range(0,len(FileRanges)):

          thisjob_dir = base_rundir+'/'
          if IsKISTI:
            thisjob_dir = base_rundir

          this_status = ""
          this_status = CheckJobStatus(thisjob_dir, args.Analyzer, it_job, IsKISTI)

          if "ERROR" in this_status:
            GotError = True
            statuslog.write("#### ERROR OCCURED ####\n")
            statuslog.write(this_status+'\n')
            ErrorLog = this_status
            break

          if "FINISHED" not in this_status:
            ThisSampleFinished = False

          outlog = ""
          if "FINISHED" in this_status:
            finished.append("Finished")

            EventInfo = this_status.split()[1].split(':')

            this_EventDone = int(EventInfo[2])
            this_EventTotal = int(EventInfo[2])

            EventDone += this_EventDone
            EventTotal += this_EventTotal

            line_EventRunTime = this_status.split()[2]+' '+this_status.split()[3]
            this_jobstarttime = GetDatetimeFromMyFormat(line_EventRunTime)
            this_diff = datetime.datetime.now()-this_jobstarttime
            this_EventRunTime = 86400*this_diff.days+this_diff.seconds

            this_TimePerEvent = float(this_EventRunTime)/float(this_EventDone)
            this_TimeLeft = (this_EventTotal-this_EventDone)*this_TimePerEvent

            TotalEventRunTime += this_EventRunTime
            MaxTimeLeft = max(MaxTimeLeft,this_TimeLeft)
            MaxEventRunTime = max(MaxEventRunTime,this_EventRunTime)

          elif "RUNNING" in this_status:
            outlog = str(it_job)+'\t| '+this_status.split()[1]+' %'

            if len(this_status.split())<3 :
              SubmitOutput.write('len(this_status.split())<3;; Priting this_status.split()\n')
              SubmitOutput.write(this_status.split()+'\n')

            EventInfo = this_status.split()[2].split(':')

            this_EventDone = int(EventInfo[1])
            this_EventTotal = int(EventInfo[2])

            EventDone += this_EventDone
            EventTotal += this_EventTotal

            line_EventRunTime = this_status.split()[3]+' '+this_status.split()[4]
            this_jobstarttime = GetDatetimeFromMyFormat(line_EventRunTime)
            this_diff = datetime.datetime.now()-this_jobstarttime
            this_EventRunTime = 86400*this_diff.days+this_diff.seconds

            if this_EventDone==0:
              this_EventDone = 1

            this_TimePerEvent = float(this_EventRunTime)/float(this_EventDone)
            this_TimeLeft = (this_EventTotal-this_EventDone)*this_TimePerEvent

            TotalEventRunTime += this_EventRunTime
            MaxTimeLeft = max(MaxTimeLeft,this_TimeLeft)
            MaxEventRunTime = max(MaxEventRunTime,this_EventRunTime)

            round_this_TimeLeft = round(this_TimeLeft,1)
            round_this_EventRunTime = round(this_EventRunTime,1)

            outlog += ' ('+str(round_this_EventRunTime)+' s ran, and '+str(round_this_TimeLeft)+' s left)'
            ToStatuslog.append(outlog)
            n_eventran += 1

          else:
            outlog = str(it_job)+'\t| '+this_status
            ToStatuslog.append(outlog)

          ##---- END it_job loop

        if GotError:
          ## When error occured, change both Finished/PostJob Flag to True
          SampleFinishedForEachSample[it_sample] = True
          PostJobFinishedForEachSample[it_sample] = True
          break

        for l in ToStatuslog:
          statuslog.write(l+'\n')
        statuslog.write('\n==============================================================\n')
        statuslog.write('HOSTNAME = '+HOSTNAME+'\n')
        statuslog.write('queue = '+args.Queue+'\n')
        statuslog.write(str(len(FileRanges))+' jobs submitted\n')
        statuslog.write(str(n_eventran)+' jobs are running\n')
        statuslog.write(str(len(finished))+' jobs are finished\n')

        ThisTime = datetime.datetime.now()
        string_ThisTime =  ThisTime.strftime('%Y-%m-%d %H:%M:%S')

        statuslog.write('EventDone = '+str(EventDone)+'\n')
        statuslog.write('EventTotal = '+str(EventTotal)+'\n')
        statuslog.write('EventLeft = '+str(EventTotal-EventDone)+'\n')
        statuslog.write('TotalEventRunTime = '+str(TotalEventRunTime)+'\n')
        statuslog.write('MaxTimeLeft = '+str(MaxTimeLeft)+'\n')
        statuslog.write('MaxEventRunTime = '+str(MaxEventRunTime)+'\n')

        t_per_event = 1
        if EventDone is not 0:
          t_per_event = float(TotalEventRunTime)/float(EventDone)
        statuslog.write('t_per_event = '+str(t_per_event)+'\n')

        EstTime = ThisTime+datetime.timedelta(0, MaxTimeLeft)

        statuslog.write('Estimated Finishing Time : '+EstTime.strftime('%Y-%m-%d %H:%M:%S')+'\n')
        statuslog.write('Last checked at '+string_ThisTime+'\n')
        statuslog.close()

        ## copy statuslog to webdir
        os.system('cp '+base_rundir+'/JobStatus.log '+this_webdir)
        #os.system('scp -r '+webdir+' jskim@147.47.242.71:/var/www/html/SKFlatAnalyzerJobLogs/')
        #os.system('ssh -Y jskim@147.47.242.71 chmod -R 777 /var/www/html/SKFlatAnalyzerJobLogs/'+base_rundir.replace(SKFlatRunlogDir,''))

        ## This time, it is found to be finished
        ## Change the flag
        if ThisSampleFinished:
          SampleFinishedForEachSample[it_sample] = True
        ##---- END if finished

      else:

        ## Job was finished in the previous monitoring
        ## Check if PostJob is also finished

        if not PostJobFinished:

          ## PostJob was not done in the previous monitoring
          ## Copy output, and change the PostJob flag

          outputname = args.Analyzer+'_'+InputSample
          if IsDATA:
            outputname += '_'+DataPeriod

          if not GotError:
            cwd = os.getcwd()
            os.chdir(base_rundir)

            if IsKISTI:
              os.system('hadd -f '+outputname+'.root output/*.root >> JobStatus.log')
              os.system('rm output/*.root')
            else:
              os.system('hadd -f '+outputname+'.root job_*/*.root >> JobStatus.log')
              #os.system('rm job_*/*.root')

            ## Final Outputpath

            FinalOutputPath = args.Outputdir
            if args.Outputdir=="":
              FinalOutputPath = SKFlatOutputDir+'/'+args.Analyzer+'/'
              if IsDATA:
                FinalOutputPath += '/DATA/'

            os.system('mkdir -p '+FinalOutputPath)
            os.system('cp '+outputname+'.root '+FinalOutputPath)
            os.chdir(cwd)

          PostJobFinishedForEachSample[it_sample] = True

    os.system('scp -r '+webdirpathbase+'/* jskim@147.47.242.71:/var/www/html/SKFlatAnalyzerJobLogs/')
    os.system('ssh -Y jskim@147.47.242.71 chmod -R 777 /var/www/html/SKFlatAnalyzerJobLogs/'+args.Analyzer+"*")

    time.sleep(20)

except KeyboardInterrupt:
  print('interrupted!')

## Send Email now

from SendEmail import SendEmail
JobFinishEmail = '''#### Job Info ####
Analyzer = {0}
InputSample = {1}
OutputDir = {2}
'''.format(args.Analyzer,InputSamples,args.Outputdir)
JobFinishEmail += '''##################
Job started at {0}
Job finished at {1}
'''.format(string_JobStartTime,string_ThisTime)

EmailTitle = 'Job Summary'
if GotError:
  JobFinishEmail = "#### ERROR OCCURED ####\n"+JobFinishEmail
  JobFinishEmail = ErrorLog+"\n------------------------------------------------\n"+JobFinishEmail
  EmailTitle = '[ERROR] Job Summary'

SendEmail('jskim','jae.sung.kim@cern.ch',EmailTitle,JobFinishEmail)




