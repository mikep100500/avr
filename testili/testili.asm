.include "m328pdef.inc"
.include "defs.inc"
.include "macros.inc"



.cseg
.org 0
 rjmp reset

.org int_vectors_size

.include "utils.inc"
.include "uart.inc"
.include "font.inc"
.include "ili9341.inc"
//.include "tests.inc"


RESET:

/* oldreset



MOVEPMEM SOMEPOS,BOARD,BSIZE

ldi r16,4
ldi r17,15
 
rcall FINDWAY

andi r16,0xff
breq FINISH



rcall UART_INIT


rcall TFT_INI
;ldi valhigh,HIGH(CYAN)
;ldi vallow,LOW(CYAN)
;rcall TFT_FILLSCREEN
;ldi valhigh,HIGH(RED)
;ldi vallow,LOW(RED)
ldi XL,100
;clr XH
ldi YL,100
;clr YH
;rcall TFT_DRAWPIXEL

;clr XL
clr XH
;clr YL
clr YH
ldi valhigh,HIGH(RED)
ldi vallow,LOW(RED)
;ldi value,'�'
;rcall TFT_DRAWCHAR
ldi ZH,HIGH(FUCKOFF*2)
ldi ZL,LOW(FUCKOFF*2)
;rcall TFT_DRAWSTRING

ldi YL,100
clr YH
ldi valhigh,HIGH(CYAN)
ldi vallow,LOW(CYAN)

clr XL
clr XH

lds val0,x_size
lds val1,x_size+1

;rcall TFT_DRAWHLINE

ldi XL,50
clr XH
ldi valhigh,HIGH(BLUE)
ldi vallow,LOW(BLUE)

ldi YL,50
clr YH

ldi val2,100
ldi val3,0


;rcall TFT_DRAWVLINE

rcall DRAWGRID

oldreset  */



//rjmp deb
rcall UART_INIT
rcall TFT_INI




deb:


ldi temp,1
out TCCR0B,temp


ldi temp,1
sts SEED,temp
rcall INITBOARD
//rcall DRAWBOARD



ldi zl,low(dtt)
ldi zh,high(dtt)

ld number,Z+
rcall OUTNUM
ld number,Z+
rcall OUTNUM
ld number,Z+
rcall OUTNUM
ld number,Z+
rcall OUTNUM

rjmp PC



ldi zl,low(WAY)
ldi zh,high(WAY)

PRINTUART:
ld number,Z+
rcall OUTNUM
ldi value,' '
rcall OUTBYTE
dec r22
brne PRINTUART



FINISH:
;sbi DDRB,0
;cbi PORTB,0
rjmp PC


DRAWGRID:

clr XL
clr XH

ldi YL,CELLSIZE*CELLCNT
clr YH

lpdrawgridx:
ldi val0,CELLSIZE*CELLCNT
clr val1
rcall TFT_DRAWHLINE
subi YL,CELLSIZE
brne lpdrawgridx

clr YL
clr YH

ldi XL,CELLSIZE*CELLCNT
clr XH

lpdrawgridy:
ldi val2,CELLSIZE*CELLCNT
clr val3
rcall TFT_DRAWVLINE
subi XL,CELLSIZE
brne lpdrawgridy

ret


DRAWBALL: //value,val0/1-pos


ldi ZL,LOW(COLORS*2)
ldi ZH,HIGH(COLORS*2)
clr temp
lsl value
add ZL,value
adc ZH,temp
lpm vallow,Z+
lpm valhigh,Z


ldi temp,CELLSIZE
mul val0,temp
mov XL,r0
mov XH,r1

subi XL,-2
sbci XH,-1

mul val1,temp
mov YL,r0
mov YH,r1

subi YL,-2
sbci YH,-1

sts PALETTE+2,vallow
sts PALETTE+3,valhigh

ldi val0,LOW(PALETTE)
ldi val1,HIGH(PALETTE)
ldi val2,LOW(BALL*2)
ldi val3,HIGH(BALL*2)
ldi vallow,BALLSIZE
ldi valhigh,BALLSIZE
rcall TFT_DRAWIMAGE
ret

INITBOARD:
rcall DRAWGRID
ldi temp,LOW(WHITE)
sts PALETTE,temp
ldi temp,HIGH(WHITE)
sts PALETTE+1,temp
CLEARMEM BOARD,BSIZE
rcall ADDBALLS
ret


ADDBALL: ;out val0,val1 - posxy,vallow -posmul, value-color index
rcall RND
cpi value,BSIZE
brcc ADDBALL
clr temp
ldi YL,LOW(BOARD)
ldi YH,HIGH(BOARD)
add YL,value
adc YH,temp

ld temp,Y
and temp,temp
brne ADDBALL

push value
mov r20,value
ldi r21,BSIDE
rcall DIVMOD
mov val0,r19
mov val1,r18

ldi r21,7
rcall RNDN
inc value
st Y,value
pop vallow
ret


ADDBALLS:
ldi r16,255
lpaddballs:
push r16
rcall ADDBALL
push vallow
rcall DRAWBALL
pop value
rcall CHECKLINES
pop r16
dec r16
brne lpaddballs
ret


CHECKLINES:
rcall GETSQUARE
mov r1,r0 //color
ldi r16,4//compare const
mov r3,r16
ldi zh,high(BOARD)
ldi zl,low(BOARD)


clr r2 //counter
push value

lpcheckright:
mov r16,value
rcall GETRIGHT
inc r23
brne checkleft
cp r1,r0
brne checkleft
inc r2
rjmp lpcheckright

checkleft:
pop value
push value
lpcheckleft:
mov r16,value
rcall GETLEFT
inc r23
brne endcheckleft
cp r1,r0
brne endcheckleft
inc r2
rjmp lpcheckleft


