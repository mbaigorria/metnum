import random
import math
import io
import settings

PI = math.pi
YES = 'y'

class RandomParametersGenerator(object):
    def __init__(self, r_i=10, r_e=100, m=30, n=30, iso=500, ninst=1, innerTemp=1500, outerTemp=200):
        self.r_i = r_i
        self.r_e = r_e
        self.m = m
        self.n = n
        self.iso = iso
        self.ninst = ninst
        self.innerTemp = innerTemp
        self.outerTemp = outerTemp
        self.lastFile = 0
        
    def generateTest(self, name, r_i=10, r_e=100, m=30, n=30, iso=500, ninst=1, tempInt=1500, tempExt=30, vecTemp=[], subindex=1, tempConst=True):
        
        self.r_i = r_i
        self.r_e = r_e
        self.m = m
        self.n = n
        self.iso = iso
        self.ninst = ninst
        self.innerTemp = tempInt
        self.outerTemp = tempExt
        # cTemp = temp1
        
        f = open("instancias/" + name + "_" + str(subindex) + ".in", 'w+')
        self.lastFile += 1
        # w+ == f.truncate() pero truncate no funciona..
        f.write(str(self.r_i) + " " + str(self.r_e) + " " + str(self.m) + " " + str(self.n) + " " + str(self.iso) + " " + str(self.ninst) + "\n")
            
        j = 0
        while j < self.ninst:   
            i = 0
            while i < 2*self.n:
                if i < self.n:
                    f.write(str(self.innerTemp) + " ")
                else:
                    if tempConst:
                        f.write(str(self.outerTemp))
                    else:
                        f.write(str(vecTemp[i - self.n]))
                    # else:
                        # if cTemp == temp2:
                            # f.write(str(temp2))
                            # cTemp = temp1
                        # else:
                            # f.write(str(temp1))
                            # cTemp = temp2
                    
                    if i < 2*self.n-1:
                        f.write(" ")
                    else:
                        f.write("\n")
                
                i+=1            

            j+=1    

        f.close()
        
rpg = RandomParametersGenerator()

# Generar isoterma ideal
rpg.generateTest(name='isotermaIdeal', r_i=settings.radioInt, r_e=settings.radioExt, m=settings.mIdeal, n=settings.nIdeal, tempInt=settings.tempInt, tempExt=settings.tempExt)

# Generar tests de m
valorM = settings.inicioM
for i in xrange(settings.cantM):
    rpg.generateTest(name="mVariable", r_i=settings.radioInt, r_e=settings.radioExt, m=valorM, n=settings.valorN, tempInt=settings.tempInt, tempExt=settings.tempExt, subindex=i+1)
    valorM += settings.aumentoM
    
# Generar tests de n
valorN = settings.inicioN
for i in xrange(settings.cantN):
    rpg.generateTest(name="nVariable", r_i=settings.radioInt, r_e=settings.radioExt, m=settings.valorM, n=valorN, tempInt=settings.tempInt, tempExt=settings.tempExt, subindex=i+1)
    valorN += settings.aumentoN

# Generar tests de propagacion
tempInt = settings.tempIntProp
for i in xrange(settings.cantMultiplos):
    rpg.generateTest(name="testProp", r_i=settings.radioIntProp, r_e=settings.radioExtProp, m=settings.valorMProp, n=settings.valorNProp, iso=settings.isotermaProp, tempInt=tempInt, tempExt=settings.tempExtProp, subindex=i+1)
    tempInt += settings.tempIntProp