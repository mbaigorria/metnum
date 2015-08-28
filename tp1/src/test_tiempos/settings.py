import os

# Paramtros test de dimension
cantDimVariable = 30
inicioDim = 3 # Primer valor para m y n
aumentoDim = 1 # En cuanto se aumentan los valores de m y n

# Parametros test de instancias
cantInstVariable = 15
inicioInst = 1
aumentoInst = 1
valorM = 15
valorN = 15

# Diferentes variantes del ejercicio
variantes = ['EG', 'LU']

# Muestras por instancia
muestras = 10

# Programa compilado
executable = './tp' if os.name == 'posix' else 'tp.exe'