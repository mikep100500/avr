.include "m328pdef.inc"
.include "defs.inc"
.include "macros.inc"



.cseg
.org 0
rjmp reset

.org int_vectors_size

//.include "hd44780.inc"
.include "uart.inc"
.include "keyboard.inc"
.include "LCD4.asm"
.include "LCD4_macro.inc"



RESET:



//rcall LCDINIT
INIT_LCD
WR_DATA 'A'
RJMP PC


//rjmp PC

ldi temp,OPDIV
sts OP,temp

//ldi value,5
//rcall PRINTINT

//ldi zl,low(PREVED*2)
//ldi zh,high(PREVED*2)
//ldi i,6
//rcall PRINTSTR

rcall KEYBINIT
READSYMLOOP:
rcall READSYM
brcs READSYMLOOP

cpi value,'*'
brne LPCHAR


lds temp,OP
inc temp
andi temp,0b11
sts OP,temp

and temp,temp
brne LPMINUS
ldi value,'+'
rjmp LPOP

LPMINUS:
push temp
//rcall SHIFTLEFT
pop temp

dec temp
brne LPMUL
ldi value,'-'
rjmp LPOP

LPMUL:
dec temp
brne LPDIV
ldi value,'*'
rjmp LPOP

LPDIV:
ldi value,'/'
rjmp LPOP

LPCHAR:
ldi temp,OPDIV
sts OP,temp

LPOP:
//rcall WRITEDATA
rcall ANTIBOUNCE

rjmp READSYMLOOP


FINISH:
sbi DDRB,0
cbi PORTB,0
rjmp PC




ANTIBOUNCE:
rcall DELAY_50US
rcall READSYM
brcc ANTIBOUNCE
ret




DELAY_50US:
  ldi temp,0  ; $017700 - ���� �������� � 30�� ��� 16���
  delay_50us_loop:
  subi temp,1 
  brne delay_50us_loop
  ret

DELAY_5US:
  ldi temp,25  ; $017700 - ���� �������� � 30�� ��� 16���
  delay_5us_loop:
  subi temp,1 
  brne delay_5us_loop
  ret


DELAY_1US:
  ldi temp,5  ; $017700 - ���� �������� � 30�� ��� 16���
  delay_1us_loop:
  subi temp,1 
  brne delay_1us_loop
ret




PREVED: .
.db "������"


.dseg
BOARD: .byte BSIZE
FINDBOARD: .byte BSIZE
WAY: .byte BSIZE
OP: .byte 1


