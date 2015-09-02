import csv
import settings

class Data:
    dim = 0
    time_eg = 0
    time_lu = 0

dataRadios = []
dataDifL = []
dataDifW = []
dataTiempoL = []
dataTiempoW = []

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
    with open('isotermas/l/mVariable_' + str(i+1) + '.iso.out','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        j = 0
        for row in reader:
            if(j != settings.valorN):
                tIsoterma += float(row[0])
                j += 1
        tIsoterma /= settings.valorN
    dataDifL.append(tIsoterma)
    cantRadios += settings.aumentoM

for i in xrange(settings.cantM):
    tIsoterma = 0
    with open('isotermas/w/mVariable_' + str(i+1) + '.iso.out','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        j = 0
        for row in reader:
            if(j != settings.valorN):
                tIsoterma += float(row[0])
                j += 1
        tIsoterma /= settings.valorN
    dataDifW.append(tIsoterma)

dataDifW = [abs(isotermaIdeal - x) for x in dataDifW]
dataDifL = [abs(isotermaIdeal - x) for x in dataDifL]
    
with open('promedios/mVariable_l.csv','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        if(row[0] != 'm'):
            dataTiempoL.append(row[1])
            
with open('promedios/mVariable_w.csv','rb') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
        if(row[0] != 'm'):
            dataTiempoW.append(row[1])
    
with open('tablas/tablaTiempo_w.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    writer.writerow(["m"] + ["Tiempo(W)"] + ["Dif. solucion optima"] + ["Dif/Tiempo"]);
    for i in xrange(len(dataRadios)):
        tiempo =  (float(dataTiempoW[i]) / 1000000.0)
        dif = float(dataDifW[i])
        ratio = dif/tiempo
        writer.writerow([dataRadios[i]] + [round(tiempo,3)] + [round(dif,3)] + [round(ratio,3)])
        
with open('tablas/tablaTiempo_l.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    writer.writerow(["m"] + ["Tiempo(L)"] + ["Dif. solucion optima"] + ["Dif/Tiempo"]);
    for i in xrange(len(dataRadios)):
        tiempo =  float(dataTiempoL[i]) / 1000000.0
        dif = float(dataDifL[i])
        ratio = dif/tiempo
        writer.writerow([dataRadios[i]] + [round(tiempo,3)] + [round(dif,3)] + [round(ratio,3)])
    
with open('tablas/tablaComparativa.csv', 'wb') as csvfile:
    writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    writer.writerow(["m"] + ["Dif. solucion optima(L)"] + ["Dif. solucion optima(W)"] + ["Dif(W)/Dif(L)"]);
    for i in xrange(len(dataRadios)):
        difL = round(dataDifL[i],3)
        difW = round(dataDifW[i],3)
        ratio = difW/difL
        writer.writerow([dataRadios[i]] + [difL] + [difW] + [round(ratio,3)])    

# with open('tablas/ninstVariable.csv', 'wb') as csvfile:
    # writer = csv.writer(csvfile, delimiter=',', quotechar='|', quoting=csv.QUOTE_MINIMAL)
    # writer.writerow(["Ninst"] + ["EG"] + ["LU"] + ["EG/LU"]);
    # for data in dataList:
        # tiempo_eg = data.time_eg / 1000000.0
        # tiempo_lu = data.time_lu / 1000000.0
        # division = data.time_eg / data.time_lu
        # writer.writerow([data.dim] + [round(tiempo_eg,3)] + [round(tiempo_lu,3)] + [round(division,3)])