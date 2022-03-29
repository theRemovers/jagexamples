include	Makefile.config

PROJECT=example24
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
SRCBLOBS=blob48_0.png blob48_1.png blob48_2.png blob48_3.png
SRCBLOBS+=blob48_4.png blob48_5.png blob48_6.png blob48_7.png
SRCBLOBS+=blob48_8.png blob48_9.png blob48_10.png blob48_11.png blob48_12.png
CRYBLOBS=$(SRCBLOBS:.png=.cry)
DATA=$(SRCBLOBS)
EXTRAS=$(CRYBLOBS)
RMVLIBS=interrupt.o display.o rmvlib.a

include	Makefile.example

data.s: $(CRYBLOBS)

%.cry: %.png
	$(CONVERTER) -cry --binary --gray --glass --positive $<
