CC=gcc
DEBUG=
CFLAGS=-c -Wall -pedantic -I. $(DEBUG)

LDFLAGS=
#SOURCES=apc220_reader.c serial_lib.c
SOURCES=thingspeak.c ts_http.c ts_time.c test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test
#EXECUTABLE

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm $(OBJECTS) $(EXECUTABLE)
