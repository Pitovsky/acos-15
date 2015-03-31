CC=gcc
CFLAGS=-c -D LOG
LDFLAGS=
SOURCES=main.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=exec

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
