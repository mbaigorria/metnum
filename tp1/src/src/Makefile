# Compilador que será utilizado
CC=g++

# flags de compilación
CFLAGS=-g -Wall -std=c++11

# flags de linkeo
LDFLAGS=

# Agrear acá los directorios a incluir en la compilación
INCDIR=../../../../../Modulos-CPP

# Agregar acá los archivos .cpp a compilar
SOURCES=buildSystem.cpp

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@
