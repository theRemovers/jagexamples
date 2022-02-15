include	Makefile.config

PROJECT=example20
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=egypt.png egypt.lev
RMVLIBS=display.o interrupt.o rmvlib.a

include Makefile.example

data.o: egypt.map egypt_rgb.pal egypt.lev

egypt.map egypt_rgb.pal: egypt.png
	$(CONVERTER) --binary --clut --overwrite $<
