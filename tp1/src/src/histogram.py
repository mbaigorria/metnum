import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
from   scipy.interpolate import interp1d
import math
import os

PI = math.pi

def plotHistogram():

	fileName = raw_input("please give a directory of the file: ")

	if (os.path.isfile(fileName) and os.access(fileName, os.R_OK)) == False:
		print "Could not open file! Invalid file name!"
		return 

	f = open(fileName, 'r')
	
	angulos, r_i, r_e, iso, innerTemp = map(int, f.readline().split())	

	i = 0

	yAxisIso = []	

	while i < angulos:
		arr = map(int, f.readline().split())	
		if i < angulos:		
			yAxisIso.append(arr[0])

		i+=1

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

	title = 'Histogram of isotherm: $\pi_i$ = %d, $\pi_e$ = %d, $\Theta_0$ = %1.1f$\Theta$' % (r_i, r_e, d0)
	plt.title(title)

	#Calculo automatico del dominio	
	xAxisTheta = np.arange(0, 2, d0)
	plt.plot(xAxisTheta, yAxisIso, 'bo') 
	plt.plot(xAxisTheta, yAxisIso, '-')

	#Es igual a unir cada punto con una linea
	#f = interp1d(xAxisTheta, yAxisIso, bounds_error=False)
	#xnew = np.arange(0, 2, d0) #Queda igual que la funcion original dado que el dominio es el mismo.	
	#xnew = np.arange(0, 2, 0.1)	
	#plt.plot(xnew, f(xnew), '-') 

	plt.axis([0, 2, r_i, r_e])	 

	plt.ylabel('Isotherm position: $x_{iso}$ (less is better)')
	plt.legend(['data', 'linear interpolation'], loc='best')
	# Tweak spacing to prevent clipping of ylabel
	plt.subplots_adjust(left=0.15)

	plt.xlabel('Theta (Angle): $[0, 2\Theta]$')
	plt.show()



plotHistogram()
