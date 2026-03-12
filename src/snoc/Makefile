CC     = gcc
CFLAGS = -Wall -Wno-unused-function -Wno-incompatible-pointer-types -g -O0 -I.
BISON  = bison
FLEX   = flex

SRCS   = sno.tab.c lex.yy.c emit.c main.c
OBJS   = $(SRCS:.c=.o)

.PHONY: all clean

all: snoc

snoc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

sno.tab.c sno.tab.h: sno.y
	$(BISON) -d -o sno.tab.c sno.y

lex.yy.c: sno.l sno.tab.h
	$(FLEX) -o lex.yy.c sno.l

%.o: %.c sno.tab.h snoc.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f snoc *.o sno.tab.c sno.tab.h lex.yy.c
