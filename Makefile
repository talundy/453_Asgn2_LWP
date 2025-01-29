.PHONY: clean intel-all

CC = gcc

CFLAGS = -Wall -g -fpic

INC = -I ./include

LIBDIR = lib64

LIB = $(LIBDIR)/liblwp.so


SRCS = lwp.c ./include/magic64.S RoundRobin.c
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.S=.o)


# Default target
intel-all: $(LIB)


$(LIB): $(OBJS) | $(LIBDIR)
	$(CC) $(CFLAGS) $(INC) -shared -o $@ $(OBJS)

./include/magic64.o: ./include/magic64.S
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

RoundRobin.o: RoundRobin.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

lwp.o: lwp.c
	$(CC) $(CFLAGS) $(INC) -m64 -c $< -o $@

$(LIBDIR):
	mkdir -p $@

clean:
	rm -f $(OBJS) $(LIB)
	rm -rf $(LIBDIR)