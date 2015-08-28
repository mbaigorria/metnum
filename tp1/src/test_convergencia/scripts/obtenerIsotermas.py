import settings
import subprocess as sp
import time
import csv

def runSystem(input, output, method, isotherm):
    sp.check_call([settings.executable, input, output, method, isotherm], stdin=None, stdout=None, stderr=None)

runSystem('instancias/isotermaIdeal_1.in', 'isotermaIdeal_1.out', str(1), 'isotermaIdeal_1.iso')

for i in xrange(settings.cantM):
    fName = 'instancias/mVariable_' + str(i+1)
    runSystem(fName + '.in', fName + '.out', str(1), fName + '.iso')
    
for i in xrange(settings.cantN):
    fName = 'instancias/nVariable_' + str(i+1)
    runSystem(fName + '.in', fName + '.out', str(1), fName + '.iso')
    
for i in xrange(settings.cantAumentos):
    fName = 'instancias/tempVariable_' + str(i+1)
    runSystem(fName + '.in', fName + '.out', str(1), fName + '.iso')
    
for i in xrange(settings.cantAleatorias):
    fName = 'instancias/tempAleatoria_' + str(i+1)
    runSystem(fName + '.in', fName + '.out', str(1), fName + '.iso')