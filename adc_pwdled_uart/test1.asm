/*
 * AVRAssembler1.asm
 *
 *  Created: 30.10.2019 19:32:42
 *   Author: mike
 */ 

.def temp=r16
.def value=r17
.def vallow=r18
.def valhigh=r19
.def number=r18

.equ CLOCK=8000000
.equ MYUBRR=CLOCK/16/9600-1

.include "m328pdef.inc"


.org 0
rjmp RESET
.org OVF0addr
rjmp TIM0
.org OVF1addr
//.org OC1Aaddr
rjmp TIM1

.org INT_VECTORS_SIZE


TIM0:
push temp
push ZL
push ZH
in temp,SREG
push temp

ldi temp,255-CLOCK/256/1000
out TCNT0,temp

ldi ZL,LOW(TCNT)
ldi ZH,HIGH(TCNT)

ld temp,Z
subi temp,-1
st Z+,temp
ld temp,Z
sbci temp,-1
st Z+,temp
ld temp,Z
sbci temp,-1
st Z+,temp
ld temp,Z
sbci temp,-1
st Z+,temp

pop temp
out SREG,temp
pop ZH
pop ZL
pop temp

reti


TIM1:
	push temp
push ZL
push ZH
in temp,SREG
push temp


lds temp,ANALOGCNT
inc temp
sts ANALOGCNT,temp

cpi temp,20 //PWM width
lds temp,LEDON
brcc ANALOGOFF

ori temp,0b10
rjmp ANALOGDONE
ANALOGOFF:


andi temp,0b11111101


ANALOGDONE:
sts LEDON,temp
out PORTB,temp


pop temp
out SREG,temp
pop ZH
pop ZL
pop temp

reti


DELAY: //r18,r19=millis
push temp
push r20
push r21
push r22
push r23
push r0
push ZH
push ZL

ldi ZL,LOW(TCNT)
ldi ZH,HIGH(TCNT)


ld temp,Z+
add temp,vallow
mov r20,temp
ld temp,Z+
adc temp,valhigh
mov r21,temp
ld temp,Z+

clr r0

adc temp,r0
mov r22,temp
ld temp,Z+
adc temp,r0
mov r23,temp


LPDEL:

ldi ZL,LOW(TCNT)
ldi ZH,HIGH(TCNT)
ld temp,Z+
sub temp,r20
ld temp,Z+
sbc temp,r21
ld temp,Z+
sbc temp,r22
ld temp,Z+
sbc temp,r23

brcs LPDEL

pop ZL
pop ZH
pop r0
pop r23
pop r22
pop r21
pop r20
pop temp
ret

OUTBYTE: //r17 BYTE
lds temp,UCSR0A
sbrs temp,UDRE0
rjmp OUTBYTE
sts UDR0,value
ret

OUTNUM: //number NUM
push temp
push value

clr value
LPDIG2:
subi number,100
brcs OUTDIG2
inc value
rjmp LPDIG2
OUTDIG2:
subi number,-100
cpi value,0
breq LPDIG1
subi value,-'0'
rcall OUTBYTE

clr value
LPDIG1:
subi number,10
brcs OUTDIG1
inc value
rjmp LPDIG1
OUTDIG1:
subi number,-10
cpi value,0
breq LPDIG0
subi value,-'0'
rcall OUTBYTE

LPDIG0:
mov value,number
subi value,-'0'
rcall OUTBYTE

ldi value,' '
rcall OUTBYTE

pop value
pop temp
ret


RESET: 

///////////////////////////INIT


//ldi temp,0b11 << WGM00 //fastpwm
//out TCCR0A,temp

ldi temp,(1<<TOIE0) 
sts TIMSK0,temp
ldi temp,0b100 << CS00 // div256
out TCCR0B,temp
ldi temp,255-CLOCK/256/1000 //32 ticks per int
out TCNT0,temp


//clr temp
//sts OCR1AH,temp
//ldi temp,255
//sts OCR1AL,temp


ldi temp,(1<<TOIE1) //| (1<<OCIE1A)
sts TIMSK1,temp
ldi temp,(0b001<<CS10) | (0b01 <<WGM12) //nodiv 8bit fastpwm
sts TCCR1B,temp
ldi temp, 0b01<<WGM10
sts TCCR1A,temp


ldi temp,0b0111
out DDRB,temp
ldi temp,1
out PORTB,temp
sts LEDON,temp    // PB0-2 leds, B3 - BUTTON, LED0 ON

//ldi temp,1
//out DDRC,temp
//out PORTC,temp  //PC0 - LED, ON



clr temp
sts TCNT,temp
sts TCNT+1,temp
sts TCNT+2,temp
sts TCNT+3,temp
sts ANALOGCNT,temp



//ADC
ldi temp,(0 << REFS0) | (0 << MUX0) | (1 << ADLAR)
sts ADMUX,temp
ldi temp, (1 << ADEN) | (0b110 << ADPS0)
sts ADCSRA,temp


//USART
ldi temp,LOW(MYUBRR)
sts UBRR0L,temp
ldi temp,HIGH(MYUBRR)
sts UBRR0H,temp
ldi temp,1<<TXEN0
sts UCSR0B,temp
ldi temp,0b11 << UCSZ00
sts UCSR0C,temp
clr temp
sts UCSR0A,temp


sei


///////////////////////////INIT END


lp:

ldi r18,LOW(1)
ldi r19,HIGH(1)
rcall DELAY  //1 sec


ldi temp, (1 << ADEN) | (0b110 << ADPS0) | (1 << ADSC) | (1 << ADIF)
sts ADCSRA,temp

WAITADC:
lds temp,ADCSRA
sbrs temp,ADIF
rjmp WAITADC


lds number,ADCH
rcall OUTNUM

cli

lds temp,LEDON
ldi r17,1
eor temp,r17

andi temp, (1<<2) ^ 0xff
sbic PINB,3
ori temp,1<<2  
sts LEDON,temp

sei

rjmp lp




.dseg
TCNT: .byte 4
LEDON: .byte 1
ANALOGCNT: .byte 1
