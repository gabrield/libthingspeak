CC=gcc
DEBUG=
CFLAGS=-c -Wall -std=gnu99  -I. $(DEBUG)

LDFLAGS=
#SOURCES=apc220_reader.c serial_lib.c
SOURCES=thingspeak.c ts_http.c test.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test_update
#EXECUTABLE

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm $(OBJECTS) $(EXECUTABLE)
