	.data
	
	.globl	_ocean_snd
	.even
_ocean_snd:
 	incbin	"ocean.raw"

	.globl	_muzaxx
	.even
_muzaxx:
 	incbin	"subgliep.mod"
	.rept	31
	dc.w	0
	.endr
			
