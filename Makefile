CC = gcc
CFLAGS = -c -Wall -std=c99 -ggdb
LDFLAGS =
SOURCES = Varion.c moves.c bitboard.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = Varion

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm Varion $(OBJECTS)
