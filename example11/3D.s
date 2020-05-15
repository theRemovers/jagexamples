NB_POINTS	equ	(19*2)
	

;*******************************************************

	.globl	_init_3D
_init_3D:	

	jsr	Cal_cos
	
	clr.l	Tx
	clr.l	Ty
	move.l	#-512*70,Tz

	rts

	.globl	_animate_3D
	
_animate_3D:
	movem.l	d2-d7/a2-a6,-(sp)

	.if	1
	tst.w	_pause
	bne.s	Pas_mod_c
	add.w	#2,A
	cmp.w	#720,A
	blt.s	Pas_mod_a
	sub.w	#720,A
Pas_mod_a:

	addq.w	#4,B
	cmp.w	#720,B
	blt.s	Pas_mod_b
	sub.w	#720,B
Pas_mod_b:

	addq.w	#2,C
	cmp.w	#720,C
	blt.s	Pas_mod_c
	sub.w	#720,C
Pas_mod_c:
	.endif
************
	lea	Mcos,a0
	lea	Msin,a1
	move.w	C,d0
	sub.w	B,d0
	move.w	C,d1
	add.w	B,d1
*  ia&=(COSQ(c&-b&)+COSQ(c&+b&))*prec&/2
	move.w	(a0,d0.w),d2
	add.w	(a0,d1.w),d2
	asr.w	#1,d2
	move.w	d2,Ia

*  ja&=(SINQ(c&-b&)+SINQ(c&+b&))*prec&/2

	move.w	(a1,d0.w),d2
	add.w	(a1,d1.w),d2
	asr.w	#1,d2
	move.w	d2,Ja
*  
*  ib&=(COSQ(b&+c&-a&)-COSQ(b&+c&+a&)+COSQ(b&-c&-a&)-COSQ(b&-c&+a&))*prec&/4

	move.w	B,d0
	add.w	C,d0
	sub.w	A,d0

	move.w	B,d1
	add.w	C,d1
	add.w	A,d1

	move.w	B,d2
	sub.w	C,d2
	sub.w	A,d2

	move.w	B,d3
	sub.w	C,d3
	add.w	A,d3

	move.w	C,d4
	sub.w	A,d4

	move.w	C,d5
	add.w	A,d5

*  ib&=(COSQ(b&+c&-a&)-COSQ(b&+c&+a&)+COSQ(b&-c&-a&)-COSQ(b&-c&+a&))*prec&/4
*      +(SINQ(c&-a&)+SINQ(c&+a&))*prec&/2

	move.w	0(a0,d0.w),d6
	sub.w	0(a0,d1.w),d6
	add.w	0(a0,d2.w),d6
	sub.w	0(a0,d3.w),d6
	asr.w	#2,d6

	move.w	0(a1,d4.w),d7
	add.w	0(a1,d5.w),d7
	asr.w	#1,d7
	add.w	d7,d6
	move.w	d6,Ib
	

*  ic&=(SINQ(b&+c&-a&)+SINQ(b&+c&+a&)+SINQ(b&-c&-a&)+SINQ(b&-c&+a&))*prec&/4
*      -(COSQ(c&-a&)-COSQ(c&+a&))*prec&/2

	move.w	0(a1,d0.w),d6
	add.w	0(a1,d1.w),d6
	add.w	0(a1,d2.w),d6
	add.w	0(a1,d3.w),d6
	asr.w	#2,d6

	move.w	0(a0,d4.w),d7
	sub.w	0(a0,d5.w),d7
	asr.w	#1,d7
	sub.w	d7,d6
	move.w	d6,Ic

*******************

*  jb&=(SINQ(a&+c&-b&)+SINQ(a&+b&-c&)-SINQ(a&+c&+b&)-SINQ(a&-c&-b&))*prec&/4
	move.w	A,d0
	add.w	C,d0
	sub.w	B,d0

	move.w	A,d1
	add.w	B,d1
	sub.w	C,d1

	move.w	A,d2
	add.w	B,d2
	add.w	C,d2

	move.w	A,d3
	sub.w	C,d3
	sub.w	B,d3

	move.w	A,d4
	sub.w	C,d4

	move.w	A,d5
	add.w	C,d5

*  jb&=(SINQ(a&+c&-b&)+SINQ(a&+b&-c&)-SINQ(a&+c&+b&)-SINQ(a&-c&-b&))*prec&/4
*      -(COSQ(a&-c&)+COSQ(a&+c&))*prec&/2

	move.w	0(a1,d0.w),d6
	add.w	0(a1,d1.w),d6
	sub.w	0(a1,d2.w),d6
	sub.w	0(a1,d3.w),d6
	asr.w	#2,d6

	move.w	0(a0,d4.w),d7
	add.w	0(a0,d5.w),d7
	asr.w	#1,d7
	sub.w	d7,d6
	move.w	d6,Jb

