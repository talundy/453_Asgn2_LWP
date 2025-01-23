.PHONY: clean lwp
# Makefile for my malloc implementation for intro to operating systems

CC = gcc

CFLAGS = -Wall -g -fpic -I./include

lwp: 

intel-all: lib/liblwp.so lib64/liblwp.so

lib/liblwp.so: lib lwp32.o
	$(CC) $(CFLAGS) -m32 -shared -o $@ lwp32.o

lib64/liblwp.so: lib64 lwp64.o
	$(CC) $(CFLAGS) -shared -o $@ lwp64.o

lib:
	mkdir lib

lib64:
	mkdir lib64

lwp32.o: lwp.c
	$(CC) $(CFLAGS) -m32 -c -o lwp32.o

lwp64.o: lwp.c
	$(CC) $(CFLAGS) -m64 -c -o lwp64.o

clean:
	rm -rf lib lib64 lwp32.o lwp64.o
