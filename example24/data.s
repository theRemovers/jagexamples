	.data

	.phrase
blob48_0_gfx:	incbin	"blob48_0.cry"
	.phrase
blob48_1_gfx:	incbin	"blob48_1.cry"
	.phrase
blob48_2_gfx:	incbin	"blob48_2.cry"
	.phrase
blob48_3_gfx:	incbin	"blob48_3.cry"
	.phrase
blob48_4_gfx:	incbin	"blob48_4.cry"
	.phrase
blob48_5_gfx:	incbin	"blob48_5.cry"
	.phrase
blob48_6_gfx:	incbin	"blob48_6.cry"
	.phrase
blob48_7_gfx:	incbin	"blob48_7.cry"
	.phrase
blob48_8_gfx:	incbin	"blob48_8.cry"
	.phrase
blob48_9_gfx:	incbin	"blob48_9.cry"
	.phrase
blob48_10_gfx:	incbin	"blob48_10.cry"
	.phrase
blob48_11_gfx:	incbin	"blob48_11.cry"
	.phrase
blob48_12_gfx:	incbin	"blob48_12.cry"

	.globl	_blob48_animation
	
	.long
_blob48_animation:
	;; ping-pong animation for the blobs
	dc.l	blob48_0_gfx
	dc.l	blob48_1_gfx
	dc.l	blob48_2_gfx
	dc.l	blob48_3_gfx
	dc.l	blob48_4_gfx
	dc.l	blob48_5_gfx
	dc.l	blob48_6_gfx
	dc.l	blob48_7_gfx
	dc.l	blob48_8_gfx
	dc.l	blob48_9_gfx
	dc.l	blob48_10_gfx
	dc.l	blob48_11_gfx
	dc.l	blob48_12_gfx
	dc.l	blob48_11_gfx
	dc.l	blob48_10_gfx
	dc.l	blob48_9_gfx
	dc.l	blob48_8_gfx
	dc.l	blob48_7_gfx
	dc.l	blob48_6_gfx
	dc.l	blob48_5_gfx
	dc.l	blob48_4_gfx
	dc.l	blob48_3_gfx
	dc.l	blob48_2_gfx
	dc.l	blob48_1_gfx
	dc.l	0
