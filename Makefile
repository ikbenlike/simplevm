CC = gcc
CFLAGS ?= -O2 -Wall -std=c99 -masm=intel -g
LDFLAGS ?= 

ODIR = ./bin

src = $(filter-out src/main.c, $(wildcard src/*.c))
obj = $(src:.c=.o)

.PHONY: clean build

build:
	@printf "Please specify one of these:\nlibsimplevm-windows\nlibsimplevm\ntests-windows\ntests"

libsimplevm: $(src)
	$(CC) $(src) $(CFLAGS) $(LDFLAGS) -c -fpic
	mv *.o ./src/
	$(CC) -shared -o $(ODIR)/libsimplevm.so $(obj)
	rm $(obj)

libsimplevm-windows: $(src)
	$(CC) $(src) $(CFLAGS) $(LDFLAGS) -shared -Wl,--out-implib,$(ODIR)/libsimplevm_dll.a -o $(ODIR)/libsimplevm.dll

clean:
	rm $(filter-out $(ODIR)/.keep, $(wildcard $(ODIR)/*))
