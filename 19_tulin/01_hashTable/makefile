CC=gcc
CFLAGS=-c
LDFLAGS=
SOURCES=main.c hash.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hash

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	    $(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
