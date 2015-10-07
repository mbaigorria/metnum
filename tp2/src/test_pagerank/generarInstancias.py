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
        
    def generateTestManip(self, name, n=30, m=30, extras=0, alg=0, c=0.85, inst=0, epsilon=0.0001, seed=45, subindex=1):
        
        self.m = m
        self.n = n
        self.alg = alg
        self.c = c
        self.inst = inst
        self.epsilon = epsilon
        self.seed = seed
        
        random.seed(self.seed)
        
        f = open("manipulacion/" + name + "_" + str(subindex) + ".in", 'w+')
        f.write(str(self.alg) + " " + str(self.c) + " " + str(self.inst) + " manipulacion/" + name + "_" + str(subindex) + ".txt" + " " + str(epsilon) + "\n")
        f.close()
        
        f = open("manipulacion/" + name + "_" + str(subindex) + ".txt", 'w+')
        f.write(str(self.n + extras)+ " " + str(self.m + extras) + "\n")
        
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
            
        lastN = self.n
        for i in xrange(extras):
            f.write(str(lastN + i + 1) + " 1\n")
            
        f.close()
        
    def generateTestManipC(self, name, nameInst, alg=0, inst=0, epsilon=0.0001, samplesC = 10, subindex=1):
        
        valorC = 0
        aumentoC = 1.0/samplesC
        
        for i in xrange(samplesC):
            f = open("manipulacion/" + name + "_" + str(i+1) + ".in", 'w+')
            f.write(str(self.alg) + " " + str(valorC) + " " + str(self.inst) + " manipulacion/" + nameInst + "_" + str(subindex) + ".txt" + " " + str(epsilon) + "\n")
            valorC += aumentoC
            f.close()
        
        
rpg = RandomParametersGenerator()

# Generar isoterma ideal
rpg.generateTest(name='webInicial', n=5000, m=5000)

# Generar test de complejidad
# valorN = settings.valorN
# valorM = settings.valorM
# for i in xrange(settings.cantComplejidad):
    # rpg.generateTest(name="complejidad", n=valorN, m=valorM, alg=0, subindex=i+1)
    # valorN += settings.aumentoN
    # valorM += settings.aumentoM
    
for i in xrange(settings.extras):
    # rpg.generateTestManip(name="manip", extras=i, subindex=i+1)
    a = i
    
ultIndex = i+1

rpg.generateTestManipC(name = "manipC", nameInst = "manip", samplesC = settings.muestrasC, subindex = ultIndex)



