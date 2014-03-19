CC = gcc
CFLAGS = -c -Wall -O3 -std=c99 -ggdb
LDFLAGS =
SOURCES = Varion.c moves.c bitboard.c minimax.c
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
