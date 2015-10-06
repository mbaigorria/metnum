import os

# Parametros test de complejidad
cantComplejidad = 4
valorN = 30
valorM = 30
aumentoN = 15
aumentoM = 15

# Cantidad de muestras para suavizar ruido
muestras = 5

# Programa compilado
executable = './tp' if os.name == 'posix' else 'tp.exe'
