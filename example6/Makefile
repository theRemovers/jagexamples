include Makefile.config

PROJECT=example6
SRCC=example6.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
DATA=original.png
EXTRAS=original.cry
RMVLIBS=display.o interrupt.o sound.o rmvlib.a

include Makefile.example

original.cry: original.png
	$(CONVERTER) --binary -cry --overwrite $<

data.s: original.cry
