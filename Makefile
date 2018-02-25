CC = gcc
CFLAGS ?= -O2 -Wall -std=c11 -masm=intel -g
LDFLAGS ?= 

ODIR = ./bin

src = $(filter-out src/main.c, $(wildcard src/*.c))
obj = $(src:.c=.o)

.PHONY: clean build

build:
	@printf "Please specify one of these:\nlibsimplevm-windows\nlibsimplevm\ntests\ntests-windows\nstandalone\nstandalone-windows"

libsimplevm: $(src)
	$(CC) $(src) $(CFLAGS) $(LDFLAGS) -c -fpic
	mv *.o ./src/
	$(CC) -shared -o $(ODIR)/libsimplevm.so $(obj)
	rm $(obj)

libsimplevm-windows: $(src)
	$(CC) $(src) $(CFLAGS) -D ON_WINDOWS $(LDFLAGS) -shared -Wl,--out-implib,$(ODIR)/libsimplevm_dll.a -o $(ODIR)/libsimplevm.dll

tests-windows: $(src)
	$(CC) $(CFLAGS) -D ON_WINDOWS -L$(ODIR) -lsimplevm src/tests/function.c -o $(ODIR)/functions
	$(CC) $(CFLAGS) -D ON_WINDOWS -L$(ODIR) -lsimplevm src/tests/prints.c -o $(ODIR)/prints
	$(CC) $(CFLAGS) -D ON_WINDOWS -L$(ODIR) -lsimplevm src/tests/conditionals.c -o $(ODIR)/conditionals

standalone: $(src)
	$(CC) $(CFLAGS) $(LDFLAGS) $(src) src/main.c -o $(ODIR)/simplevm

standalone-windows: $(src)
	$(CC) $(CFLAGS) -D ON_WINDOWS $(src) src/main.c -o $(ODIR)/simplevm

clean:
	rm $(filter-out $(ODIR)/.keep, $(wildcard $(ODIR)/*))
