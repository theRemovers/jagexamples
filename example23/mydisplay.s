	include	"jaguar.inc"

	include	"display_def.inc"

MAX_SPRITES	equ	16

	.text
	.68000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Procedure: format_link
;
;    Inputs: d1.l/d0.l is a 64-bit phrase
;            d2.l contains the LINK address to put into bits 42-24 of phrase
;
;   Returns: Updated phrase in d1.l/d0.l

format_link:
	movem.l d2-d3,-(sp)

	andi.l  #$3FFFF8,d2             ; Ensure alignment/valid address
	move.l  d2,d3                   ; Make a copy

	swap	d2	; Put bits 10-3 in bits 31-24
	clr.w	d2
	lsl.l   #5,d2
	or.l    d2,d0

	lsr.l   #8,d3                   ; Put bits 21-11 in bits 42-32
	lsr.l   #3,d3
	or.l    d3,d1

	movem.l (sp)+,d2-d3             ; Restore regs
	rts

	.globl	_a_vde
	.globl	_a_vdb
	.globl	_video_height
	.globl	_video_width

	.globl	_display_init
_display_init:
	movem.l	d2-d3,-(sp)
	;;
	move.l	#obj_list,a0
	move.l	#stop_obj,d2	; Address of STOP object
	;;
	moveq	#0,d1
	move.l	#(BRANCHOBJ|O_BRLT),d0
	bsr	format_link
	move.w	_a_vde,d3
	lsl.w	#3,d3
	or.w	d3,d0
	move.l	d1,(a0)+
	move.l	d0,(a0)+
	;;
	and.l	#$FF000007,d0
	or.l	#O_BRGT,d0
	move.w	_a_vdb,d3
	lsl.w	#3,d3
	or.w	d3,d0
	move.l	d1,(a0)+
	move.l	d0,(a0)+
	;;
	move.l	#stop_obj,a0
	moveq	#0,d1
	move.l	#STOPOBJ,d0
	move.l	d1,(a0)+
	move.l	d0,(a0)+
	;;

	bsr	_display_update
	bsr	_display_refresh

	movem.l	(sp)+,d2-d3

	move.l	#obj_list,d0
	swap	d0
	move.l	d0,OLP
	rts

	.globl	_display_refresh
_display_refresh:
	move.l	#sprite_objs_copy,a1
	lea	MAX_SPRITES*16(a1),a1
	move.l	#sprite_objs,a0
	lea	MAX_SPRITES*16(a0),a0
	moveq	#MAX_SPRITES-1,d0
.copy:
	move.l	-(a1),-(a0)
	move.l	-(a1),-(a0)
	move.l	-(a1),-(a0)
	move.l	-(a1),-(a0)
	dbf	d0,.copy
        tst.w   _display_interlaced
        beq.s   .field_done
        btst.b  #(11-8),VC
        bne.s   .bottom_field
.top_field:
        or.w    #1,_VI
        move.w  _VI,VI
        bra.s   .field_done
.bottom_field:
        and.w   #$fffe,_VI
        move.w  _VI,VI
        moveq   #MAX_SPRITES-1,d0
.fix:
        move.b  7(a0),d1
        and.b   #%111,d1        ; TYPE of object
        bne.s   .skip           ; != BITOBJ
        move.l  10(a0),d1
        and.l   #$3ff000,d1     ; IWIDTH
        lsr.l   #1,d1
        add.l   d1,(a0)         ; add IWIDTH to DATA
.skip:
        add.w   #16,a0
        dbf     d0,.fix
.field_done:
	rts

O_BRA	equ	(O_BREQ | ($7ff << 3))

_display_compute_sprite:
	;; a0 = pointer to generated sprite objects
	;; a1 = pointer to sprite
	;; d0 = pointer to next object
	movem.l	d2-d5,-(sp)
	move.l	d0,d2
	moveq	#0,d0
	moveq	#0,d1
	bsr	format_link	; link to next object

	move.l	a1,d2
	beq	.skip_sprite

	move.w	SPRITE_WORD1(a1),d2
	btst	#13,d2		; is it visible?
	bne	.skip_sprite	; no => skip it

	move.l	SPRITE_DATA(a1),d3
	beq	.skip_sprite

	move.w	SPRITE_WORD4(a1),d4
	and.w	#$3ff,d4	; HEIGHT

	move.w	SPRITE_WORD3(a1),d5
	and.w	#$ffc,d5	; DWIDTH*4 (DWIDTH is in phrase)

	move.w	SPRITE_Y(a1),d2
	add.w	_display_offset_y,d2
	lsl.w	#1,d2
	bge.s	.y_positive
