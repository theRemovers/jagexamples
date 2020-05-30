	.data

	.globl  _egyptPal
	.even
_egyptPal:
	incbin  "egypt_rgb.pal"

        .globl  _black
        .phrase
_black:
        dcb.l    8, 0

	.globl  _egyptTiles
	.phrase
_egyptTiles:
	incbin  "egypt.map"

	.globl	_egyptLevel
	.even
_egyptLevel:
	incbin	"egypt.lev"
