include	Makefile.config

PROJECT=example1
SRCC=example1.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=stabfnt0.bmp
EXTRAS=stabfnt0.rgb stabfnt0.dat
RMVLIBS=display.o interrupt.o rmvlib.a

include	Makefile.example

data.o: stabfnt0.rgb stabfnt0.dat

stabfnt0.rgb: stabfnt0.bmp
	$(CONVERTER) --binary stabfnt0.bmp

stabfnt0.dat: font_offset.ml
	$(OCAML) font_offset.ml > $@
