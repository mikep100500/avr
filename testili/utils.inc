; «адержка в милесекундах
;  @0 Ц параметр в диапазоне 1 Ц 65535 (количество миллисекунд)
; @1 Ц „астота в √ерцах ( >= 1,3 MHz)
DELAY_MS:
	push R19				
	push R18				
	push R17				
	push R16				

;	ldi R18, LOW(@0)		
;	ldi R19, HIGH(@0)		

	cpi R18, 0				
	breq re_init 			
_cicl_msl:
	DELAY_C F_CPU/1000			
	subi R18, 1				
	cpi R18, 0				
	breq re_init 			
	rjmp _cicl_msl			
re_init:
	cpi R19, 0			
	breq _end_c				
	subi R19, 1				
	ldi R18, 255			
	DELAY_C (F_CPU/1000)-255*5	
	rjmp _cicl_msl

_end_c:

	pop R16
	pop R17
	pop R18
	pop R19
ret