endcheckleft:
cp r2,r3
brlt checkbottom
//rcall CLEARHLINE

checkbottom:
clr r2
pop value
push value

lpcheckbottom:
mov r16,value
rcall GETBOTTOM
inc r23
brne checktop
cp r1,r0
brne checktop
inc r2
rjmp lpcheckbottom

checktop:

lpchecktop:
mov r16,value
rcall GETTOP
inc r23
brne endchecktop
cp r1,r0
brne endchecktop
inc r2
rjmp lpchecktop

endchecktop:
cp r2,r3
brlt checkexit
//rcall CLEARVLINE
checkexit:
pop value
ret

CLEARHLINE:
ret

CLEARVLINE:
ret



FINDWAY: ;r16-from, r17-to out - r16 nz found,WAY contains path, r22 - waysize

MOVEMEM BOARD,FINDBOARD,BSIZE
ldi r22,0 ;waysize

findloop:
cp r16,r17
breq FOUND


rcall GETFRIGHT
and r23,r23
breq left
rcall MOVE
rjmp findloop

left: 
rcall GETFLEFT
and r23,r23
breq top
rcall MOVE
rjmp findloop

top:
rcall GETFTOP
and r23,r23
breq bottom
rcall MOVE
rjmp findloop

bottom: 
rcall GETFBOTTOM
and r23,r23
breq rollback
rcall move
rjmp findloop

rollback:
rcall MOVEBACK
and r22,r22
brne findloop

clr r16
ret
FOUND:
ser r16
ret

MOVE: ; r16 oldpos, r20 newpos
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
add zl,r20
sbci zh,0

ser r24
st Z,r24

ldi zh,high(WAY)
ldi zl,low(WAY)
add zl,r22
sbci zh,0

st Z,r20
inc r22
mov r16,r20
ret

MOVEBACK:
dec r22
ldi zh,high(WAY)
ldi zl,low(WAY)
add zl,r22
sbci zh,0

ld r16,-Z
ret



GETFRIGHT:
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
GETRIGHT: ; r16=pos,out: r23=nz;r0-color
clr r23
clr r0
ldi r21,BSIDE
mov r20,r16
rcall DIVMOD
push r18
mov r20,r16
inc r20
rcall DIVMOD
pop r19
cp r18,r19
brne exitgr
rcall GETSQUARE
brne exitgr
ser r23

exitgr:
ret

GETFLEFT:
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
GETLEFT: ; r16=pos,out: r23=nz
clr r0
clr r23
mov r20,r16
and r20,r20
breq exitgl

ldi r21,BSIDE

rcall DIVMOD
push r18
mov r20,r16
dec r20
rcall DIVMOD
pop r19
cp r18,r19
brne exitgl
rcall GETSQUARE
brne exitgl
ser r23

exitgl:
ret

GETFTOP:
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
GETTOP: ; r16=pos,out: r23=nz
clr r0
clr r23
mov r20,r16
cpi r20,BSIDE
brcs exitgt

subi r20,BSIDE
rcall GETSQUARE
brne exitgt
ser r23

exitgt:
ret


GETFBOTTOM:
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
GETBOTTOM: ; r16=pos,out: r21=nz
clr r0
clr r23
mov r20,r16
cpi r20,BSIZE-BSIDE
brcc exitgb

subi r20,-BSIDE
rcall GETSQUARE
brne exitgb
ser r23

exitgb:
ret

GETSQUARE: ; value:pos, out: z if square empty; Z - square ptr, r0 - square val
add zl,value
clr r0
adc zh,r0
ld r0,Z
and r0,r0
ret

  






DIVMOD: ;r20/r21, out r18=div, r19=mod
push r20
clr r18
 
loop: cp r20,r21
brcs exitdm
sub r20,r21
inc r18
rjmp loop

exitdm: 
mov r19,r20
pop r20
ret

RND:
lds r20,SEED
ldi r21,11
mul r20,r21
subi r20,7
rol r20

push r21
in r21,TCNT0
eor r20,r21
pop r21


lds r0,SEED
cp r0,r20
brne exitrnd
in r20,TCNT0
sts SEED,r20
rjmp RND

exitrnd:
sts SEED,r20
mov value,r20

ret

RNDN:  ; r21 N. out value
push r21
rcall RND
pop r21
mov r20,value
rcall DIVMOD
mov value,r19
ret


SOMEPOS: .db 3,1,2,0
         .db 0,0,0,3
		 .db 0,1,0,2
		 .db 0,0,0,0

FUCKOFF: .db "fuckoff!",0

BALL:	.db 0,0,0,0,0,1,1,0,0,0,0,0
		.db 0,0,0,1,1,1,1,1,1,0,0,0
		.db 0,0,1,1,1,1,1,0,1,1,0,0
		.db 0,1,1,1,1,1,1,1,0,1,1,0
		.db 0,1,1,1,1,1,1,1,1,1,1,0
		.db 1,1,1,1,1,1,1,1,1,1,1,1
		.db 1,1,1,1,1,1,1,1,1,1,1,1
		.db 0,1,1,1,1,1,1,1,1,1,1,0
		.db 0,1,1,1,1,1,1,1,0,1,1,0
		.db 0,0,1,1,1,1,1,0,1,1,0,0
		.db 0,0,0,1,1,1,1,1,1,0,0,0
		.db 0,0,0,0,0,1,1,0,0,0,0,0


COLORS: .dw BLACK,BLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,WHITE


.dseg
BOARD: .byte BSIZE
FINDBOARD: .byte BSIZE
WAY: .byte BSIZE
PALETTE: .byte 4
SEED: .byte 1
SCORE: .byte 1
