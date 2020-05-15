include	Makefile.config

PROJECT=example11
SRCC=main.c
SRCS=data.s 3D.s
SRCH=data.h
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=battlesphere_title.jpg
EXTRAS=battlesphere_title.cry.1 battlesphere_title.cry.2
RMVLIBS=display.o interrupt.o render.o rmvlib.a

include	Makefile.example

data.o: battlesphere_title.cry.1 battlesphere_title.cry.2

battlesphere_title.cry.1: battlesphere_title.jpg
	$(CONVERTER) -cry --binary battlesphere_title.jpg
	mv battlesphere_title.cry battlesphere_title.cry.1

battlesphere_title.cry.2: battlesphere_title.jpg
	$(CONVERTER) -cry --texture --binary battlesphere_title.jpg
	mv battlesphere_title.cry battlesphere_title.cry.2
