
//connect 44780 to:
;DATA=PC0-5, PB4,PB5
;CONTROL PB1-3

.equ CLEAR=1
.equ FUNCTION=32
.equ BITS8=16
.equ BITS4=0
.equ LINES2=8


.equ ONOFF=8
.equ DISPLAY=4
.equ CURSOR=2
.equ BLINKING=1

.equ SHIFT=16
.equ SCREEN=8

.equ RW=1
.equ RS=2
.equ EN=3

.equ	SPEED	= 10; 14 для XTAL=16MHz, 10 для XTAL=8MHz,  
						; 6 для XTAL=4MHz, 5 для XTAL<4MHz


.equ DEBUG=0


PRINTSTR: ; z - stradr (flash),i - strlen

prnloop:

lpm value,z+
RCALL WRITEDATA
dec i
brne prnloop
ret

SHIFTLEFT:

ldi value,SHIFT
rjmp WRITECMD


PRINTINT: ; <20


cpi  value,10
brcs onedigit
subi value,10
push value
ldi value,49
rcall WRITEDATA
pop value
onedigit: subi value,-48
rcall WRITEDATA
ret


TEMPDELAY:
l_delay:
push temp
rcall LCDDELAY
pop temp
dec temp
brne l_delay
ret

LCDINIT:

clr temp
out PORTB,temp

ldi temp,0b00001110
out DDRB,temp

ldi temp,25
rcall TEMPDELAY

ldi value,FUNCTION | BITS8 
cbi PORTB,RS
rcall WRITE4BITS

ldi temp,25
rcall TEMPDELAY

ldi value,FUNCTION | BITS8 
cbi PORTB,RS
rcall WRITE4BITS

ldi temp,25
rcall TEMPDELAY

ldi value,FUNCTION | BITS8 
cbi PORTB,RS
rcall WRITE4BITS

ldi temp,10
rcall TEMPDELAY

ldi value,FUNCTION | BITS4
cbi PORTB,RS
rcall WRITE4BITS


ldi temp,25
rcall TEMPDELAY

ldi value,FUNCTION | BITS4 | LINES2
RCALL WRITECMD

ldi temp,0
rcall TEMPDELAY

ldi value, ONOFF
rcall WRITECMD

ldi temp,25
rcall TEMPDELAY

ldi value,CLEAR
RCALL WRITECMD

ldi temp,0
rcall TEMPDELAY
ldi temp,0
rcall TEMPDELAY

ldi value, ONOFF | DISPLAY
RCALL WRITECMD

ret


LCDDELAY:
ldi	temp,SPEED
L_loop:	dec		temp
brne	L_loop
ret

WAITBUSY:

;rjmp LCDDELAY

//ldi temp,0x80
//out PORTC,temp
clr temp
out DDRC,temp 
sbi PORTB,RW
cbi PORTB,RS
WAITBUSYLOOP:
sbi PORTB,EN
rcall LCDDELAY

in temp,PINC
push temp


cbi PORTB,EN
rcall LCDDELAY

sbi PORTB,EN
rcall LCDDELAY
cbi PORTB,EN
rcall LCDDELAY

pop temp
andi temp,0x08

brne WAITBUSYLOOP
//clr temp
//out PORTC,temp



ret


WRITEBYTE: 
rcall WRITE4BITS
;rcall WAITBUSY





WRITE4BITS:
swap value

ldi temp,0b00001110
out DDRB,temp



ldi temp,0x0f

out DDRC,temp

cbi PORTB,RW
sbi PORTB,EN

out PORTC,value

cbi PORTB,EN

rcall LCDDELAY

ret

WRITEDATA:
rcall WAITBUSY
sbi PORTB,RS
rjmp WRITEBYTE

WRITECMD:
rcall WAITBUSY
cbi PORTB,RS
rjmp WRITEBYTE

/*
READDATA:
rcall WAITBUSY
sbi PORTB,RS
sbi PORTB,RW
clr temp
out DDRB,temp
sbi PORTB,EN
in value,PORTB
cbi PORTB,EN
ret */

