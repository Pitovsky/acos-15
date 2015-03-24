CC=gcc
CFLAGS=-c
LDFLAGS=
SOURCES=main.c equationSolver.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=equationSolver

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
