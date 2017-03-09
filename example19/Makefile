include	Makefile.config

PROJECT=example19
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=willow.png
EXTRAS=willow.map willow_rgb.pal
RMVLIBS=display.o interrupt.o rmvlib.a

include Makefile.example

data.o: willow.map willow_rgb.pal

willow.map willow_rgb.pal: willow.png
	$(CONVERTER) --binary --clut --overwrite $<
