all_masses = open('masses.txt').readlines()

masses_500_to_3500 = []
masses_3500_to_5000 = []
masses_5000_to_6000 = []

for mass in all_masses:
  if "#" in mass:
    continue

  ## mass = WRtoNLtoLLJJ_WR1200_N100
  words = mass.split('_')
  mWR = int(words[1].replace('WR',''))
  mN  = int(words[2].replace('N',''))

  mass = mass.replace('WRtoNLtoLLJJ_','').strip('\n')

  if (500 <= mWR) and (mWR <=3500):
    masses_500_to_3500.append(mass)
  elif (mWR<=5000):
    masses_3500_to_5000.append(mass)
  else:
    masses_5000_to_6000.append(mass)

out = open('list_500_to_3500.txt','w')
for m in masses_500_to_3500:
  out.write(m+'\n')
out.close()

out = open('list_3500_to_5000.txt','w')
for m in masses_3500_to_5000:
  out.write(m+'\n')
out.close()

out = open('list_5000_to_6000.txt','w')
for m in masses_5000_to_6000:
  out.write(m+'\n')
out.close()

#print masses_500_to_3500
#print masses_3500_to_5000
#print masses_5000_to_6000

#### Debug
#masses = [
#"WR5000_N100",
#"WR5000_N3000",
#]
####
