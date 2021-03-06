all: lush

CC=gcc

LIBS=
CFLAGS=-O3 -pipe -s -pedantic -Wno-stringop-overflow
DEBUGCFLAGS=-Og -pipe -g

INPUT=lush.c
OUTPUT=lush

RM=/bin/rm

.PHONY: lush
lush:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS)

debug:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(DEBUGCFLAGS)

clean:
	if [ -e $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
