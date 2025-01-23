.PHONY: clean lwp
# Makefile for my malloc implementation for intro to operating systems

CC = gcc

CFLAGS = -Wall -g -fpic

lwp: 

intel-all: lib64/liblwp.so

lib64/liblwp.so: lib64 lwp64.o
	$(CC) $(CFLAGS) -shared -o $@ lwp64.o

lib:
	mkdir lib

magic64.o:
	$(CC) $(CFLAGS) -o magic64.o -c magic64.S

lib64:
	mkdir lib64

lwp64.o: lwp.c
	$(CC) $(CFLAGS) -m64 -c -o lwp64.o

clean:
	rm -rf lib lib64 lwp32.o lwp64.o
