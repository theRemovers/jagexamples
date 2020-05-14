include	Makefile.config

PROJECT=example3
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=wave.ml jagware.png eye.png
EXTRAS=wave.dat jagware.rgb eye.rgb
RMVLIBS=display.o interrupt.o rmvlib.a

include	Makefile.example

jagware.rgb: jagware.png
	$(CONVERTER) --binary $<

eye.rgb: eye.png
	$(CONVERTER) --binary $<

data.o: wave.dat jagware.rgb eye.rgb

wave.dat: wave.ml
	$(OCAML) wave.ml > $@