*  jc&=(COSQ(a&+c&-b&)+COSQ(b&+a&-c&)-COSQ(a&+c&+b&)-COSQ(a&-c&-b&))*prec&/4
*      +(SINQ(a&-c&)+SINQ(a&+c&))*prec&/2

	move.w	0(a0,d0.w),d6
	add.w	0(a0,d1.w),d6
	sub.w	0(a0,d2.w),d6
	sub.w	0(a0,d3.w),d6
	asr.w	#2,d6

	move.w	0(a1,d4.w),d7
	add.w	0(a1,d5.w),d7
	asr.w	#1,d7
	add.w	d7,d6
	move.w	d6,Jc

*  '
*  ka&=SINQ(b&)*prec&
	move.w	B,d0
	move.w	0(a1,d0.w),Ka
*  '
	move.w	A,d0
	add.w	B,d0

	move.w	A,d1
	sub.w	B,d1
*  kb&=(SINQ(a&+b&)+SINQ(a&-b&))*prec&/2
	move.w	(a1,d0.w),d2
	add.w	(a1,d1.w),d2
	asr.w	#1,d2
	move.w	d2,Kb
*  kc&=(COSQ(a&+b&)+COSQ(a&-b&))*prec&/2
	move.w	(a0,d0.w),d2
	add.w	(a0,d1.w),d2
	asr.w	#1,d2
	move.w	d2,Kc

	lea	Xp,a0
	lea	Yp,a1
	lea	Zp,a2
	lea	AA,a3
	lea	BB,a4
	lea	CC,a6
	
	moveq	#NB_POINTS/2-1,d7
Start:	
	lea	Ia,a5

	move.w	(a0)+,d6
	move.w	(a1)+,d5
	move.w	(a2)+,d4

	move.w	d6,d0
	move.w	d5,d1
	move.w	d4,d2

	muls	(a5)+,d0
	muls	(a5)+,d1
	muls	(a5)+,d2
	add.l	d1,d0
	sub.l	d2,d0
	move.l	d0,XX1


	move.w	d6,d0
	move.w	d5,d1
	move.w	d4,d2

	muls	(a5)+,d0
	muls	(a5)+,d1
	muls	(a5)+,d2
	add.l	d1,d0
	add.l	d2,d0
	move.l	d0,YY1

	muls	(a5)+,d6
	muls	(a5)+,d5
	muls	(a5)+,d4
	add.l	d5,d6
	add.l	d4,d6		; ZZ1

	move.l	#512*128,d0
	sub.l	d6,d0
	add.l	Tz,d0

	move.l	d0,(a6)+	

	moveq	#9,d1
	asr.l	d1,d0	
	
	move.l	XX1,d1
	divs	d0,d1
	add.w	#160,d1
	move.w	d1,(a3)+
	
	move.l	YY1,d1
	divs	d0,d1
	add.w	#120,d1
	move.w	d1,(a4)+
	dbra	d7,Start

	moveq	#NB_POINTS/2-1,d7
Start2:	
	lea	Ia,a5

	move.w	(a0)+,d6
	move.w	(a1)+,d5
	move.w	(a2)+,d4

	move.w	d6,d0
	move.w	d5,d1
	move.w	d4,d2

	muls	(a5)+,d0
	muls	(a5)+,d1
	muls	(a5)+,d2
	add.l	d1,d0
	sub.l	d2,d0

	move.w	d6,d0
	move.w	d5,d1
	move.w	d4,d2

	muls	(a5)+,d0
	muls	(a5)+,d1
	muls	(a5)+,d2
	add.l	d1,d0
	add.l	d2,d0

	muls	(a5)+,d6
	muls	(a5)+,d5
	muls	(a5)+,d4
	add.l	d5,d6
	add.l	d4,d6		; ZZ1

	move.l	d6,(a6)+	
	dbra	d7,Start2
	
; ***********************************************************

	;; 0 - 1 - 2 - 3
 	;; 4 - 5 - 6 - 7	
 	;; 8 - 9 - 10 - 11
	;; 15 - 16 - 17 - 18

	;; 0 - 15
	;; 1 - 16
	;; 2 - 17
	;; 3 - 18
	
;; 	lea	AA,a2
;; 	lea	BB,a3
;; 	lea	Buffer,a4

;; Copy_xy_line_into_buffer:

;; 	;; 0 - 1 - 2 - 3
;; 	move.w	(a2),(a4)+	; 0
;; 	move.w	(a3),(a4)+
;; 	move.w	2(a2),(a4)+	; 1
;; 	move.w	2(a3),(a4)+

;; 	move.w	2(a2),(a4)+	; 2
;; 	move.w	2(a3),(a4)+
;; 	move.w	4(a2),(a4)+	; 3
;; 	move.w	4(a3),(a4)+