.y_negative:
	neg.w	d2
	mulu.w	d2,d5		; |Y|*DWIDTH*8
	add.l	d5,d3		; DATA += |Y|*DWIDTH*8
	lsr.w	#1,d2
	sub.w	d2,d4		; HEIGHT -= |Y|
	clr.w	d2
.y_positive:
	tst.w	d4		; check HEIGHT > 0
	ble	.skip_sprite

        tst.w   _display_interlaced
        beq.s   .y_not_interlaced
	lsr.w	#1,d2
.y_not_interlaced:
	add.w	_a_vdb,d2
        addq.w  #1,d2           ; a_vdb is odd
	lsl.w	#3,d2
	and.w	#$3ff0,d2	; make Y even
	or.w	d2,d0		; YPOS

	lsl.l	#8,d4
        lsl.l   #5,d4
        tst.w   _display_interlaced
        bne.s   .height_interlaced
        add.l   d4,d4
.height_interlaced:
	and.l	#$ffc000,d4
	or.l	d4,d0		; HEIGHT

	and.l	#$fffff8,d3	; phrase aligned
	lsl.l	#8,d3
	or.l	d3,d1

	move.l	d1,(a0)+
	move.l	d0,(a0)+

	move.l	SPRITE_WORD1(a1),d1
	move.l	SPRITE_WORD3(a1),d0

        tst.w   _display_interlaced
        beq.s   .dwidth_not_interlaced
        and.l   #$f003ffff,d0
        move.w  SPRITE_WORD3(a1),d2
        and.w   #$0ffc,d2
        add.w   d2,d2
        and.w   #$0ffc,d2
        swap    d0
        or.w    d2,d0
        swap    d0
.dwidth_not_interlaced:
	and.l	#$007fffff,d1
	and.l	#$fffff000,d0

	move.w	SPRITE_X(a1),d2
	add.w	_display_offset_x,d2
	and.w	#$fff,d2
	or.w	d2,d0

	move.l	d1,(a0)+
	move.l	d0,(a0)+
	movem.l	(sp)+,d2-d5
	rts
.skip_sprite:
	or.w	#BRANCHOBJ|O_BRA,d0
	move.l	d1,(a0)+
	move.l	d0,(a0)+
	addq.w	#8,a0
	movem.l	(sp)+,d2-d5
	rts

	.globl	_display_update_sprite
_display_update_sprite:
	moveq	#0,d0
	move.w	4+2(sp),d0	; sprite number
	blt.s	.error
	cmp.w	#MAX_SPRITES,d0
	bge.s	.error
	lsl.w	#2,d0
	move.l	#_display_sprites,a0
	move.l	(a0,d0.w),a1
	lsl.w	#2,d0
	move.l	#sprite_objs_copy,a0
	add.w	d0,a0
	add.l	#sprite_objs,d0
	add.l	#16,d0
	bra	_display_compute_sprite
.error:
	rts

	.globl	_display_update
_display_update:
	movem.l	d2-d3/a2,-(sp)
	moveq	#MAX_SPRITES-1,d2
	move.l	#sprite_objs_copy,a0
	move.l	#sprite_objs,d3
	move.l	#_display_sprites,a2
.update:
	move.l	(a2)+,a1
	add.l	#16,d3
	move.l	d3,d0
	bsr	_display_compute_sprite
	dbf	d2,.update
	movem.l	(sp)+,d2-d3/a2
	rts

	.globl	_stop_object

_display_cleanup:
	move.l	#_stop_object,d0
	swap	d0
	move.l	d0,OLP
	rts

	.bss

	.dphrase
obj_list:
	ds.l	2		; BRANCH
	ds.l	2		; BRANCH
sprite_objs:
	.rept	MAX_SPRITES
	ds.l	4		; SPRITE
	.endr
stop_obj:
	ds.l	2		; STOP

sprite_objs_copy:
	.rept	MAX_SPRITES
	ds.l	4
	.endr

	.globl	_display_sprites
_display_sprites:
	ds.l	MAX_SPRITES

	.globl	_display_offset_x
_display_offset_x:
	ds.w	1

	.globl	_display_offset_y
_display_offset_y:
	ds.w	1

	.globl	_display_interlaced
_display_interlaced:
	ds.w	1

        .globl  _VI
_VI:
        ds.w    1

