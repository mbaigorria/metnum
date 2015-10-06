import csv
import settings
import matplotlib.pyplot as plt
import math

#plt.xkcd()

dataDim = []
dataTime = []
with open('promedios/complejidad.csv','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        if(row[0] != 'n'):
            dataDim.append(int(row[0]))
            dataTime.append(float(row[1]) / 1000000.0)

fig = plt.figure()
sub = fig.add_subplot(1,1,1)
sub.scatter(dataDim, dataTime, color='blue', edgecolor='black')
#sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
#plt.legend(loc='upper right', scatterpoints=1)
plt.axis([0.0, 2400.0, 0.0, 5.0])
plt.xlabel('Dimension')
plt.ylabel('T(s)')
plt.title('Tiempo de ejecucion segun dimension')
#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
fig.savefig('graficos/complejidad.pdf')
plt.close(fig)