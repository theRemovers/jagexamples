include	Makefile.config

PROJECT=example9
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=wave.ml
EXTRAS=wave.dat
RMVLIBS=display.o interrupt.o rmvlib.a

include	Makefile.example

data.o: wave.dat

wave.dat: wave.ml
	$(OCAML) wave.ml > $@
