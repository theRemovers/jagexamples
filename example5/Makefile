include	Makefile.config

PROJECT=example5
SRCC=main.c
SRCS=data.s
SRCH=
OBJS=$(SRCC:.c=.o) $(SRCS:.s=.o)
OTHEROBJS=
DATA=imp1.gif imp2.gif imp3.gif imp4.gif imp5.gif imp6.gif imp7.gif
EXTRAS=imp1.rgb imp2.rgb imp3.rgb imp4.rgb imp5.rgb imp6.rgb imp7.rgb
RMVLIBS=display.o interrupt.o rmvlib.a

include	Makefile.example

data.o: data_def.s imp1.rgb imp2.rgb imp3.rgb imp4.rgb imp5.rgb imp6.rgb imp7.rgb

%.rgb: %.gif
	$(CONVERTER) --binary $<
