CC = gcc
CFLAGS = -c -D LOG
LDFLAGS = -lpthread 
SOURCES = main.c stringMagic.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = paralloc

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
