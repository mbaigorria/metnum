# Métodos Numéricos - 2c 2015

Termine la busqueda de isotermas, y el script de matlab horno.m ya las grafica bien.
horno.m que es llamado desde test.m carga un archivo de entrada, test1.in, un archivo de salida, test1.out y un archivo de isotermas.

El archivo de isotermas tiene en cada linea el radio de la isoterma, arrancando con el angulo 0

Para crear estos archivos corren el buildSystem.cpp

Ejemplo:
./a.out test4.in test4.out 1 iso.out
Aca se generan dos archivos, test4.out, que tiene el vector solucion del sistema, e iso.out, que tiene las isotermas.

Hay dos metodos para calcular isotermas, generate_isotherm_lower y generate_isotherm_weighted. Hay que experimentar con ambos.

Eso se hace comentando el codigo, no agregue un parametro para poder elegir.
Asi que lo que falta es:

1. Terminar la eliminacion gausiana
2. Exp (comp de sistema). Ir variando las granularidades de angulo y de radio, analizar. Cambiar temperaturas externas con uniformidad.
3. Exp con muchas instancias, ninst > 1. Aca es donde la LU te ahorra banda. Hacer graficos de complejidad, y una tabla comparando porcentajes.
