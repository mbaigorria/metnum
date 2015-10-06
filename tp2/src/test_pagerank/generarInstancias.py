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
        
    def generateTest(self, name, n=30, m=30, alg=0, c=0.85, inst=0, epsilon=0.0001, seed=45, subindex=1):
        
        self.m = m
        self.n = n
        self.alg = alg
        self.c = c
        self.inst = inst
        self.epsilon = epsilon
        self.seed = seed
        
        random.seed(self.seed)
        
        f = open("instancias/" + name + "_" + str(subindex) + ".in", 'w+')
        f.write(str(self.alg) + " " + str(self.c) + " " + str(self.inst) + " instancias/" + name + "_" + str(subindex) + ".txt" + " " + str(epsilon) + "\n")
        f.close()
        
        f = open("instancias/" + name + "_" + str(subindex) + ".txt", 'w+')
        f.write(str(self.n)+ " " + str(self.m) + "\n")
        
        lst = []
        
        for i in xrange(self.n):
            for j in xrange(self.n):
                if i != j:
                    tup = (i+1, j+1)
                    lst.append(tup)
                
        random.shuffle(lst)
                
        for i in xrange(self.m):
            tup = lst.pop()
            f.write(str(tup[0]) + " " + str(tup[1]) + "\n")
            
        f.close()
        
rpg = RandomParametersGenerator()

# Generar isoterma ideal
rpg.generateTest(name='webInicial', n=5000, m=5000)

# Generar test de complejidad
valorN = settings.valorN
valorM = settings.valorM
for i in xrange(settings.cantComplejidad):
    rpg.generateTest(name="complejidad", n=valorN, m=valorM, alg=0, subindex=i+1)
    valorN += settings.aumentoN
    valorM += settings.aumentoM

# # Generar tests de m
# valorM = settings.inicioM
# for i in xrange(settings.cantM):
    # rpg.generateTest(name="mVariable", r_i=settings.radioInt, r_e=settings.radioExt, m=valorM, n=settings.valorN, tempInt=settings.tempInt, tempExt=settings.tempExt, subindex=i+1)
    # valorM += settings.aumentoM
    
# # Generar tests de n
# valorN = settings.inicioN
# for i in xrange(settings.cantN):
    # rpg.generateTest(name="nVariable", r_i=settings.radioInt, r_e=settings.radioExt, m=settings.valorM, n=valorN, tempInt=settings.tempInt, tempExt=settings.tempExt, subindex=i+1)
    # valorN += settings.aumentoN

# # Generar tests de propagacion
# tempInt = settings.tempIntProp
# for i in xrange(settings.cantMultiplos):
    # rpg.generateTest(name="testProp", r_i=settings.radioIntProp, r_e=settings.radioExtProp, m=settings.valorMProp, n=settings.valorNProp, iso=settings.isotermaProp, tempInt=tempInt, tempExt=settings.tempExtProp, subindex=i+1)
    # tempInt += settings.tempIntProp