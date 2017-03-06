include	Makefile.config

PROJECT=example1
SRCC=example1.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
RMVLIBS=display.o interrupt.o rmvlib.a

data.o: stabfnt0.rgb stabfnt0.dat

stabfnt0.rgb: stabfnt0.bmp
	$(CONVERTER) --binary stabfnt0.bmp

stabfnt0.dat: font_offset.ml
	$(OCAML) font_offset.ml > $@

include	Makefile.example
