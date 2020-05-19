include	Makefile.config

PROJECT=example15
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=ocean.raw subgliep.mod
EXTRAS=
RMVLIBS=display.o interrupt.o paula.o protracker.o rmvlib.a

include	Makefile.example

data.o: ocean.raw subgliep.mod

