import os

# Parametros de isoterma "ideal" y temperaturas para los proximos dos tests
tempInt = 1500
tempExt = 200
mIdeal = 40
nIdeal = 40
radioInt = 10
radioExt = 100

# Paramtros test aumentando m temperatura constante
cantM = 30
inicioM = 3 # Valor inicial de m
aumentoM = 1 # En cuanto se aumentan los valores de m 
valorN = 4

# Paramtros test aumentando n temperatura constante
cantN = 30
inicioN = 4 # Valor incial de n
aumentoN = 1 # En cuanto se aumentan los valores de n
valorM = 3

# Parametros test temperatura constante con aumento
cantAumentos = 30
tempInicial = 30
tempAumento = 5

# Parametros test temperatura variable
cantAleatorias = 3
seed = 45
nTestAleatorio = 20
mTestAleatorio = 20

# Programa compilado
executable = './tp' if os.name == 'posix' else 'tp.exe'
