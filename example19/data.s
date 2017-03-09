        .data

        .globl  _willowPal
        .even
_willowPal:
        incbin  "willow_rgb.pal"

        .globl  _willowPacked
        .long
_willowPacked:
        incbin  "willow.map.lz77"
