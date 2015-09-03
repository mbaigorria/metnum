import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from   scipy.interpolate import interp1d
from   numpy import matrix
import math
import os
import sys
import settings

PI = math.pi

def plotData(fileName, angulosVariables=False):
    soluciones = []
    radiosPorPlot = []
    angulosPorPlot = []
    maximo = 0
    minimo = -1

    r_i = settings.radioInt
    r_e = settings.radioExt
    fig = plt.figure()

    if fileName == 'mVariable' or fileName == 'mVariableTempVariable':
        cant = settings.cantM
        aumento = settings.aumentoM
        valor = settings.inicioM
    # else:
        # cant = settings.cantN
        # aumento = settings.aumentoN
        # valor = settings.inicioN

    for i in xrange(cant):
        fileNameTemp = 'isotermas/' + fileName + '_' + str(i+1) + 'iso.out'

        if (os.path.isfile(fileNameTemp) and os.access(fileNameTemp, os.R_OK)) == False:
            print "Could not open file! Invalid file name!"
            return

        f = open(fileNameTemp, 'r')

        #Solo si hay mas de una solucion a mostrar
        if cant > 1:
            plt.subplot(211)
		
        cantSol = settings.valorN
        if fileName == 'nVariable':
            cantSol = valor

        angulosPorPlot.append(cantSol)

        if fileName == 'nVariable':
            radiosPorPlot.append(settings.valorN)
        else:
            radiosPorPlot.append(valor)        

        yAxisIso = []

        for z in xrange(cantSol):
            arr = map(float, f.readline().split())
            yAxisIso.append(arr[0])

        soluciones.append(yAxisIso)

        tempMax = max(yAxisIso)
        if maximo < tempMax:
            maximo = tempMax

        if minimo == -1:
            minimo = maximo

        tempMin = min(yAxisIso)
        if minimo > tempMin:
            minimo = tempMin

        xAxisTheta = []

        d0 = 2/float(cantSol)
        
        #Solo muestro las soluciones pares
        if i % 2 == 0:
            xAxisTheta = np.arange(0, 2, d0)
            plt.plot(xAxisTheta, yAxisIso, '-',  linewidth=1)

        valor += aumento

        plt.axis([0, 2, minimo + -10, maximo + 10])
        title = 'Isotherm proximity to the external edge: $r_i$: %d, $r_e$: %d' % (r_i, r_e)
        plt.title(title)
        plt.ylabel('Isotherm position: $x_{iso}$ (less is better)')
        plt.xlabel('Theta (Angle): $[0, 2\pi]$')

        legend = []

#        n = 0
#        while n < len(angulosPorPlot):
#            text = '$\Delta\Theta$: $\\frac{%d}{%d}$$\pi$ - $\Delta$r: %.3f' % (2, angulosPorPlot[n], (r_e-r_i)/float#(radiosPorPlot[n]))

#            legend.append(text)
#            n+=1

#        plt.legend(legend, loc='center left', bbox_to_anchor=(1, 0.5))
	
        # Tweak spacing to prevent clipping of ylabel
        plt.subplots_adjust(left=0.15)
        plt.grid(True)

    if len(soluciones) > 1:
        plt.subplot(212)

        ultima = soluciones[len(soluciones)-1]

        listDifferences = []

        tempIndex = 0
        for lista in soluciones:
            if tempIndex != len(soluciones)-1:
                value = (sum(ultima)/float(len(lista))) - (sum(lista)/float(len(lista)))
                if value < 0:
                    value *=-1
                listDifferences.append(value)

        xAxisAngulos = range(cant)
        xAxisRadios = np.arange(0, max(radiosPorPlot)+1, 0.01)
        interpolFunc = interp1d(xAxisAngulos, listDifferences, bounds_error=False)
        plt.plot(xAxisRadios, interpolFunc(xAxisRadios), 'k',  linewidth=1)

        plt.ylabel('Difference with the theorical isotherm')
        plt.xlabel('#Radios: m+1')

    plt.subplots_adjust(left=0.15)
    plt.grid(True)

    fig.savefig('graficos/' + fileName + '.pdf')

def main(argv):
    plotData('mVariable')
    #plotData('nVariable')
    #plotData('mVariableTempVariable')

if __name__ == "__main__":
    main(sys.argv[1:])
