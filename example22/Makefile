include	Makefile.config

PROJECT=example22
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=eye.png
EXTRAS=eye.rgb
RMVLIBS=display.o interrupt.o rmvlib.a

include Makefile.example

data.o: eye.rgb

eye.rgb: eye.png
	$(CONVERTER) --binary $<
