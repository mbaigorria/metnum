# compiler
CC=g++

# compilation flags
CFLAGS=-g -Wall -std=c++11

# linker flags
LDFLAGS=

# -L comp flag
INCDIR=../

# .cpp files to compile
SOURCES=buildSystem.cpp

# objects that will be generated from .cpp files
OBJECTS=$(SOURCES:.cpp=.o)

# executable name
EXECUTABLE=sys

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

.cpp.o:
	$(CC) -L$(INCDIR) $(CFLAGS) $< -c -o $@