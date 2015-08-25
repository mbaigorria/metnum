import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from   scipy.interpolate import interp1d
import math
import os

PI = math.pi

def plotData():

	maximo = 0
	minimo = -1

	fileName = raw_input("please give a directory of the file: ")

	if (os.path.isfile(fileName) and os.access(fileName, os.R_OK)) == False:
		print "Could not open file! Invalid file name!"
		return 

	f = open(fileName, 'r')
	
	ninst, r_i, r_e = map(int, f.readline().split())
	
	radiosPorPlot = []
	angulosPorPlot = []

	z = 0
	
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
	
		#Plot con las diferencias entre la posicion de la isoterma y el radio externo	
		#plt.figure(1)	
		#plt.subplot(211)

		#Calculo automatico del dominio	
		xAxisTheta = np.arange(0, 2, d0)
		#plt.plot(xAxisTheta, yAxisIso, 'o') 
		plt.plot(xAxisTheta, yAxisIso, '-',  linewidth=2)

		#Es igual a unir cada punto con una linea
		#interpFunc = interp1d(xAxisTheta, yAxisIso, bounds_error=False)
		#xnew = np.arange(0, 2, d0) #Queda igual que la funcion original dado que el dominio es el mismo.	
		#xnew = np.arange(0, 2, .001)	
		#plt.plot(xnew, interpFunc(xnew), '-') 
		
		z+=1

	print maximo 
	print minimo
	#plt.axis([0, 2, r_i, r_e + len(angulosPorPlot)*3])	 
	plt.axis([0, 2, minimo + -10, maximo + 10])	
	title = 'Isotherm proximity to the external edge: $r_i$: %d, $r_e$: %d' % (r_i, r_e)
	plt.title(title)
	plt.ylabel('Isotherm position: $x_{iso}$ (less is better)')
	plt.xlabel('Theta (Angle): $[0, 2\pi]$')

	legend = []
	
	#while para las legendas
	n = 0	
	while n < len(angulosPorPlot):
		text = '$\Delta\Theta$: $\\frac{%d}{%d}$$\pi$ - $\Delta$r: %.3f' % (2, angulosPorPlot[n], (r_e-r_i)/float(radiosPorPlot[n]))

		legend.append(text)
		n+=1

	plt.legend(legend, loc='best')
	# Tweak spacing to prevent clipping of ylabel
	plt.subplots_adjust(left=0.15)
	plt.grid(True)
	plt.show()

plotData()
