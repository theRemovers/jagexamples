        .data

        .globl  _willowPal
        .even
_willowPal:
        incbin  "willow_rgb.pal"

        .globl  _willowMap
        .phrase
_willowMap:
        incbin  "willow.map"
