import os

# Parametros de isoterma "ideal" y temperaturas para los proximos dos tests
tempInt = 1500
tempExt = 100
mIdeal = 2000
nIdeal = 2
radioInt = 10
radioExt = 90

# Paramtros test aumentando m temperatura constante
cantM = 100
inicioM = 8 # Valor inicial de m
aumentoM = 4 # En cuanto se aumentan los valores de m 
valorN = 2

# Paramtros test aumentando n temperatura constante
cantN = 12
inicioN = 4 # Valor incial de n
aumentoN = 1 # En cuanto se aumentan los valores de n
valorM = 30

# Parametros test temperatura con picos
cantAngulos = 10
temp1 = 30
temp2 = 200
cantRadios = 10

# Cantidad de muestras para el test de m
muestras = 5

# Criterio a utilizar (w para weighted, l para lower)
criterio = 'l'

# Programa compilado
executable = './tp_' + criterio if os.name == 'posix' else 'tp_' + criterio + '.exe'
