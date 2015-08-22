import random
import math
import io

PI = math.pi
YES = 'y'

class RandomParametersGenerator(object):
	def __init__(self, r_i=10, r_e=100, m=30, n=30, iso=500, ninst=1, innerTemp=1500):
		self.r_i = r_i
		self.r_e = r_e
		self.m = m
		self.n = n
		self.iso = iso
		self.ninst = ninst
		self.innerTemp = innerTemp
		
	def generateRandomOutPut(self):
		self.ninst = raw_input("Enter ninst value: ")
		res = raw_input("Would you use standard inputs or maybe change with other values? \n please, press Y or n: ")
		if res.lower() == YES:
			self.r_i = raw_input("r_i: ")
			self.r_e = raw_input("r_e: ")
			self.m = raw_input("m: ")
			self.n = raw_input("n: ")
			self.iso = raw_input("iso: ")

		res = raw_input("Would you use standard inner temp or maybe change it with other value? \n please, press Y or n: ")
		if res.lower() == YES:
			self.innerTemp = raw_input("Enter new innerTemp: ")

		f = open('dataTest', 'w+')
		f.truncate()
		f.write("hola")
		f.close()		
			
rpg = RandomParametersGenerator()
rpg.generateRandomOutPut()
