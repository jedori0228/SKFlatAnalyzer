import os

os.system('ls -1 CommonSampleInfo/*.txt &> temp.txt');
lines = open('temp.txt').readlines()
os.system('rm temp.txt')

out_MC = open('SampleSummary_MC.txt','w')
out_Sig_HNWR = open('SampleSummary_Signal_HNWR.txt','w')
out_Sig_HNWR_FullSim = open('SampleSummary_Signal_HNWR_FullSim.txt','w')
out_Sig_HNWR_Official = open('SampleSummary_Signal_HNWR_Official.txt','w')

FirstLine = '# alias PD xsec nmc sumw\n'
out_MC.write(FirstLine)
out_Sig_HNWR.write(FirstLine)
out_Sig_HNWR_FullSim.write(FirstLine)
out_Sig_HNWR_Official.write(FirstLine)

for line in lines:

  line = line.strip('\n')
  alias = line.replace('CommonSampleInfo/','').replace('.txt','')

  infoline = open(line).readlines()[1]

  if alias.startswith("WRtoNLtoLLJJ_"):
    out_Sig_HNWR.write(infoline)
  elif alias.startswith("FullSim_") and "WRtoNLtoLLJJ" in alias:
    out_Sig_HNWR_FullSim.write(infoline)
  elif "Official" in alias and "WR" in alias:
    out_Sig_HNWR_Official.write(infoline)
  else:
    out_MC.write(infoline)

out_MC.close()
out_Sig_HNWR.close()
out_Sig_HNWR_FullSim.close()
out_Sig_HNWR_Official.close()
