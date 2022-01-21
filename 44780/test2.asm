.include "m328pdef.inc"



.equ RW=0
.equ RS=1
.equ EN=2


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



.def value=r20
.def temp=r21
.def i=r22




.equ FOSC=1843200 // Clock Speed
.equ BAUD=9600
.equ MYUBRR=FOSC/16/BAUD-1


.org 0
rjmp RESET
.org OVF0addr
rjmp TIMER0

.org INT_VECTORS_SIZE

RESET:


//stack
ldi temp,HIGH(RAMEND)
out SPH,temp
ldi temp,LOW(RAMEND)
out SPL,temp


//button init
cbi DDRD,0
sbi PORTD,0
sbi PIND,0


//uart init

ldi temp, LOW(MYUBRR)
sts UBRR0H, temp
ldi temp, HIGH(MYUBRR)
sts UBRR0L, temp
ldi temp, (1<<RXEN0)|(1<<TXEN0)
sts UCSR0B,temp
ldi temp, (1<<USBS0)|(3<<UCSZ00)
sts UCSR0C,temp



ldi value,FUNCTION | BITS8 | LINES2 
RCALL WRITECMD



ldi value,CLEAR
RCALL WRITECMD

ldi value, ONOFF | DISPLAY
RCALL WRITECMD



ldi i,(fuckend-fuckoff)*2
ldi zh,high(fuckoff*2)
ldi zl,low(fuckoff*2)

prnloop:

lpm value,z+
RCALL WRITEDATA
dec i
brne prnloop


;ldi temp, 1 << COM0A0 | 2<<WGM00
;out TCCR0A,temp
;ldi temp,128
;out OCR0A,temp

;ldi temp,4<<CS00 | 0 << WGM02
ldi temp,1
out TCCR0B, temp 
ldi temp,1<<TOIE0 ; OCIE0A
sts TIMSK0, temp


sei

loop: rjmp loop


TIMER0:
;ldi value, SHIFT | SCREEN
;RCALL WRITECMD 
lds temp, TCNT
inc temp
sts TCNT,temp

sbic PIND,0
rjmp timerexit

//ldi value,CLEAR
//rcall WRITECMD

lds value,TCNT
andi value,15

rcall PRINTINTSERIAL

ldi value, 32
rcall WRITEDATAS

lds value, TCNT
lsr value
lsr value
lsr value
lsr value

rcall PRINTINTSERIAL


timerexit:
reti



fuckoff: .db "fuckoff             fuckoff             "
fuckend:


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



PRINTINTSERIAL: ; <20


cpi  value,10
brcs onedigits
subi value,10
push value
ldi value,49
rcall WRITEDATAS
pop value
onedigits: subi value,-48
rcall WRITEDATAS
ret


WRITEDATAS:


lds temp,UCSR0A
andi temp,1<<UDRE0
breq WRITEDATAS

sts UDR0,value
ret


WAITBUSY:
sbi PORTC,RW
cbi PORTC,RS
sbi PORTC,EN
in temp,PORTB
lsl temp
cbi PORTC,EN
brcs WAITBUSY
ret


WRITEBYTE: 
cbi PORTC,RW
ser r16
out DDRB,r16
sbi PORTC,EN
out PORTB,value
cbi PORTC,EN
clr r16
out DDRB,r16
ret

WRITEDATA:
rcall WAITBUSY
sbi PORTC,RS
rjmp WRITEBYTE

WRITECMD:
rcall WAITBUSY
cbi PORTC,RS
rjmp WRITEBYTE

READDATA:
rcall WAITBUSY
sbi PORTC,RS
sbi PORTC,RW
clr r16
out DDRB,r16
sbi PORTC,EN
in value,PORTB
cbi PORTC,EN
ret


.dseg
 TCNT: .byte 1

 
