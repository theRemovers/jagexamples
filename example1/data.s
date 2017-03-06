	.data
	
	.globl	_font

	.phrase
_font:
	incbin	"stabfnt0.rgb"

	.globl	_font_offset
	.long
_font_offset:
	include	"stabfnt0.dat"

