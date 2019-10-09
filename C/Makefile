# Michel Georges Najarian, 2018/10/15

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g

OBJS=recursive_rename.o

all: recursive_rename

recursive_rename: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
.c.o:
	$(CC) $(CFLAGS) -c $<

%.d: %.c
	gcc -MM -MF $@ $<


.PHONY: clean
clean:
	rm -f *.d *.o *.out recursive_rename
