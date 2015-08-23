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

	#yAxisIso = []
	yAxisIsoDiff = []
	outerTemp = []	

	while i < 2*angulos:
		arr = map(int, f.readline().split())	
		if i < angulos:		
			yAxisIsoDiff.append(r_e - arr[0])
			#yAxisIso.append(arr[0])
		elif i < 2*angulos-1:
			outerTemp.append(arr[0])
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
	title = 'Histogram of isotherm: $\pi_e$ = %d, $\pi_i$ = %d, $\Theta_0$ = %1.1f$\Theta$' % (r_i, r_e, d0)
	plt.title(title)

	#plt.subplot(211)
	#Calculo automatico del dominio	
	xAxisTheta = np.arange(0, 2, d0)
	plt.plot(xAxisTheta, yAxisIsoDiff, 'bo') 
	plt.plot(xAxisTheta, yAxisIsoDiff, '-')

	#Es igual a unir cada punto con una linea
	#f = interp1d(xAxisTheta, yAxisIsoDiff, bounds_error=False)
	#xnew = np.arange(0, 2, d0)	
	#plt.plot(xnew, f(xnew), '-') 

	plt.axis([0, 2, r_e, r_i])	 

	plt.ylabel('Difference: $x_e$ - $x_{iso}$ (more is better)')
	plt.legend(['data', 'linear interpolation'], loc='best')
	# Tweak spacing to prevent clipping of ylabel
	plt.subplots_adjust(left=0.15)

	#Plot con la posicion real de la isoterma
	#plt.subplot(212)
	#plt.plot(xAxisTheta, yAxisIso, 'bo') 
	#plt.plot(xAxisTheta, yAxisIso, 'k')
	#f = interp1d(xAxisTheta, yAxisIso, bounds_error=False)
	#xnew = np.arange(0, 2, d0)	
	#plt.plot(xnew, f(xnew), '--') 
	#plt.axis([0, 2, r_i, r_e]) 

	#plt.ylabel('Isotherm position: $x_{iso}$')
	#plt.legend(['data', 'linear interpolation'], loc='best')
	# Tweak spacing to prevent clipping of ylabel
	#plt.subplots_adjust(left=0.15)

	plt.xlabel('Theta (Angle): $[0, 2\Theta]$')
	plt.show()



plotHistogram()
