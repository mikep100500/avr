
//connect 44780 to:
;DATA=PC0-5, PB4,PB5
;CONTROL PB1-3

.equ CLEAR=1
.equ FUNCTION=32
.equ BITS8=16
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

.equ	SPEED	= 10	; 14 для XTAL=16MHz, 10 для XTAL=8MHz,  
						; 6 для XTAL=4MHz, 5 для XTAL<4MHz


.equ DEBUG=1

TEMPDELAY:
l_delay:
push temp
rcall LCDDELAY
pop temp
dec temp
brne l_delay
ret

SHIFTLEFT:

ldi value,SHIFT
rjmp WRITECMD


PRINTSTR: ; z - stradr (flash),i - strlen

prnloop:

lpm value,z+
RCALL WRITEDATA
dec i
brne prnloop
ret


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


LCDINIT:
ldi value,FUNCTION | BITS8 | LINES2 
RCALL WRITECMD
ldi value,CLEAR
RCALL WRITECMD

;ldi value,6
;rcall WRITECMD

ldi value, ONOFF | DISPLAY
RCALL WRITECMD

;ldi value,2
;rcall WRITECMD

ret


LCDDELAY:
ldi	temp,SPEED
L_loop:	dec		temp
brne	L_loop
ret

WAITBUSY:
ldi temp,0b00001110
out DDRB,temp
clr temp
out DDRC,temp 
sbi PORTB,5
sbi PORTB,RW
cbi PORTB,RS
WAITBUSYLOOP:
sbi PORTB,EN
rcall LCDDELAY

.if DEBUG

in temp,PINC
andi temp,0b1000000

.else

in temp,PINB
andi temp,0b00100000

.endif
cbi PORTB,EN
brne WAITBUSYLOOP
clr temp
out DDRB,temp
out DDRC,temp
cbi PORTB,5
ret


WRITEBYTE: 
ldi temp,0b00111110
out DDRB,temp

.if DEBUG

ldi temp,0xff

.else 

ldi temp,0b00111111

.endif

out DDRC,temp

cbi PORTB,RW
sbi PORTB,EN

.if DEBUG

out PORTC,value

.else

mov r0,value
ldi temp,0b00111111
and r0,temp
in temp,PORTC
andi temp,0b11000000
or temp,r0
out PORTC,temp
mov r0,value
ldi temp,0b11000000
and r0,temp
lsr r0
lsr r0
in temp,PORTB
andi temp,0b11001111
or temp,r0
out PORTB,temp

.endif


cbi PORTB,EN

rcall LCDDELAY

clr temp
out DDRB,temp
out DDRC,temp
ret

WRITEDATA:
//rcall WAITBUSY
sbi PORTB,RS
rjmp WRITEBYTE

WRITECMD:
//rcall WAITBUSY
cbi PORTB,RS
rjmp WRITEBYTE

/*
READDATA:
rcall WAITBUSY
sbi PORTC,RS
sbi PORTC,RW
clr temp
out DDRB,temp
sbi PORTC,EN
in value,PORTB
cbi PORTC,EN
ret */

