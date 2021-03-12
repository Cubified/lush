all: lush

CC=gcc

HAS_FZF=$(shell if [ $$(command -v fzf) ]; then echo "-DHAS_FZF"; else echo "-UHAS_FZF"; fi)

LIBS=
CFLAGS=-O3 -pipe -s -pedantic $(HAS_FZF)
DEBUGCFLAGS=-Og -pipe -g $(HAS_FZF)

INPUT=lush.c
OUTPUT=lush

RM=/bin/rm

.PHONY: lush
lush:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS)

debug:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(DEBUGCFLAGS)

nofzf:
	$(CC) $(INPUT) -o $(OUTPUT) $(LIBS) $(CFLAGS) -UHAS_FZF

clean:
	if [ -e $(OUTPUT) ]; then $(RM) $(OUTPUT); fi
