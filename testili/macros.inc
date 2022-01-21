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

.macro CLEARMEM

push r16
push r0
push zh
push zl
clr r0

ldi zh,high(@0)
ldi zl,low(@0)
ldi r16,@1

loop:
st Z+,r0
dec r16
brne loop

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



; Задержка в циклах
; @0 – параметр в диапазоне 9-255 (количество циклов)
.macro DELAY_CL
	;push R16
	
	ldi R16, LOW(@0)-5
_delay_cl:
	subi R16, 4
	cpi R16, 4
	brsh _delay_cl
	
	cpi R16, 1
	breq end_cl_1	
	cpi R16, 0
	breq end_cl
	cpi R16, 2
	breq end_cl
	rjmp end_cl
			
end_cl_1:
	NOP
	NOP
	NOP
end_cl:
.endm	





; Задержка в циклах
; @0 – параметр в диапазоне 15-65535 (количество циклов)
.macro DELAY_C
	ldi R16, LOW(@0)	
	cpi R16, 17			
	brsh fault			
	rjmp init_R17		
	
fault:					
	DELAY_CL LOW(@0-7)	
	
init_R17:
	ldi R17, HIGH(@0)	
	
	cpi R17, 0			
	breq end_c				
	
new_cycle:				
	subi R17, 1			
	DELAY_CL 252		 
	cpi R17, 0			
	brne new_cycle		
	NOP					
end_c:
.endm




