import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from   scipy.interpolate import interp1d
from   numpy import matrix
import math
import os
import sys

PI = math.pi

def plotData(fileName, angulosVariables=False):
        soluciones = []
        maximo = 0
        minimo = -1

        if (os.path.isfile(fileName) and os.access(fileName, os.R_OK)) == False:
            print "Could not open file! Invalid file name!"
            return

        f = open(fileName, 'r')

        ninst, r_i, r_e = map(int, f.readline().split())

        radiosPorPlot = []
        angulosPorPlot = []

        z = 0

        #Plot con las diferencias entre la posicion de la isoterma y el radio externo
        #plt.figure(1)
        #PRE: ninst > 1 <==> Las instancias tienen igual cantidad de soluciones.
        if ninst > 1 and angulosVariables == False:
            plt.subplot(211)

        while z < ninst:

            radios, angulos = map(int, f.readline().split())

            radiosPorPlot.append(radios)
            angulosPorPlot.append(angulos)

            i = 0

            yAxisIso = []

            while i < angulos:
                arr = map(float, f.readline().split())
                if i < angulos:
                    yAxisIso.append(arr[0])

                i+=1


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

            n = float(angulos)
            d0 = 2/n

            #Calcular a mano del dominio
            #j = 0
            #count = 0.0
            #while j < angulos:
            #xAxisTheta.append(round(count,2))
            #count+=d0
            #j+=1

            #Calculo automatico del dominio
            if z % 2 == 0:
                xAxisTheta = np.arange(0, 2, d0)
                plt.plot(xAxisTheta, yAxisIso, '-',  linewidth=1)

            #Es igual a unir cada punto con una linea
            #interpFunc = interp1d(xAxisTheta, yAxisIso, bounds_error=False)
            #xnew = np.arange(0, 2, d0) #Queda igual que la funcion original dado que el dominio es el mismo.
            #xnew = np.arange(0, 2, .001)
            #plt.plot(xnew, interpFunc(xnew), '-')

            z+=1

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

        if len(soluciones) > 1 and angulosVariables == False:
	    #PRE: usar soluciones > 1 <==> ninst > 1 : usar ninst > 1 solo con soluciones que tengan la misma cantidad de puntos.
            plt.subplot(212)

            ultima = soluciones[len(soluciones)-1]

            listDifferences = []

            tempIndex = 0
            for lista in soluciones:
                if tempIndex != len(soluciones)-1:
		    #Suma de las diferencias (arma pares y los resta)
                    #listDifferences.append(sum([x-y if x>y else y-x for x, y in zip(ultima, lista)])/len(lista))

		    value = (sum(ultima)/float(len(lista))) - (sum(lista)/float(len(lista)))
		    if value < 0:
			value *=-1
		    listDifferences.append(value)
	
            xAxisAngulos = np.arange(0, len(angulosPorPlot), 1)
            xAxisRadios = np.arange(0, max(radiosPorPlot)+1, 0.01)
            interpolFunc = interp1d(xAxisAngulos, listDifferences, bounds_error=False)
            plt.plot(xAxisRadios, interpolFunc(xAxisRadios), 'k',  linewidth=1)

            plt.ylabel('Difference with the theorical isotherm')
            plt.xlabel('#Radios: m+1')

        plt.subplots_adjust(left=0.15)
        plt.grid(True)

        plt.show()

def main(argv):
    lista = sys.argv
    if len(sys.argv) == 2:
        fileName = lista[1]
        plotData(fileName)
    elif len(sys.argv) == 3:
        fileName = lista[1]
        angulosVariables = bool(int(lista[2]))
        print angulosVariables
        plotData(lista[1], angulosVariables)
    else:
        print "Invalid format: filename method (0 or 1)"

if __name__ == "__main__":
   main(sys.argv[1:])
