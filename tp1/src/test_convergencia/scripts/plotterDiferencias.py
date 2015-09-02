import csv
import settings
import matplotlib.pyplot as plt
import math

#plt.xkcd()

dataRadios = []
dataIsotermas = [] # Diferencia punto a punto con la ideal, sumar todo y dividir por cantidad
isotermaIdeal = 0

with open('isotermas/' + settings.criterio + '/isotermaIdeal_1.iso.out','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    i = 0
    for row in reader:
        if(i != settings.nIdeal):
            isotermaIdeal += float(row[0])
            i += 1
    isotermaIdeal /= settings.nIdeal

cantRadios = settings.inicioM
for i in xrange(settings.cantM):
    dataRadios.append(cantRadios)
    tIsoterma = 0
    with open('isotermas/' + settings.criterio + '/mVariable_' + str(i+1) + '.iso.out','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        j = 0
        for row in reader:
            if(j != settings.valorN):
                tIsoterma += float(row[0])
                j += 1
        tIsoterma /= settings.valorN
    dataIsotermas.append(tIsoterma)
    cantRadios += settings.aumentoM

radiosPos = []
radiosNeg = []
valoresPos = []
valoresNeg = []

for i in xrange(len(dataRadios)):
    dif = isotermaIdeal-dataIsotermas[i]
    if (dif < 0) :
        radiosNeg.append(dataRadios[i])
        valoresNeg.append(abs(dif))
    else:
        radiosPos.append(dataRadios[i])
        valoresPos.append(dif)
        
fig = plt.figure()
fig.add_subplot(111)
plt.axis([0.0, 425.0, 0.0, 12.0])
plt.xlabel('Radios')
plt.ylabel('Diferencia entre isoterma ideal y la obtenida(' + settings.criterio + ')')
plt.scatter(radiosPos, valoresPos, color='green', edgecolor='black', label='Diferencia')
plt.scatter(radiosNeg, valoresNeg, color='red', edgecolor='black', label='Modulo de la diferencia')
plt.legend(loc='upper right', scatterpoints=1)
fig.savefig('graficos/mVariable_' + settings.criterio + '.pdf')
plt.close(fig)

dataAngulos = []
dataIsotermas = []
cantAngulos = settings.inicioN
for i in xrange(settings.cantN):
    dataAngulos.append(cantRadios)
    tIsoterma = 0
    with open('isotermas/' + settings.criterio + '/nVariable_' + str(i+1) + '.iso.out','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        j = 0
        for row in reader:
            if(j != settings.valorN):
                tIsoterma += float(row[0])
                j += 1
        tIsoterma /= cantAngulos
    dataIsotermas.append(tIsoterma)
    cantRadios += settings.aumentoM

angulosPos = []
angulosNeg = []
valoresPos = []
valoresNeg = []

for i in xrange(len(dataAngulos)):
    dif = isotermaIdeal-dataIsotermas[i]
    if (dif < 0) :
        angulosNeg.append(dataAngulos[i])
        valoresNeg.append(abs(dif))
    else:
        angulosPos.append(dataAngulos[i])
        valoresPos.append(dif)

print angulosPos
print valoresPos        

fig = plt.figure()
fig.add_subplot(111)
plt.axis([400.0, 460.0, 0.0, 48.0])
plt.xlabel('Angulos')
plt.ylabel('Diferencia entre isoterma ideal y la obtenida(' + settings.criterio + ')')
plt.scatter(angulosPos, valoresPos, color='green', edgecolor='black', label='Diferencia')
plt.scatter(angulosNeg, valoresNeg, color='red', edgecolor='black', label='Modulo de la diferencia')
# plt.legend(loc='upper right', scatterpoints=1)
fig.savefig('graficos/nVariable_' + settings.criterio + '.pdf')
plt.close(fig)