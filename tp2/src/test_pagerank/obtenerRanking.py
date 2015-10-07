import settings
import subprocess as sp
import time
import csv

def runSystem(input, output):
    sp.check_call([settings.executable, input, output], stdin=None, stdout=None, stderr=None)
    
def getTime():
    return 1000000 * time.time()

# runSystem('instancias/isotermaIdeal_1.in', 'instancias/' + settings.criterio + '/isotermaIdeal_1.out', str(0), 'isotermas/' + settings.criterio + '/isotermaIdeal_1.iso.out')

# valorN = settings.valorN
# with open('resultados/complejidad.csv', 'wb') as csvfile:
    # writer = csv.writer(csvfile, delimiter=',')
    # writer.writerow(['n'] + ['Time'])
    # for j in xrange(settings.cantComplejidad):
        # for i in xrange(settings.muestras):
            # writer = csv.writer(csvfile, delimiter=',')
            # start_time = getTime()
            # fName = 'complejidad_' + str(j+1)
            # runSystem('instancias/' + fName + ".in", 'ranking/' + fName + ".out")
            # end_time = getTime() - start_time
            # writer.writerow([str(valorN)] + [str(end_time)])
            
        # valorN += settings.aumentoN

for i in xrange(settings.extras):
    fName = 'manip_' + str(i+1)
    runSystem('manipulacion/' + fName + '.in', 'manipulacion/out/' + fName + '.out')
    
for i in xrange(settings.muestrasC):
    fName = 'manipC_' + str(i+1)
    runSystem('manipulacion/' + fName + '.in', 'manipulacion/out/' + fName + '.out')
        
# valorM = settings.inicioM
# with open('resultados/mVariable_' + settings.criterio + '.csv', 'wb') as csvfile:
    # writer = csv.writer(csvfile, delimiter=',')
    # writer.writerow(['m'] + ['Time'])
    # for j in xrange(settings.cantM):
        # for i in xrange(settings.muestras):
            # writer = csv.writer(csvfile, delimiter=',')
            # start_time = getTime()
            # fName = 'mVariable_' + str(j+1)
            # runSystem('instancias/' + fName + '.in', 'instancias/' + settings.criterio + '/' + fName + '.out', str(0), 'isotermas/' + settings.criterio + '/' + fName + '.iso.out')
            # end_time = getTime() - start_time
            # writer.writerow([str(valorM)] + [str(end_time)])
            
        # valorM += settings.aumentoM
            
# for i in xrange(settings.cantN):
    # fName = 'nVariable_' + str(i+1)
    # runSystem('instancias/' + fName + '.in', 'instancias/' + settings.criterio + '/' + fName + '.out', str(0), 'isotermas/' + settings.criterio + '/' + fName + '.iso.out')

# for i in xrange(settings.cantMultiplos):
    # fName = 'testProp_' + str(i+1)
    # runSystem('instancias/' + fName + '.in', 'instancias/' + settings.criterio + '/' + fName + '.out', str(0), 'isotermas/' + settings.criterio + '/' + fName + '.iso.out')