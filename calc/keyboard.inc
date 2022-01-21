
KEYBINIT:
ldi temp,0b11110000
out DDRD,temp
ldi temp,0b00001111
out PORTD,temp
ret


READSYM:

//rcall KEYBINIT
ldi temp,0b01111111

out PORTD,temp
in value,PIND

lsr value
brcs RS4
ldi value,'1'
ret

RS4:
lsr value
brcs RS7
ldi value,'4'
ret

RS7:
lsr value
brcs RSSTAR
ldi value,'7'
ret

RSSTAR:
lsr value
brcs RS2
ldi value,'*'
ret


RS2:
ldi temp,0b10111111
out PORTD,temp
in value,PIND

lsr value
brcs RS5
ldi value,'2'
ret

RS5:
lsr value
brcs RS8
ldi value,'5'
ret

RS8:
lsr value
brcs RS0
ldi value,'8'
ret

RS0:
lsr value
brcs RS3
ldi value,'0'
ret


RS3:
ldi temp,0b11011111
out PORTD,temp
in value,PIND

lsr value
brcs RS6
ldi value,'3'
ret

RS6:
lsr value
brcs RS9
ldi value,'6'
ret

RS9:
lsr value
brcs RSHASH
ldi value,'9'
ret

RSHASH:
lsr value
ldi value,'#'
ret
