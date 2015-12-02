import csv
import settings
import matplotlib.pyplot as plt
import math

#plt.xkcd()

instanciasSNAP = ['BerkStan', 'NotreDame', 'Stanford']

for instanciaSNAP in instanciasSNAP:
    for i in range(20,25):
        dataDim = []
        dataTime = []
        with open('norma/web-' + str(instanciaSNAP) + '_' + str(i) + '.csv','rb') as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            j = 1
            for row in reader:
                if(row[0] != 'n'):
                    dataDim.append(j)
                    dataTime.append(float(row[0]))
                    j += 1

        fig = plt.figure()
        sub = fig.add_subplot(1,1,1)
        sub.scatter(dataDim, dataTime, color='blue', edgecolor='black')
        #sub.scatter(dataDim, dataTimeLU, color='red', edgecolor='black', label='LU')
        #plt.legend(loc='upper right', scatterpoints=1)
        plt.axis([0.0, j + 1.0, -0.1, 1.2])
        plt.xlabel('Iteraciones')
        plt.ylabel('Delta')
        plt.title('Convergencia de la norma')
        #plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
        fig.savefig('graficos/normas/web-' + str(instanciaSNAP) + '_' + str(i) + '.pdf')
        plt.close(fig)