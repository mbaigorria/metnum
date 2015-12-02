import random
import math
import io
import settings

PI = math.pi
YES = 'y'

class RandomParametersGenerator(object):
    def __init__(self, n=30, m=30, alg=0, c=0.85, inst=0, epsilon=0.0001, seed=45):
        
        self.n = n
        self.alg = alg
        self.c = c
        self.inst = inst
        self.epsilon = epsilon
        self.seed = seed
        
    def generateTestSNAP(self, name, epsilon=0.0001, c=0.85, subindex=1):
    
        f = open("instancias/" + name + "_" + str(subindex) + ".in", 'w+')
        f.write("0 " + str(c) + " 0 instancias/" + name + ".txt" + " " + str(epsilon) + "\n")
        f.close()
        
        
rpg = RandomParametersGenerator()

valorC = settings.aumentoC
for i in xrange(settings.cantValoresC-1):
    rpg.generateTestSNAP(name="web-NotreDame", c=valorC, subindex=i+1)
    rpg.generateTestSNAP(name="web-BerkStan", c=valorC, subindex=i+1)
    rpg.generateTestSNAP(name="web-Stanford", c=valorC, subindex=i+1)
    valorC += settings.aumentoC