;; 	move.w	4(a2),(a4)+	; 4
;; 	move.w	4(a3),(a4)+
;; 	move.w	6(a2),(a4)+	; 5
;; 	move.w	6(a3),(a4)+

;; 	move.w	6(a2),(a4)+	; 6
;; 	move.w	6(a3),(a4)+
;; 	move.w	(a2),(a4)+	; 7
;; 	move.w	(a3),(a4)+

;; 	;; 4 - 5 - 6 - 7
;; 	move.w	8(a2),(a4)+	; 8
;; 	move.w	8(a3),(a4)+
;; 	move.w	10(a2),(a4)+	; 9
;; 	move.w	10(a3),(a4)+

;; 	move.w	10(a2),(a4)+	; 10
;; 	move.w	10(a3),(a4)+
;; 	move.w	12(a2),(a4)+	; 11
;; 	move.w	12(a3),(a4)+

;; 	move.w	12(a2),(a4)+	; 12
;; 	move.w	12(a3),(a4)+
;; 	move.w	14(a2),(a4)+	; 13
;; 	move.w	14(a3),(a4)+

;; 	move.w	14(a2),(a4)+	; 14
;; 	move.w	14(a3),(a4)+
;; 	move.w	8(a2),(a4)+	; 15
;; 	move.w	8(a3),(a4)+

;; 	;; 8 - 9 - 10 - 11
;; 	move.w	16(a2),(a4)+	; 16
;; 	move.w	16(a3),(a4)+
;; 	move.w	18(a2),(a4)+	; 17
;; 	move.w	18(a3),(a4)+

;; 	move.w	18(a2),(a4)+	; 18
;; 	move.w	18(a3),(a4)+
;; 	move.w	20(a2),(a4)+	; 19
;; 	move.w	20(a3),(a4)+

;; 	move.w	20(a2),(a4)+	; 20
;; 	move.w	20(a3),(a4)+
;; 	move.w	22(a2),(a4)+	; 21
;; 	move.w	22(a3),(a4)+

;; 	move.w	22(a2),(a4)+	; 22
;; 	move.w	22(a3),(a4)+
;; 	move.w	16(a2),(a4)+	; 23
;; 	move.w	16(a3),(a4)+

;; 	;; 15 - 16 - 17 - 18
;; 	move.w	30(a2),(a4)+	; 24
;; 	move.w	30(a3),(a4)+
;; 	move.w	32(a2),(a4)+	; 25
;; 	move.w	32(a3),(a4)+

;; 	move.w	32(a2),(a4)+	; 26
;; 	move.w	32(a3),(a4)+
;; 	move.w	34(a2),(a4)+	; 27
;; 	move.w	34(a3),(a4)+

;; 	move.w	34(a2),(a4)+	; 28
;; 	move.w	34(a3),(a4)+
;; 	move.w	36(a2),(a4)+	; 29
;; 	move.w	36(a3),(a4)+

;; 	move.w	36(a2),(a4)+	; 30
;; 	move.w	36(a3),(a4)+
;; 	move.w	30(a2),(a4)+	; 31
;; 	move.w	30(a3),(a4)+

	;; 0 - 15
;; 	move.w	(a2),(a4)+	; 32
;; 	move.w	(a3),(a4)+
;; 	move.w	30(a2),(a4)+	; 33
;; 	move.w	30(a3),(a4)+

	;; 1 - 16
;; 	move.w	2(a2),(a4)+	; 34
;; 	move.w	2(a3),(a4)+
;; 	move.w	32(a2),(a4)+	; 35
;; 	move.w	32(a3),(a4)+

	;; 2 - 17
;; 	move.w	4(a2),(a4)+	; 36
;; 	move.w	4(a3),(a4)+
;; 	move.w	34(a2),(a4)+	; 37
;; 	move.w	34(a3),(a4)+

	;; 3 - 18
;; 	move.w	6(a2),(a4)+	; 38
;; 	move.w	6(a3),(a4)+
;; 	move.w	36(a2),(a4)+	; 39
;; 	move.w	36(a3),(a4)+

;; 	move.w	#-1,(a4)+
;; 	move.w	#-1,(a4)+

	;; end of animate step
	movem.l	(sp)+,d2-d7/a2-a6
	rts



;***********************************************
;*                                             *
;***********************************************

Cal_cos:
	movem.l 	d0-d7/a0-a6,-(a7)

	moveq	#12,d5

	lea	Cosinus,a0
 	lea 	Sinus,a1

 	move.w 	#2896,90(a0)	; Cos 45*4096
 	move.w 	#2896,90(a1)	; Sin 45

 	moveq 	#44,d0
