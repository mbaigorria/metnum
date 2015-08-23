"""
Demo of the histogram (hist) function with a few features.

In addition to the basic histogram, this demo shows a few optional features:

    * Setting the number of data bins
    * The ``normed`` flag, which normalizes bin heights so that the integral of
      the histogram is 1. The resulting histogram is a probability density.
    * Setting the face color of the bars
    * Setting the opacity (alpha value).

"""
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
	
	j = 0
	count = 0.0
	while j < angulos:
		xAxisTheta.append(round(count,1))
		count+=d0
		j+=1
	
	#Plot con las diferencias entre la posicion de la isoterma y el radio externo
	
	#plt.figure(1)	
	plt.title(r'Histogram of isotherm: $\pi_e=100, \pi_i=100, \Theta=15$')

	#plt.subplot(211)
	plt.plot(xAxisTheta, yAxisIsoDiff, 'bo') 
	#plt.plot(xAxisTheta, yAxisIsoDiff, 'k')
	f = interp1d(xAxisTheta, yAxisIsoDiff, bounds_error=False)
	xnew = np.arange(0, 2, d0)	
	plt.plot(xnew, f(xnew), '-') 
	plt.axis([0, 2, r_i, r_e]) 

	plt.ylabel('Difference: $x_e - x_{iso}$')
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
