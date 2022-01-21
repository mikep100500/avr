.equ CLOCK=8000000
.equ MYUBRR=CLOCK/16/9600-1

UART_INIT:

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
