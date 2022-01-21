.include "m328pdef.inc"
.include "defs.inc"
.include "macros.inc"



.cseg
.org 0
rjmp reset

.org int_vectors_size

.include "hd44780.inc"
.include "uart.inc"



RESET:



;rcall LCDINIT

;clr r1
;lplongdel:
;clr r0
;lplcddel:
;dec r0
;brne lplcddel
;dec r1
;brne lplongdel

;ldi value,'1'
;rcall WRITEDATA
;ldi value,'1'
;rcall WRITEDATA
;ldi value,'0'
;rcall WRITEDATA
;ldi value,'0'
;rcall WRITEDATA
;ldi value,'2'
;rcall WRITEDATA
;ldi value,'2'
;rcall WRITEDATA
;ldi value,'5'
;rcall WRITEDATA
;ldi value,'5'
;rcall WRITEDATA
;ldi value,'6'
;rcall WRITEDATA
;ldi value,'6'
;rcall WRITEDATA
;sbi DDRB,0
;sbi PORTB,0
;rjmp PC




MOVEPMEM SOMEPOS,BOARD,BSIZE

ldi r16,4
ldi r17,15
 
rcall FINDWAY

andi r16,0xff
breq FINISH

push r22

rcall LCDINIT
ldi zl,low(WAY)
ldi zh,high(WAY)

PRINTWAY: 

ld value,Z+
rcall PRINTINT
ldi value,' '
rcall WRITEDATA

dec r22
brne PRINTWAY




pop r22
rcall UART_INIT

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
sbi DDRB,0
cbi PORTB,0
rjmp PC



FINDWAY: ;r16-from, r17-to out - r16 nz found,WAY contains path, r22 - waysize

MOVEMEM BOARD,FINDBOARD,BSIZE
ldi r22,0 ;waysize

findloop:
cp r16,r17
breq FOUND


rcall GETRIGHT
and r23,r23
breq left
rcall MOVE
rjmp findloop

left: 
rcall GETLEFT
and r23,r23
breq top
rcall MOVE
rjmp findloop

top:
rcall GETTOP
and r23,r23
breq bottom
rcall MOVE
rjmp findloop

bottom: 
rcall GETBOTTOM
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


GETRIGHT: ; r20=pos,out: r23=nz
clr r23
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


GETLEFT: ; r20=pos,out: r23=nz
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

GETTOP: ; r20=pos,out: r23=nz
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

GETBOTTOM: ; r20=pos,out: r21=nz
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



GETSQUARE: ; r20:pos, out: z if square empty
ldi zh,high(FINDBOARD)
ldi zl,low(FINDBOARD)
add zl,r20
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


SOMEPOS: .db 3,1,2,0
         .db 0,0,0,3
		 .db 0,1,0,2
		 .db 0,0,0,0

.dseg
BOARD: .byte BSIZE
FINDBOARD: .byte BSIZE
WAY: .byte BSIZE

