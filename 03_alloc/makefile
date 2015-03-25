CC=gcc
CFLAGS=-c 
LDFLAGS=
SOURCES=main.c alloc.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=alloc

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
