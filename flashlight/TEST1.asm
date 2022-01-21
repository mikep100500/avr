.include "m328pdef.inc"

.cseg
.org 0
rjmp reset

.org	INT0addr
reti

.org	INT1addr
reti

.org	PCI0addr
reti

.org	PCI1addr
reti

.org	PCI2addr
reti

.org	WDTaddr
reti

.org	OC2Aaddr
reti

.org	OC2Baddr
reti

.org	OVF2addr
reti

.org	ICP1addr
reti

.org	OC1Aaddr
reti

.org	OC1Baddr
reti

.org	OVF1addr
reti

.org	OC0Aaddr
reti

.org	OC0Baddr
reti

.org	OVF0addr
rjmp timer0

.org	SPIaddr
reti

.org	URXCaddr
rjmp uart_rx

.org	UDREaddr
reti

.org	UTXCaddr
reti

.org	ADCCaddr
reti

.org	ERDYaddr
reti

.org	ACIaddr
reti

.org	TWIaddr
reti

.org	SPMRaddr
reti



.org int_vectors_size


reset: 


ldi r16,high(RAMEND)
out sph,r16
ldi r16,low(RAMEND)
out spl,r16


;ldi r16, 1<<rxcie0
;sts ucsr0b, r16


clr r16
sts tcnt,r16

ldi r16,5<<CS00
out tccr0b,r16
ldi r16,1 << toie0 
sts timsk0,r16 

cbi portb,0

sei

loop: nop
rjmp loop

uart_rx:
lds r16, udr0
reti

timer0:
push r16
;push sreg
lds r16,tcnt
inc r16
andi r16,1
sts tcnt,r16
out portb,r16
pop r16
reti


.dseg
tcnt: .byte 1


