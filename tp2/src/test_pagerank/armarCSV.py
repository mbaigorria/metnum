import csv
import settings
import matplotlib.pyplot as plt

dataN = []
dataRank = []
extras = settings.extras

for i in xrange(settings.extras):
    with open('manipulacion/out/manip_' + str(i+1) +'.out', 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=",")
        row = next(reader)
        dataN.append(extras)
        extras += 1
        dataRank.append(row[0])

fig = plt.figure()
sub = fig.add_subplot(1,1,1)
sub.scatter(dataN, dataRank, color='blue', edgecolor='black')
#sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
#plt.legend(loc='upper right', scatterpoints=1)
plt.axis([75.0, 165.0, 0.0, 0.2])
plt.xlabel('Paginas extras')
plt.ylabel('Rank')
plt.title('Manipulacion del ranking')
#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
fig.savefig('graficos/manipulacion.pdf')
plt.close(fig)
        
dataC = []
dataRankC = []
valorC = 0
aumentoC = 1.0/settings.muestrasC

for i in xrange(settings.muestrasC):
    with open('manipulacion/out/manipC_' + str(i+1) +'.out', 'rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=",")
        row = next(reader)
        dataC.append(valorC)
        valorC += aumentoC
        dataRankC.append(row[0])

fig = plt.figure()
sub = fig.add_subplot(1,1,1)
sub.scatter(dataC, dataRankC, color='blue', edgecolor='black')
#sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
#plt.legend(loc='upper right', scatterpoints=1)
plt.axis([0.0, 1.0, 0.0, 0.25])
plt.xlabel('Valor de C')
plt.ylabel('Rank')
plt.title('Manipulacion del ranking para diferentes valores de C')
#plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
fig.savefig('graficos/manipulacionC.pdf')
plt.close(fig)