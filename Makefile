CC = gcc
CFLAGS ?= -O2 -Wall -std=c99 -masm=intel -g
LDFLAGS ?= 

ODIR = ./bin

src = $(wildcard src/*.c)
obj = $(src:.c=.o)

simplevm: $(src)
	$(CC) $(src) $(CFLAGS) $(LDFLAGS) -o $(ODIR)/simplevm

.PHONY: clean

clean:
	rm $(ODIR)/simplevm
