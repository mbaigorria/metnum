import csv
import settings
import matplotlib.pyplot as plt
import math

#plt.xkcd()

for variante in settings.variantes:
    dataDim = []
    dataTime = []
    dataTimeDiv = []
    with open('promedios/dimVariable_' + variante + '.csv','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            if(row[0] != 'Dim'):
                lastline = row
                dataDim.append(int(row[0]))
                dataTime.append(float(row[1]))
                dataTimeDiv.append(float(row[1]) / (float(row[0]))**3)
                
    fig = plt.figure(figsize=(5,5))
    sub = fig.add_subplot(1,1,1)
    sub.scatter(dataDim, dataTime)
    plt.axis([0.0, float(lastline[0]) + 100.0, 0.0, float(lastline[1])])
    plt.xlabel('Dimension')
    plt.ylabel('T(us)')
    plt.title('Tiempo de ejecucion segun dimension (' + variante + ')')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    fig.savefig('graficos/dimVariable_' + variante + '.pdf')
    plt.close(fig)

    fig = plt.figure(figsize=(5,5))
    sub = fig.add_subplot(1,1,1)
    sub.scatter(dataDim, dataTimeDiv)
    plt.axis([0.0, float(lastline[0]) + 100.0, 0.0, 0.1]) # Ajustar el eje Y a mano
    plt.xlabel('Dimension')
    plt.ylabel('T(us)/ dim3')
    plt.title('Tiempo de ejecucion segun dimension (' + variante + ')')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    fig.savefig('graficos/dimVariableDiv_' + variante + '.pdf')
    plt.close(fig)
    
    dataNinst = []
    dataTime = []
    with open('promedios/ninstVariable_' + variante + '.csv','rb') as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            if(row[0] != 'Ninst'):
                lastline = row
                dataNinst.append(int(row[0]))
                dataTime.append(float(row[1]))
                
    fig = plt.figure(figsize=(5,5))
    sub = fig.add_subplot(1,1,1)
    sub.scatter(dataNinst, dataTime)
    plt.axis([0.0, float(lastline[0]), 0.0, float(lastline[1])])
    plt.xlabel('Nro. de instacias')
    plt.ylabel('T(us)')
    plt.title('Tiempo de ejecucion segun Nro. de instacias (' + variante + ')')
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    fig.savefig('graficos/instVariable_' + variante + '.pdf')
    plt.close(fig)