Boucle0:
 	move.w 	d0,d1
 	add.w	d1,d1
	move.w	2(a0,d1.w),d2
 	asl.l	d5,d2
	move.w	2(a1,d1.w),d3
 	muls 	#71,d3		; Sin 1
 	add.l 	d2,d3
	asr.l	d5,d3
	move.w	d3,0(a0,d1.w)
	move.w	2(a1,d1.w),d2
	asl.l	d5,d2
	move.w	2(a0,d1.w),d3
 	muls 	#71,d3		; Sin 1
 	sub.l 	d3,d2
	asr.l	d5,d2
	move.w	d2,0(a1,d1.w)
 	dbra 	d0,Boucle0
 
 	moveq 	#44,d0
Boucle1:
 	move.w 	d0,d2
 	add.w   	d2,d2
 	moveq 	#90,d1
 	sub.w 	d0,d1
 	add.w	d1,d1
 	move.w 	0(a0,d2.w),0(a1,d1.w)
 	move.w 	0(a1,d2.w),0(a0,d1.w)
 	dbra	d0,Boucle1 

 	moveq 	#89,d0
 	move.w 	#180,d1
Boucle2: 
 	move.w 	d1,d2
 	add.w	d2,d2
 	move.w 	#360,d3
 	sub.w 	d2,d3
 	move.w 	0(a1,d3.w),0(a1,d2.w)
 	move.w 	0(a0,d3.w),d4
 	neg.w 	d4
 	move.w 	d4,(a0,d2.w)
 	subq.w 	#1,d1
 	dbra	d0,Boucle2

 	move.w 	#178,d0
 	move.w 	#181,d1
Boucle3:
 	move.w 	d1,d2
 	add.w	d2,d2
 	move.w 	d2,d3
 	sub.w 	#360,d3
 	move.w 	0(a1,d3.w),d4
 	neg.w 	d4
 	move.w 	d4,(a1,d2.w)
 	move.w 	#720,d3
 	sub.w 	d2,d3
 	move.w 	0(a0,d3.w),0(a0,d2.w)
 	addq.w 	#1,d1
 	dbra	d0,Boucle3

	lea	Cosinus,a0
	lea	Sinus,a1
 	move.w 	#359,d0
Divs_8:
 	move.w 	(a0),d1   
	ext.l	d1
	divs	#32,d1
 	move.w 	d1,(a0)+
 	move.w 	(a1),d1   
	ext.l	d1
	divs	#32,d1
 	move.w 	d1,(a1)+
 	dbra 	d0,Divs_8
 
	lea	Cosinus,a2
	lea	Sinus,a3
	moveq	#5,d1
Tab0:
	move.w	#359,d0
	lea	Cosinus,a0
	lea	Sinus,a1
Tab1:
	move.w	(a0)+,(a2)+
	move.w	(a1)+,(a3)+
	dbra	d0,Tab1
	dbra	d1,Tab0	

 	movem.l 	(a7)+,d0-d7/a0-a6
 	rts

;***********************************************************************

	DATA

X_inc:	dc.w	2
Y_inc:	dc.w	2

Xp:	dc.w	-25,025,025,-25,-20,020,020,-20,-14,014,014,-14,-10,000,015,-25,025,025,-25
NXp:	dc.w	-1, 0, -1, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -2

Yp:	dc.w	025,025,-27,-27,025,025,-05,-05,-10,-10,-27,-27,010,000,020,025,025,-27,-27
NYp:	dc.w	38, 38, -40, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 40, -38, -38

Zp:	dc.w	002,002,002,002,002,002,002,002,002,002,002,002,002,002,002,-02,-02,-02,-02
NZp:	dc.w	510, 510, 510, 510, 512, 512, 512, 512, 512, 512, 512, 512, 0, 0, 0, -510, -510, -510, -510

	.globl	_pause
_pause:
	dc.w	0
	
	.globl	_A
_A:	
A:	dc.w	0
	.globl	_B
_B:	
B:	dc.w	0
	.globl	_C
_C:	
C:	dc.w	0

	BSS
	
Cosinus:
	ds.w	360*3
Mcos:	ds.w	360*3
Sinus:	ds.w	360*3
Msin:	ds.w	360*3

	.globl	_X2D
	.globl	_Y2D
_X2D:	
AA:	ds.w	NB_POINTS

_Y2D:	
BB:	ds.w	NB_POINTS

	.globl	_Z3D
_Z3D:
CC:	
	ds.l	NB_POINTS
	
XX1:	ds.l	1
YY1:	ds.l	1
ZZ1:	ds.l	1

Tx:	ds.l	1
Ty:	ds.l	1
Tz:	ds.l	1

*Dx:	ds.w	1
*Dy:	ds.w	1

Ia:	ds.w	1
Ja:	ds.w	1
Ka:	ds.w	1
Ib:	ds.w	1
Jb:	ds.w	1
Kb:	ds.w	1
Ic:	ds.w	1
Jc:	ds.w	1
Kc:	ds.w	1

