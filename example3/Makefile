include	Makefile.config

PROJECT=example3
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=wave.ml JagCF2.rgb jagware.rgb
EXTRAS=wave.dat
RMVLIBS=display.o interrupt.o rmvlib.a

OCAML=ocaml

include	Makefile.example

data.o: JagCF2.rgb wave.dat jagware.rgb

wave.dat: wave.ml
	$(OCAML) wave.ml > wave.dat

