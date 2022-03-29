include	Makefile.config

PROJECT=example19
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=willow.png
EXTRAS=willow.map willow_rgb.pal
RMVLIBS=display.o gpudriver.o interrupt.o lz77.o rmvlib.a

include Makefile.example

data.o: willow.map.lz77 willow_rgb.pal

%.lz77: %
	$(LZ77) p $< $@

willow.map willow_rgb.pal: willow.png
	$(CONVERTER) --binary --clut --overwrite $<
