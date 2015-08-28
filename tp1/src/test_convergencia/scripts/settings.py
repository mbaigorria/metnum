import os

# Parametros de isoterma "ideal" y temperaturas para los proximos dos tests
tempInt = 1500
tempExt = 200
mIdeal = 100
nIdeal = 100

# Paramtros test aumentando m
cantM = 10
inicioM = 3 # Valor inicial de m
aumentoM = 1 # En cuanto se aumentan los valores de m 
valorN = 5

# Paramtros test aumentando n
cantN = 10
inicioN = 3 # Valor incial de n
aumentoN = 1 # En cuanto se aumentan los valores de n
valorM = 5

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