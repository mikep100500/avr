.macro MOVEMEM ; 

push r16
push r0

push zh
push zl
push yh
push yl


ldi zh,high(@0)
ldi zl,low(@0)
ldi yh,high(@1)
ldi yl,low(@1)
ldi r16,@2

loop:
ld r0,z+
st y+,r0
dec r16
brne loop

pop yl
pop yh
pop zl
pop zh
pop r0
pop r16


.endm

.macro MOVEPMEM ; 

push r16
push r0

push zh
push zl
push yh
push yl


ldi zh,high(@0*2)
ldi zl,low(@0*2)
ldi yh,high(@1)
ldi yl,low(@1)
ldi r16,@2

loop:
lpm 
st y+,r0
ld r0,z+
dec r16
brne loop

pop yl
pop yh
pop zl
pop zh
pop r0
pop r16


.endm

